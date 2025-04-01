#include <Arduino.h>
#include <NimBLEDevice.h>
#include "Bluetooth.h"


float sensorValue = 0.0;


Bluetooth ergospiro_server;

void setup() {
  Serial.begin(115200);
  ergospiro_server.bt_init();

}

void loop() {
  sensorValue = random(100, 1000) / 10.0;

    vTaskDelay(5 / portTICK_PERIOD_MS);

}
