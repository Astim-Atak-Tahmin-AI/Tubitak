#pragma once
#include <Arduino.h>


struct BME680Data {
    bool ok=false;
    float temp_c=NAN;
    float rh=NAN;
    float press_hpa=NAN;
    float gas_ohms=NAN;
    float voc_proxy=NAN;
};

bool bme680_init();
BME680Data bme680_read();