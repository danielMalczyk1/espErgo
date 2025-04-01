#ifndef BLUETOOTH_H  // Sprawdza, czy BLUETOOTH_H nie zostało zdefiniowane
#define BLUETOOTH_H  // Definiuje BLUETOOTH_H, aby uniknąć ponownego dołączenia

#include <Arduino.h>
#include <NimBLEDevice.h>
#include <string>




#define SERVICE_UUID        "baad0000-0000-0000-0000-00000000baad"
#define SENSOR_VALUE_UUID   "f00d0000-0000-0000-0000-00000000f00d"
#define REQUEST_UPDATE_UUID "c01d0000-0000-0000-0000-00000000c01d"
#define CHAR_USER_DESC_UUID "2901"
#define CHAR_FORMAT_UUID    "2904"



class Bluetooth {
private:
    int period=1000;
    unsigned long lastUpdateTime;
public:
    void bt_init();
    void sendSensorValue(float ts_sensorValue);
    void setPeriod(int periodToSet){period=periodToSet;};
    void BTService ();


    static void BTServiceWrapper(void* pvParameters);

};

#endif // BLUETOOTH_H  // Koniec bloku warunkowego