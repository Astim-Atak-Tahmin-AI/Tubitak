#pragma once
#include <Arduino.h>

bool timeSync_begin(const char* ssid,
                    const char* pass,
                    const char* ntpServer="pool.ntp.org",
                    long gmtOffset_sec=3*3600,
                    int daylightOffset_sec=0);


bool timeSync_isValid();
uint64_t timeSync_epochMs();
String timeSync_iso8601();
