#include <Wire.h>

class I2CDevice {
public:
    I2CDevice(uint8_t sda, uint8_t scl, uint32_t freq = 100000) {
        _sda = sda;
        _scl = scl;
        _freq = freq;
    }

    void begin() {
        Wire.begin(_sda, _scl, _freq);
    }

    bool writeByte(uint8_t deviceAddress, uint8_t registerAddress, uint8_t data) {
        Wire.beginTransmission(deviceAddress);
        Wire.write(registerAddress);
        Wire.write(data);
        return (Wire.endTransmission() == 0);
    }

    bool writeBytes(uint8_t deviceAddress, uint8_t registerAddress, const uint8_t* data, size_t length) {
        Wire.beginTransmission(deviceAddress);
        Wire.write(registerAddress);
        Wire.write(data, length);
        return (Wire.endTransmission() == 0);
    }

    bool readByte(uint8_t deviceAddress, uint8_t registerAddress, uint8_t &data) {
        Wire.beginTransmission(deviceAddress);
        Wire.write(registerAddress);
        if (Wire.endTransmission(false) != 0) {
            return false;
        }

        Wire.requestFrom(deviceAddress, (uint8_t)1);
        if (Wire.available()) {
            data = Wire.read();
            return true;
        }
        return false;
    }

    bool readBytes(uint8_t deviceAddress, uint8_t registerAddress, uint8_t *buffer, size_t length) {
        Wire.beginTransmission(deviceAddress);
        Wire.write(registerAddress);
        if (Wire.endTransmission(false) != 0) {
            return false;
        }

        Wire.requestFrom(deviceAddress, (uint8_t)length);
        size_t index = 0;
        while (Wire.available() && index < length) {
            buffer[index++] = Wire.read();
        }

        return index == length;
    }

private:
    uint8_t _sda;
    uint8_t _scl;
    uint32_t _freq;
};