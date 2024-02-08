#include "stm32l4xx_hal.h"

void writeStringToFlash(uint64_t address, const char* str);
void readDataFromFlash(uint64_t address, char* buffer, uint64_t dataSize);
void eraseFlashPage(uint64_t pageAddress);
