#include "sensor_pms5003.h"

static bool findHeader(HardwareSerial& ser, uint32_t timeout_ms){
    uint32_t t0=millis();
    while (millis()-t0<timeout_ms){
        if(ser.available()>=2){
            int b1=ser.peek();
            if (b1==0x42){
                ser.read();
                int b2=ser.read();
                if(b2==0x4D) return true;
            } else {
                ser.read();
            }
        }
        delay(1);

    }
    return false;
}


static uint16_t u16(const uint8_t* b, int idx){
    return ((uint16_t)b[idx]<<8)|b[idx+1];
}

bool pms5003_init(HardwareSerial& ser) {
    return true;
}

PMS5003Data pms5003_read(HardwareSerial& ser, uint32_t timeout_ms){
    PMS5003Data out;

    if(!findHeader(ser, timeout_ms)){
        out.ok=false;
        return out;
    }

    uint8_t frame[30];
    uint32_t t0=millis();
    int got=0;
    while(got<30 && millis() -t0 < timeout_ms){
        if(ser.available()){
            frame[got++]=(uint8_t)ser.read();

        }else{
            delay(1);
        }
    }
    if(got !=30){
        out.ok=false;
        return out;
    }

    uint16_t checksum_rx=u16(frame,28);
    uint32_t sum=0x42+0x4D;
    for(int i=0;i<28; i++) sum+=frame[i];
    uint16_t checksum_calc=(uint16_t)(sum & 0XFFFF);


    if(checksum_calc != checksum_rx){
        out.ok=false;
        return out;
    }

    out.pm1_0=u16(frame,8);
    out.pm2_5=u16(frame,10);
    out.pm10=u16(frame,12);
    out.ok=true;
    return out;

}

void pms5003_sleep(bool enable, int setPin){
    pinMode(setPin, OUTPUT);
    digitalWrite(setPin, enable ? LOW: HIGH);
}