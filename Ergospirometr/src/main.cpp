#include <Arduino.h>
#include <NimBLEDevice.h>
#include "Bluetooth.h"
#include "I2C_Pressure_sens.h"


float sensorValue = 0.0;
float sensorValue2x = 0.0;
int sensorPin = 4;   // GPIO34, wejście analogowe ADC1



Bluetooth ergospiro_server;
I2C_Pressure_sens pressure_sens;

void setup() {
  
  Serial.begin(115200);
  delay(1000);
  ergospiro_server.bt_init();
  //analogReadResolution(12);
  
  pressure_sens.init(64);
  Serial.println("Setup");

}

void loop() {
    sensorValue2x = random(100, 1000) / 10.0;
    sensorValue=pressure_sens.get_sens_val();
    vTaskDelay(5 / portTICK_PERIOD_MS);

}
