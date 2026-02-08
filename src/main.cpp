#include <Arduino.h>
#include <Wire.h>

#include "config.h"

#include "sensors/sensor_bme680.h"
#include "sensors/sensor_dht22.h"
#include "sensors/sensor_pms5003.h"

#include "processing/fusion.h"
#include "time_sync.h"

static uint32_t lastSample=0;
static uint32_t lastDHT =0;

static DHT22Data dht_last;
static BME680Data bme_last;
static PMS5003Data pms_last;


void setup(){
  Serial.begin(DEBUG_BAUD);
  delay(200);


  Wire.begin(PIN_I2C_SDA,PIN_I2C_SCL);

  bool bme_ok=bme680_init();
  dht22_init(PIN_DHT);

  Serial2.begin(PMS_BAUD,SERIAL_8N1, PIN_UART2_RX, PIN_UART2_TX);
  pms5003_init(Serial2);


  pinMode(PIN_PMS_SET,OUTPUT);
  digitalWrite(PIN_PMS_SET,HIGH);

  Serial.println("# boot");
  Serial.print("# bme680_init"); Serial.println(bme_ok ? "OK":"FAIL");

  
  const char* WIFI_SSID = "SSID_YAZ";
  const char* WIFI_PASS = "SIFRE_YAZ";

  bool tOK = timeSync_begin(WIFI_SSID, WIFI_PASS, "pool.ntp.org", 3*3600, 0);
  Serial.print("# ntp_sync="); Serial.println(tOK ? "OK" : "FAIL");
  Serial.print("# time="); Serial.println(timeSync_iso8601());


}

void loop(){
  uint32_t now=millis();

  if(now-lastDHT>=DHT_MIN_PERIOD_MS){
    lastDHT=now;
    DHT22Data d=dht22_read();
    if (d.ok) dht_last=d;
  }

  BME680Data b=bme680_read();
  if (b.ok) bme_last=b;


  PMS5003Data p=pms5003_read(Serial2,1200);
  if(p.ok) pms_last=p;


  if (now-lastSample>= SAMPLE_PERIOD_MS){
    lastSample=now;

    uint64_t ts_ms=(uint64_t)now;

    FusionSample s=fuse(ts_ms,bme_last,dht_last,pms_last);

    Serial.println(toJsonLine(s));
  }

  
  uint64_t ts_ms = timeSync_isValid() ? timeSync_epochMs() : (uint64_t)millis();
  FusionSample s = fuse(ts_ms, bme_last, dht_last, pms_last);
  Serial.println(toJsonLine(s));

}