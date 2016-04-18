

#include "Time.h"
#include "Eeprom.h"

static u8 time_sec = 0;
static u8 time_min = 0;
static u8 time_hou = 0;
static u8 time_day = 0;

void TimeInit(void) {
    TIM4_IER = 0x00;   
    TIM4_EGR = 0x01;
    TIM4_PSCR = 0x07;
    TIM4_ARR = 0xFA;
    TIM4_IER = 0x01;
    TIM4_CR1 = 0x01;  
    
    if(EepromRead(20) != 0x55) {
        EepromWrite(20,0x55);
        EepromWrite(21,0x00);
        EepromWrite(22,0x00);
        EepromWrite(23,0x00);
        EepromWrite(24,0x00);
    }
    time_sec = EepromRead(21);
    time_min = EepromRead(22);  
    time_hou = EepromRead(23); 
    time_day = EepromRead(24); 
}
/*

*/
static u8 time_sec_flag = 0;

u8 TimeGetSecFlag(void) {
    return time_sec_flag;
}

void TimeSetSecFlag(u8 cmd) {
    time_sec_flag = cmd;
}
/*
light---------------------
*/
static u16 time_light_flag = 1;

u16 TimeGetLightFlag(void) {
    return time_light_flag;
}

void TimeSetLightFlag(u16 cmd) {
    time_light_flag = cmd;
}
/*
water---------------------
*/
static u16 time_water_flag = 1;

u16 TimeGetWaterFlag(void) {
    return time_water_flag;
}

void TimeSetWaterFlag(u16 cmd) {
    time_water_flag = cmd;
}
/*
nutrition---------------------
*/
static u16 time_nutrition_flag = 0;

u16 TimeGetNutritionFlag(void) {
    return time_nutrition_flag;
}

void TimeSetNutritionFlag(u8 cmd) {
    time_nutrition_flag = cmd;
}
/*
get day
*/
u8 TimeGetDay(void) {
    return time_day;
}
void TimeSetDay(u8 cmd) {
    time_day = cmd;
    EepromWrite(24,time_day);
    time_sec = cmd;
    EepromWrite(21,time_sec);
    time_min = cmd;
    EepromWrite(22,time_min);
    time_hou = cmd;
    EepromWrite(23,time_hou);
}
#pragma vector=0x19
__interrupt void TIM4_UPD_OVF_IRQHandler(void)
{
    static u16 count_time = 0;
    
    TIM4_SR = 0x00;
    
    if(count_time < 500) {
        count_time++;
    } else {
        count_time = 0;
        time_sec_flag = 0x80;
        if(time_light_flag > 0) {
            time_light_flag++;
        }
        if(time_water_flag > 0) {
            time_water_flag++;
        }
        if(time_nutrition_flag > 0) {
            time_nutrition_flag++;
        }
        if(time_sec < 60) {
            time_sec++;
            EepromWrite(21,time_sec);
        } else {
            time_sec = 0;
            if(time_min < 60) {
                time_min++;
                EepromWrite(22,time_min);
            } else {
                time_min = 0;
                if(time_hou < 24) {
                    time_hou++;
                    EepromWrite(23,time_hou);
                } else {
                    time_hou = 0;
                    if(time_day < 15) {
                        time_day++;
                        EepromWrite(24,time_day);
                    } else {
                        //tow week arrived
                    }
                }
            }
        }
    }
    return;
}