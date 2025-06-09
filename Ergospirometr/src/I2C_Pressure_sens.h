#include <Arduino.h>
#include "I2CDevice.h"

constexpr uint8_t whoAmI_reg_val = 0xBD;

constexpr uint8_t whoAmI_reg_addr = 0x0F;

constexpr uint8_t SDA_PIN = 21;
constexpr uint8_t SCL_PIN = 22;
#define GET_KEY_REGISTER     0x0A


class I2C_Pressure_sens {
    float sens_val;
    int sens_address;
    //int co2_sens_address;
    I2CDevice sens;
    float key;

    public:
    I2C_Pressure_sens() 
    : sens(SDA_PIN, SCL_PIN,100000) {};
    float get_sens_press_val();
    float get_sens_co2_val();
    float get_sens_o2_val();
    void readFlash();
    bool init(int sens_addr, int co2);


};