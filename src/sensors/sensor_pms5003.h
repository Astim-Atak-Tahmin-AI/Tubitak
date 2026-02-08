#pragma once
#include <Arduino.h>

struct PMS5003Data {
    bool ok=false;
    uint16_t pm1_0=0;
    uint16_t pm2_5=0;
    uint16_t pm10=0;
};


bool pms5003_init(HardwareSerial& ser);
PMS5003Data pms5003_read(HardwareSerial& ser,uint32_t
timeout_ms=1200);

void pms5003_sleep(bool enable, int setPin);