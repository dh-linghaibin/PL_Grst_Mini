/*******************************************************************************
* Copyright 2016 HangZhou 杭州久天科技有限公司
* All right reserved
*
* 文件名称：Menu.c
*
* 文件标识：
* 摘    要：
*
*
* 当前版本：
* 作    者：lhb Steven
* 完成日期：2016/4/15
* 编译环境：C:\Program Files (x86)\IAR Systems\Embedded Workbench 6.5\stm8        
*
* 历史信息：
*******************************************************************************/
#include "Menu.h"
#include "Led.h"
#include "Moto.h"
#include "time.h"
typedef struct DataNode
{
    u8 mode;//模式
    u8 light;//灯是否可以亮
    u16 light_work_timr;//灯的工作时间
    u16 light_stop_time;//灯的停止时间
    u16 water_work_time;//水泵工作时间
    u16 water_stop_time;//水泵停止时间
} tDataNode;
/***********************************************变量声明*****************************************************
* 功    能: caidan  
* 作    者: by lhb_steven
* 日    期: 2016/4/15
************************************************************************************************************/ 
static tDataNode menu;
/**********************************************函数定义***************************************************** 
* 函数名称: void MenuInit(void) 
* 输入参数: void 
* 返回参数: void  
* 功    能: csh  
* 作    者: by lhb_steven
* 日    期: 2016/4/15
************************************************************************************************************/ 
void MenuInit(void) { 
    menu.light_work_timr = 3600;
    menu.light_stop_time = 3600;
    menu.water_work_time = 60;
    menu.water_stop_time = 7200;
}
/**********************************************函数定义***************************************************** 
* 函数名称: void MenuSet(void) 
* 输入参数: void 
* 返回参数: void  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/4/15
************************************************************************************************************/ 
void MenuSet(u8 cmd) { 
    switch( cmd ) {
    case 0:
        break;
    case 0x01:
        if(menu.mode == 0) {
            menu.mode = 1;
            LedSet(0);
        } else {
            menu.mode = 0;
            //关闭一切
            LedSet(1);
            MotoTempSet(0);
            MotoWaterSet(0);
        }
        break;
    case 0x02:
        if(menu.mode == 1) {
            if(menu.light == 0) {//灯可以亮
                menu.light = 1;
                MotoTempSet(1);
                TimeSetSecFlag(0);
            } else {
                menu.light = 0;
                MotoTempSet(0);
            }
        } else {
            
        }
        break;
    case 0x03:
        break;
    default:
        break;
    }
}
/**********************************************函数定义***************************************************** 
* 函数名称: void MenuFlash(void) 
* 输入参数: void 
* 返回参数: void  
* 功    能: pd  
* 作    者: by lhb_steven
* 日    期: 2016/4/15
************************************************************************************************************/ 
void MenuFlash(void) { 
    if(TimeGetSecFlag() == 0x80) {
        TimeSetSecFlag(0);
        if(menu.mode == 1) {
            if(menu.light == 1) {
                if(TimeGetLightFlag() < menu.light_work_timr ) {//补光LED灯（大灯）
                    //open light 
                    MotoTempSet(1);
                } else if(TimeGetLightFlag() < (menu.light_work_timr+menu.light_stop_time)) {
                    //cloose
                    MotoTempSet(0);
                } else {
                    TimeSetLightFlag(1);//clear
                }
            } else {
                MotoTempSet(0);
            }
            //水泵
            if(TimeGetWaterFlag() < menu.water_work_time ) {//补光LED灯（大灯）
                //open light 
                MotoWaterSet(1);
            } else if(TimeGetWaterFlag() < (menu.water_work_time+menu.water_stop_time)) {
                //cloose
                MotoWaterSet(0);
            } else {
                TimeSetWaterFlag(1);//clear
            }
        } else {
            TimeSetLightFlag(1);//clear
            TimeSetWaterFlag(1);//clear
        }
    }
}








