#include <xc.h>
#include "ADC.h"
#include "fuses.h"
void ADC_init(void){
    ADCON1bits.ADFM = 1; //  Justificaci�n derecha (modo-8bits).
    ADCON1bits.VCFG0 = 0; //  Selecciona Voltajes de Referencia (5v-0v).
    ADCON1bits.VCFG1 = 0; //  Selecciona Voltajes de Referencia (5v-0v). 
    ADCON0bits.ADCS = 0b01; //  Tiempo de Conversi�n Fosc/8.
} 
int ConversionADC (int channel){
    ADCON0bits.CHS = (0x0f & channel); //  Selecciona el Canal Anal�gico AN0.
     ADCON0bits.ADON = 1; //  Habilita el M�dulo AD.
     __delay_us(30);
     ADCON0bits.GO = 1; //  Inicia la COnversi�n AD.
     while (ADCON0bits.GO); //  Espera a que termine la conversi�n AD.
     ADCON0bits.ADON = 0; //  Habilita el M�dulo AD.
     return ((ADRESH << 8) | ADRESL);
    
    return  ((ADRESH<<8) | ADRESL);
}

