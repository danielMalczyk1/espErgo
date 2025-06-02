#include "I2C_Pressure_sens.h"

bool I2C_Pressure_sens::init(int sens_addr)
{
     sens_address=sens_addr;
    // uint8_t whoAmI_reg=whoAmI_reg_addr;
    // uint8_t init_reg_ctrl1[2]={0x20,0xA0};
    // uint8_t init_reg_ctrl2[2]={0x21,0x40};
    // uint8_t testID={0};

     sens.begin();
    // sens.readByte(sens_address,whoAmI_reg,testID);
    
    // if (testID==whoAmI_reg_val)
    // {;
    //     sens.writeByte(sens_address,init_reg_ctrl1[0],init_reg_ctrl1[1]);
    //     sens.writeByte(sens_address,init_reg_ctrl2[0],init_reg_ctrl2[1]);
    //     Serial.println("Udala sie inicjacja press sens\r");
    // }
    // else{
    //     Serial.println("Nie udala sie inicjacja press sens\r");
    // }
    ;
    return true;
}

float I2C_Pressure_sens::get_sens_val()
{
    char buffer[50];
    uint8_t pressure[3];
    //uint8_t reg_pres=0x28|0x80;
    uint8_t reg_pres=0xF1;
    int16_t pressure_val;
    char buf[3][20];
    pressure[0]=0;
    pressure[1]=0;
    pressure[2]=0;
    


    float press_conv;
    sens.readBytes(sens_address,reg_pres,pressure,2);
    pressure_val=((int16_t)pressure[0] << 8) | pressure[1];
    press_conv=(pressure_val/32767.0)*500*0.95;

    //if (press_conv==754.8125)
    //{
    //    init(sens_address);
    //}
        //sprintf(buf[0],"1: %d \n",pressure[0]);
    //sprintf(buf[1],"2: %d \n",pressure[1]);
    //sprintf(buf[2],"3: %d \n",pressure_val);
    //Serial.println(buf[0]);
    //Serial.println(buf[1]);
    //Serial.println(buf[2]);
    
    sprintf(buffer,"Aktualne cisnienie to %6.1f Pa!\n\r",press_conv);
    Serial.println(buffer);
    return press_conv;
}