#include "sensor_dht22.h"
#include <DHT.h>


static DHT* dht=nullptr;


bool dht22_init(int pin){
    if (dht) return true;
    dht= new DHT(pin, DHT22);
    dht->begin();
    return true;
}

DHT22Data dht22_read() {
    DHT22Data out;
    if (!dht) return out;


    float h=dht->readHumidity();
    float t=dht->readTemperature();


    if (isnan(h)||isnan(t)) {
        out.ok=false;
        return out;
    }

    out.ok=true;
    out.temp_c=t;
    out.rh=h;
    return out;
}