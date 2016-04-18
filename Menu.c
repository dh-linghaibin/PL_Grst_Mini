/*******************************************************************************
* Copyright 2016 HangZhou ���ݾ���Ƽ����޹�˾
* All right reserved
*
* �ļ����ƣ�Menu.c
*
* �ļ���ʶ��
* ժ    Ҫ��
*
*
* ��ǰ�汾��
* ��    �ߣ�lhb Steven
* ������ڣ�2016/4/15
* ���뻷����C:\Program Files (x86)\IAR Systems\Embedded Workbench 6.5\stm8        
*
* ��ʷ��Ϣ��
*******************************************************************************/
#include "Menu.h"
#include "Led.h"
#include "Moto.h"
#include "time.h"
typedef struct DataNode
{
    u8 mode;//ģʽ
    u8 light;//���Ƿ������
    u16 light_work_timr;//�ƵĹ���ʱ��
    u16 light_stop_time;//�Ƶ�ֹͣʱ��
    u16 water_work_time;//ˮ�ù���ʱ��
    u16 water_stop_time;//ˮ��ֹͣʱ��
} tDataNode;
/***********************************************��������*****************************************************
* ��    ��: caidan  
* ��    ��: by lhb_steven
* ��    ��: 2016/4/15
************************************************************************************************************/ 
static tDataNode menu;
/**********************************************��������***************************************************** 
* ��������: void MenuInit(void) 
* �������: void 
* ���ز���: void  
* ��    ��: csh  
* ��    ��: by lhb_steven
* ��    ��: 2016/4/15
************************************************************************************************************/ 
void MenuInit(void) { 
    menu.light_work_timr = 3600;
    menu.light_stop_time = 3600;
    menu.water_work_time = 60;
    menu.water_stop_time = 7200;
}
/**********************************************��������***************************************************** 
* ��������: void MenuSet(void) 
* �������: void 
* ���ز���: void  
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/4/15
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
            //�ر�һ��
            LedSet(1);
            MotoTempSet(0);
            MotoWaterSet(0);
        }
        break;
    case 0x02:
        if(menu.mode == 1) {
            if(menu.light == 0) {//�ƿ�����
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
/**********************************************��������***************************************************** 
* ��������: void MenuFlash(void) 
* �������: void 
* ���ز���: void  
* ��    ��: pd  
* ��    ��: by lhb_steven
* ��    ��: 2016/4/15
************************************************************************************************************/ 
void MenuFlash(void) { 
    if(TimeGetSecFlag() == 0x80) {
        TimeSetSecFlag(0);
        if(menu.mode == 1) {
            if(menu.light == 1) {
                if(TimeGetLightFlag() < menu.light_work_timr ) {//����LED�ƣ���ƣ�
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
            //ˮ��
            if(TimeGetWaterFlag() < menu.water_work_time ) {//����LED�ƣ���ƣ�
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








