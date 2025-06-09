#include <Arduino.h>
#include <NimBLEDevice.h>
#include "Bluetooth.h"
#include "I2C_Pressure_sens.h"
#include "DFRobot_OxygenSensor.h"


float sensorValue = 0.0;
float sensorValue2x = 0.0;
int sensorPin = 4;   // GPIO34, wejście analogowe ADC1



Bluetooth ergospiro_server;
I2C_Pressure_sens pressure_sens;
I2C_Pressure_sens co2_sens;
I2C_Pressure_sens o2_sens;

void co2Task(void* pvParameters)
{
  while (1)
  {
    //sensorValue2x = co2_sens.get_sens_co2_val();
    vTaskDelay(10000 / portTICK_PERIOD_MS);
  }
}
void o2Task(void* pvParameters)
{
  while (1)
  {
   // float oxygenData = o2_sens.getOxygenData(2);
  // Serial.print(" oxygen concentration is ");
  // Serial.print(oxygenData);
  // Serial.println(" %vol");
    sensorValue2x = o2_sens.get_sens_o2_val();

    vTaskDelay(70 / portTICK_PERIOD_MS);
  }
}

void setup() {
  
  Serial.begin(115200);
  delay(1000);
  ergospiro_server.bt_init();
  //analogReadResolution(12);
  o2_sens.init(0x73,0);
  //o2_sens.calibrate();
  
  pressure_sens.init(64,0);
  co2_sens.init(0x62,1);
  Serial.println("Setup");

  xTaskCreate(
        co2Task,      // Funkcja taska
        "BT Service",    // Nazwa taska (do debugowania)
        4096,           // Rozmiar stosu (dostosuj w zależności od potrzeb)
        NULL,           // Parametry przekazywane do funkcji (NULL = brak)
        1,              // Priorytet (1 = niski, wyższe liczby = wyższy priorytet)
        NULL            // Wskaźnik do uchwytu taska (NULL = nie potrzebujemy)
    );
      xTaskCreate(
        o2Task,      // Funkcja taska
        "BT Service",    // Nazwa taska (do debugowania)
        4096,           // Rozmiar stosu (dostosuj w zależności od potrzeb)
        NULL,           // Parametry przekazywane do funkcji (NULL = brak)
        1,              // Priorytet (1 = niski, wyższe liczby = wyższy priorytet)
        NULL            // Wskaźnik do uchwytu taska (NULL = nie potrzebujemy)
    );

}


void loop() {
    sensorValue2x = o2_sens.get_sens_o2_val();
    vTaskDelay(3 / portTICK_PERIOD_MS);
    sensorValue=pressure_sens.get_sens_press_val();
    
    vTaskDelay(3 / portTICK_PERIOD_MS);

}
