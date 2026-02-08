#pragma once
#include <Arduino.h>


struct DHT22Data {
    bool ok=false;
    float temp_c=NAN;
    float rh=NAN;

};

bool dht22_init(int pin);
DHT22Data dht22_read();