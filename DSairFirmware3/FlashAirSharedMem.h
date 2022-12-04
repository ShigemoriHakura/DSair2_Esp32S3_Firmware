#ifndef FlashAirSharedMem_h
#define FlashAirSharedMem_h

int8_t SharedMemInit(int _cs);

//Internal
uint8_t spi_trans(uint8_t x);
void cs_release();
int8_t sd_cmd(uint8_t cmd,uint32_t arg);

#endif
