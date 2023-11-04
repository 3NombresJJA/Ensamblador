#include <xc.h>
#include "Salida.h"
#include "fuses.h"

void LED_INIT (void){
    PortE= 0;
    ANSEL =0;
}
void LED_COLOR_ON(unsigned char red,unsigned char green, unsigned char blue){
    ROJO = red;
    VERDE= green;
    AZUL = blue;
}