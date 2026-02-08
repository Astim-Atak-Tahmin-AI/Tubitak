#pragma once
#include <Arduino.h>

static constexpr int PIN_I2C_SDA=21;
static constexpr int PIN_I2C_SCL=22;

static constexpr int PIN_DHT=4;


static constexpr int PIN_UART2_RX=16;
static constexpr int PIN_UART2_TX=17;

static constexpr int PIN_PMS_SET=27;

static constexpr uint32_t SAMPLE_PERIOD_MS=1000;

static constexpr uint32_t DHT_MIN_PERIOD_MS=2000;

static constexpr uint32_t DEBUG_BAUD=115200;

static constexpr uint32_t PMS_BAUD=9600;

