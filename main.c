
#include "Sys.h"
#include "Botton.h"
#include "Led.h"
#include "Moto.h"
#include "Menu.h"
#include "Time.h"
#include "Eeprom.h"

int main( void ) {
    SysInit();
    LedInit();
    MotoInit();
    EeepromInit();
    ButtonInit();
    MenuInit();
    TimeInit();
    INTEN
    while(1) {
        MenuSet(ButtonRead());
        MenuFlash();
    }
}
