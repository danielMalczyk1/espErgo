#include "Bluetooth.h"

static bool requestUpdate = false;
static NimBLEServer* pServer = nullptr;
static bool periodically=false;
extern float sensorValue;


class ServerCallbacks : public NimBLEServerCallbacks {
    void onConnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo) override {
        Serial.printf("Client connected: %s, handle: %d\r\n", 
                      connInfo.getAddress().toString().c_str(),
                      connInfo.getConnHandle());
    }

    void onDisconnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo, int reason) override {
        Serial.printf("Client disconnected, handle: %d, reason: %d\r\n", 
                      connInfo.getConnHandle(), reason);
        if (pServer) {
            pServer->startAdvertising();
        }
    }
} serverCallbacks;

class CharacteristicCallbacks : public NimBLECharacteristicCallbacks {
    void onRead(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) override {
        Serial.printf("Characteristic %s read by client %s\r\n",
                      pCharacteristic->getUUID().toString().c_str(),
                      connInfo.getAddress().toString().c_str());
    }

    void onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) override {
        if (pCharacteristic == nullptr) {
            Serial.println("Error: pCharacteristic is null!");
            return;
        }
        Serial.printf("Characteristic %s written by client %s\r\n",
                      pCharacteristic->getUUID().toString().c_str(),
                      connInfo.getAddress().toString().c_str());
                 if (pCharacteristic->getUUID().toString() == REQUEST_UPDATE_UUID) 
                {
                 std::string value = pCharacteristic->getValue();
                if (value == "P") {
                    
                   periodically = true;
                   Serial.println("Periodic updates ENABLED");
               } else if (value == "S") {
                  periodically = false;
                  Serial.println("Periodic updates DISABLED");
                  requestUpdate = true;
               }
               else
               {
                requestUpdate = true;
               }
            }
    }

    void onSubscribe(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo, uint16_t subValue) override {
        Serial.printf("Client %s subscribed to %s with value %d\r\n",
                      connInfo.getAddress().toString().c_str(),
                      pCharacteristic->getUUID().toString().c_str(), subValue);
    }
} chrCallbacks;


void Bluetooth::bt_init()
{
    Serial.println("Starting BLE Server\r");

    NimBLEDevice::init("Erg");
    pServer = NimBLEDevice::createServer();
    pServer->setCallbacks(&serverCallbacks);
    NimBLEService* pService = pServer->createService(SERVICE_UUID);
  
    NimBLECharacteristic* pValueChar = pService->createCharacteristic(
        SENSOR_VALUE_UUID, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY
    );
    pValueChar->setCallbacks(&chrCallbacks);

        NimBLECharacteristic* pValueChar2 = pService->createCharacteristic(
        SENSOR2_VALUE_UUID, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY
    );
    pValueChar2->setCallbacks(&chrCallbacks);
    
    NimBLEDescriptor* pFormatDesc = pValueChar->createDescriptor(
        CHAR_FORMAT_UUID, NIMBLE_PROPERTY::READ
    );
    uint8_t formatValue[] = {0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    pFormatDesc->setValue(formatValue, sizeof(formatValue));
  
    NimBLECharacteristic* pRequestChar = pService->createCharacteristic(
        REQUEST_UPDATE_UUID, NIMBLE_PROPERTY::WRITE
    );
    pRequestChar->setCallbacks(&chrCallbacks);
    pService->start();
  
    NimBLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setName("Ergo");
    pAdvertising->enableScanResponse(true);
    pAdvertising->start();
    
    Serial.println("Server started\r");
    xTaskCreate(
        BTServiceWrapper,      // Funkcja taska
        "BT Service",    // Nazwa taska (do debugowania)
        4096,           // Rozmiar stosu (dostosuj w zależności od potrzeb)
        this,           // Parametry przekazywane do funkcji (NULL = brak)
        1,              // Priorytet (1 = niski, wyższe liczby = wyższy priorytet)
        NULL            // Wskaźnik do uchwytu taska (NULL = nie potrzebujemy)
    );
}

void Bluetooth::BTServiceWrapper(void* pvParameters) {
    Bluetooth* pThis = static_cast<Bluetooth*>(pvParameters);
    pThis->BTService();  // Wywołanie metody na obiekcie
}


void Bluetooth::sendSensorValue(float ts_sensorValue,std::string uuid) {
    uint8_t valueBytes[4];
    memcpy(valueBytes, &ts_sensorValue, sizeof(ts_sensorValue));
    Serial.printf("Updating sensor value: %f (bytes: %02X %02X %02X %02X)\r\n", 
                  ts_sensorValue, valueBytes[0], valueBytes[1], valueBytes[2], valueBytes[3]);

    if (pServer) {
        NimBLEService* pSvc = pServer->getServiceByUUID(SERVICE_UUID);
        if (pSvc) {
            NimBLECharacteristic* pChr = pSvc->getCharacteristic(uuid);
            if (pChr) {
                pChr->setValue(valueBytes, sizeof(valueBytes));
                pChr->notify();
                Serial.println("Notification sent\r");
            }
        }
    }
}

void Bluetooth::BTService()
{
    while(1)
    {
        if (periodically)
        {
            //Serial.println("Wszedlem\r");   
            if (period<10)
            return;
    
            if (millis() - lastUpdateTime >= 20) {
               lastUpdateTime = millis();
                sendSensorValue(sensorValue,SENSOR_VALUE_UUID);
                sendSensorValue(sensorValue2x,SENSOR2_VALUE_UUID);
            
            }
            
        }
        if (requestUpdate) {
            requestUpdate = false;
            sendSensorValue(sensorValue,SENSOR_VALUE_UUID);
            sendSensorValue(sensorValue2x,SENSOR2_VALUE_UUID);
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
