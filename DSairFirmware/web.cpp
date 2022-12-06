/*
   Copyright (c) 2021 Aaron Christophel ATCnetz.de
   SPDX-License-Identifier: GPL-3.0-or-later
*/
#include <Arduino.h>
#include "web.h"
#include <FS.h>
#include "SPIFFS.h"
#include <ESPmDNS.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFSEditor.h>

#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager/tree/feature_asyncwebserver

#include "Define.h"

const char *http_username = "admin";
const char *http_password = "admin";
AsyncWebServer server(80);

unsigned long hstol(String recv)
{
  char c[recv.length() + 1];
  recv.toCharArray(c, recv.length() + 1);
  return strtoul(c, NULL, 16);
}

void init_web()
{
  WiFi.mode(WIFI_STA);
  WiFiManager wm;
  bool res;
  res = wm.autoConnect("AutoConnectAP", "`");
  if (!res)
  {
    Serial.println("Failed to connect");
    ESP.restart();
  }
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

  // Make accessible via http://dsair.local using mDNS responder
  if (!MDNS.begin("dsair"))
  {
    while (1)
    {
      Serial.println("Error setting up mDNS responder!");
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");
  MDNS.addService("http", "tcp", 80);
  SPIFFS.begin(true);

  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "*");

  server.addHandler(new SPIFFSEditor(SPIFFS, http_username, http_password));

  //?op=130&ADDR=128&LEN=264
  server.on("/command", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    String op = "Unknown";
    String returnStr = "";
    String dataStr = "";
    if (request->hasParam("op"))
    {
      op = request->getParam("op")->value();
    }
    if (request->hasParam("DATA"))
    {
      dataStr = request->getParam("DATA")->value();
    }

    if (op == "130") {
      returnStr = GetStatusString();
    } else if (op == "131") {
      WebInCommand = dataStr;
      returnStr = "OK";
    } else {
      returnStr = "WTF";
    }

    request->send(200, "text/plain", returnStr);
  });

  server.on("/heap", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(200, "text/plain", String(ESP.getFreeHeap()));
  });

  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.htm");

  server.onNotFound([](AsyncWebServerRequest * request)
  {
    if (request->url() == "/" || request->url() == "index.htm")
    { // not uploaded the index.htm till now so notify the user about it
      request->send(200, "text/html", "please use <a href=\"/edit\">/edit</a> with login defined in web.cpp to upload the supplied index.htm to get full useage");
      return;
    }
    Serial.printf("NOT_FOUND: ");
    if (request->method() == HTTP_GET)
      Serial.printf("GET");
    else if (request->method() == HTTP_POST)
      Serial.printf("POST");
    else if (request->method() == HTTP_DELETE)
      Serial.printf("DELETE");
    else if (request->method() == HTTP_PUT)
      Serial.printf("PUT");
    else if (request->method() == HTTP_PATCH)
      Serial.printf("PATCH");
    else if (request->method() == HTTP_HEAD)
      Serial.printf("HEAD");
    else if (request->method() == HTTP_OPTIONS)
      Serial.printf("OPTIONS");
    else
      Serial.printf("UNKNOWN");
    Serial.printf(" http://%s%s\n", request->host().c_str(), request->url().c_str());

    if (request->contentLength())
    {
      Serial.printf("_CONTENT_TYPE: %s\n", request->contentType().c_str());
      Serial.printf("_CONTENT_LENGTH: %u\n", request->contentLength());
    }
    int headers = request->headers();
    int i;
    for (i = 0; i < headers; i++)
    {
      AsyncWebHeader *h = request->getHeader(i);
      Serial.printf("_HEADER[%s]: %s\n", h->name().c_str(), h->value().c_str());
    }
    int params = request->params();
    for (i = 0; i < params; i++)
    {
      AsyncWebParameter *p = request->getParam(i);
      if (p->isFile())
      {
        Serial.printf("_FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
      }
      else if (p->isPost())
      {
        Serial.printf("_POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
      }
      else
      {
        Serial.printf("_GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
      }
    }
    request->send(404);
  });

  server.begin();
}
