DSair2 Firmware & Web App
Copyright(C) 2020- Desktop Station
---------------------------------------

■ 使用方法

WebAppはFlashAir向け、ino,cpp,hファイルはArduino nano向けとなります。

C言語ソースコードは、Arduino IDEを使用してください。
対応ハードウェアは、Arduino nano(ATMEGA328P)です。

WebAppは、FlashAir W-04シリーズ専用です。


■ DSair2, DSmain等 DesktopStation製Arduino利用ソースコードのライセンスについて

Arduino用のスケッチ(ino,h,cpp)は下記に記載の外部ライブラリを除き、原則、LGPLとします。
以下 LGPLの条文です。

 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * LICENSE file for more details.


Web App(SD_WLANフォルダ以下)は、MIT Licenseを適用します。以下 MIT Licenseの条文(和訳)です。

 * 以下に定める条件に従い、本ソフトウェアおよび関連文書のファイル（以下「ソフトウェア」）の複製を取得するすべての人に対し、ソフトウェアを無制限に扱うことを無償で許可します。これには、ソフトウェアの複製を使用、複写、変更、結合、掲載、頒布、サブライセンス、および/または販売する権利、およびソフトウェアを提供する相手に同じことを許可する権利も無制限に含まれます。
 * 上記の著作権表示および本許諾表示を、ソフトウェアのすべての複製または重要な部分に記載するものとします。
 * ソフトウェアは「現状のまま」で、明示であるか暗黙であるかを問わず、何らの保証もなく提供されます。ここでいう保証とは、商品性、特定の目的への適合性、および権利非侵害についての保証も含みますが、それに限定されるものではありません。 作者または著作権者は、契約行為、不法行為、またはそれ以外であろうと、ソフトウェアに起因または関連し、あるいはソフトウェアの使用またはその他の扱いによって生じる一切の請求、損害、その他の義務について何らの責任も負わないものとします。


■ DSair2での使用ライブラリ・ライセンス関連

DSair2の内部ソフトウェアでは、以下のライブラリやソースコードを使用しております。
感謝を申し上げます。ライセンスは、括弧書きの通りです。


- Arduinoファームウェア(本体ソフト)

GPS_NEMA様 FlashAirSharedMem (BSD-3)
Joerg Pleumann様 Railuino S88ライブラリ (LGPL 2.1)
Arduino 各種ライブラリ (LGPL)

- Webアプリ(FlashAir上のソフト)

jQuery (MIT License)  https://jquery.org/license/
jQuery UI (MIT License) https://github.com/jquery/jquery-ui/blob/master/LICENSE.txt
Toastr (MIT License) https://github.com/CodeSeven/toastr/blob/master/LICENSE
Ace (BSD) https://github.com/ajaxorg/ace/blob/master/LICENSE
Google LLC wwwbasic (Apache) https://github.com/google/wwwbasic/blob/master/LICENSE


