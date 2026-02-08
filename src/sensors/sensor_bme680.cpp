#include "sensor_bme680.h"
#include <Wire.h>
#include <Adafruit_BME680.h>

static Adafruit_BME680 bme;


static bool baseline_init=false;
static float gas_baseline=0.0f;

bool bme680_init() {
    Wire.begin();
    if (!bme.begin()) {
        return false;
    }


    bme.setTemperatureOversampling(BME680_OS_8X);
    bme.setHumidityOversampling(BME680_OS_2X);
    bme.setPressureOversampling(BME680_OS_4X);
    bme.setIIRFilterSize(BME680_FILTER_SIZE_3);



    bme.setGasHeater(320,150);
    return true;


}

BME680Data bme680_read() {
    BME680Data out;

    if(!bme.performReading()){
        out.ok=false;
        return out;
    }

    out.ok=true;
    out.temp_c=bme.temperature;
    out.rh=bme.humidity;
    out.press_hpa=bme.pressure/100.0f;
    out.gas_ohms=(float)bme.gas_resistance;


    if(!baseline_init){
        gas_baseline=out.gas_ohms;
        baseline_init=true;
    }else {
        gas_baseline=0.995f*gas_baseline+0.005f* out.gas_ohms;

    }

    float ratio=(gas_baseline>0)?(out.gas_ohms/gas_baseline):
1.0f;
    
    float proxy=1.0f-constrain(ratio,0.0f,2.0f)/2.0f;
    out.voc_proxy=constrain(proxy,0.0f,1.0f);

    return out;
}