#pragma once
#include <Arduino.h>
#include "../sensors/sensor_bme680.h"
#include "../sensors/sensor_dht22.h"
#include "../sensors/sensor_pms5003.h"


struct FusionSample {
    uint64_t ts_ms=0;

    BME680Data bme;
    DHT22Data dht;
    PMS5003Data pms;


    bool ok=false;

};

FusionSample fuse(uint64_t ts_ms,
                  const BME680Data& bme,
                  const DHT22Data& dht,
                  const PMS5003Data& pms);

String toJsonLine(const FusionSample& s);