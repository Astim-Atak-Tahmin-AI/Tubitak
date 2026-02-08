#include "time_sync.h"
#include <Wifi.h>
#include "time.h"



static bool s_timeValid=false;

static bool obtainTimeOnce(uint32_t waitMs=15000){
    struct tm timeinfo;
    uint32_t start=millis();
    while(millis()-start<waitMs){
        if(getLocalTime(&timeinfo,1000)){
            if(timeinfo.tm_year+1900>=2020) return true;
        }
        delay(200);
    }
    return false;
}

bool timeSync_begin(const char* ssid,
                    const char* pass,
                    const char* ntpServer,
                    long gmtOffset_sec,
                    int daylightOffset_sec){

WİFİ.mode(WIFI_STA);
WİFİ.begin(ssid,pass);


uint32_t t0=millis();

while(WİFİ.status() !=WL_CONNECTED && millis()-t0<15000){
    delay(300);
}
if(WİFİ.status()!=WL_CONNECTED){
    s_timeValid=false;
    return false;
}


    configTime(gmtOffset_sec,daylightOffset_sec,ntpServer);

    s_timeValid=obtainTimeOnce();

    return s_timeValid;

}

bool timeSync_isValid(){
    return s_timeValid;
}


uint64_t timeSync_epochMs(){
    time_t nowSec=time(nullptr);
    if(nowSec<100000) return 0;
    return (uint64_t)nowSec *1000ULL;
}


String timeSync_iso8601(){
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo,1000)) return String("null");

    char buf[32];



    snprintf(buf,sizeof(buf),
            "%04d-%02d-%02dT%02d:%02d:%02d+03:00",
             timeinfo.tm_year + 1900,
             timeinfo.tm_mon + 1,
             timeinfo.tm_mday,
             timeinfo.tm_hour,
             timeinfo.tm_min,
             timeinfo.tm_sec);
    return String(buf);




    
}

static String dailyLogPath(){
    String iso=timeSync_iso8601();
    if (iso=="null") return "/log_unknown.jsonl";
    String day=iso.substring(0,10);
    day.replace("-","");
    return "/log_"+day+".jsonl";
}