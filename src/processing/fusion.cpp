#include "fusion.h"

FusionSample fuse(uint64_t ts_ms,
                  const BME680Data& bme,
                  const DHT22Data& dht,
                  const PMS5003Data& pms){

FusionSample s;
s.ts_ms=ts_ms;
s.bme=bme;
s.dht=dht;
s.pms=pms;

s.ok=(pms.ok && (bme.ok || dht.ok));
return s;

}


static String f2s(float x, int digits=2){
    if(isnan(x)) return "null";
    return String(x,digits);

}

String toJsonLine(const FusionSample& s){
   String j= "{";
    
  j += "\"ts_ms\":" + String((unsigned long long)s.ts_ms);

  j += ",\"pms\":{\"ok\":" + String(s.pms.ok ? "true":"false");
  j += ",\"pm1\":"  + String(s.pms.pm1_0);
  j += ",\"pm25\":" + String(s.pms.pm2_5);
  j += ",\"pm10\":" + String(s.pms.pm10) + "}";

  j += ",\"bme\":{\"ok\":" + String(s.bme.ok ? "true":"false");
  j += ",\"t\":" + f2s(s.bme.temp_c);
  j += ",\"rh\":" + f2s(s.bme.rh);
  j += ",\"p\":" + f2s(s.bme.press_hpa);
  j += ",\"gas_ohm\":" + f2s(s.bme.gas_ohms, 0);
  j += ",\"voc_proxy\":" + f2s(s.bme.voc_proxy, 3) + "}";

  j += ",\"dht\":{\"ok\":" + String(s.dht.ok ? "true":"false");
  j += ",\"t\":" + f2s(s.dht.temp_c);
  j += ",\"rh\":" + f2s(s.dht.rh) + "}";

  j += ",\"ok\":" + String(s.ok ? "true":"false");
  j += "}";
  return j;

}