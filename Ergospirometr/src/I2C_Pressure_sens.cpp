#include "I2C_Pressure_sens.h"

bool I2C_Pressure_sens::init(int sens_addr, int co2)
{
     sens_address=sens_addr;
     sens.begin();
     uint8_t sth[2];
     sth[0]=0xf;
     sth[1]=0xf;

    delay(200);
     if (co2==1)
     {
        sens.write16Bytes(sens_address,0x21b1,sth,2);
     }

    // uint8_t whoAmI_reg=whoAmI_reg_addr;
    // uint8_t init_reg_ctrl1[2]={0x20,0xA0};
    // uint8_t init_reg_ctrl2[2]={0x21,0x40};
    // uint8_t testID={0};

     
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

float I2C_Pressure_sens::get_sens_co2_val()
{
    char buffer[50];
    uint8_t pressure[6];
    //uint8_t reg_pres=0x28|0x80;
    uint16_t reg_pres=0xec05;
    int16_t pressure_val;
    char buf[3][20];
    pressure[0]=0;
    pressure[1]=0;
    pressure[2]=0;
    
    float press_conv;
    sens.read16Bytes(sens_address,reg_pres,pressure,6);
    pressure_val=((int16_t)pressure[0] << 8) | pressure[1];
    press_conv=pressure_val;

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
    
    sprintf(buffer,"Aktualny co2 to %6.1f ppm!\n\r",press_conv);
    Serial.println(buffer);
    return press_conv;
}

void I2C_Pressure_sens::readFlash()
{
  uint8_t value = 0;
  sens.readByte(sens_address,GET_KEY_REGISTER,value);
  if(value == 0){
    this->key = 20.9 / 120.0;
  }else{
    this->key = (float)value / 1000.0;
  }
}


float I2C_Pressure_sens::get_sens_o2_val()
{
    readFlash();
    char buffer[50];
    vTaskDelay(50 / portTICK_PERIOD_MS);
    uint8_t pressure[3];
    //uint8_t reg_pres=0x28|0x80;
    uint8_t reg_pres=0x03;
    int16_t pressure_val;
    char buf[3][20];
    pressure[0]=0;
    pressure[1]=0;
    pressure[2]=0;
    
    float press_conv;
    sens.readBytes(sens_address,reg_pres,pressure,3);
    pressure_val= pressure[0];
    press_conv=key*pressure_val;//)*500*0.95;

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
    
    sprintf(buffer,"Aktualny tlen to %6.1f proc!\n\r",press_conv);
    Serial.println(buffer);
    return press_conv;
}


float I2C_Pressure_sens::get_sens_press_val()
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