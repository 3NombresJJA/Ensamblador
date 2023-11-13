/**
\file   LCD.c
\date   2023-09-22
\author José Parra, juan camilo fernandez, juan jose angel <jrparra@unicauca.edu.co>
\brief  LCD 16x2 mode 4 bits.

\par Copyright
Information contained herein is proprietary to and constitutes valuable
confidential trade secrets of unicauca, and
is subject to restrictions on use and disclosure.

\par
Copyright (c) unicauca 2023. All rights reserved.

\par
The copyright notices above do not evidence any actual or
intended publication of this material.
******************************************************************************
*/


#include <xc.h>
#include "ADC.h"
#include "fuses.h"
void ADC_init(void){
    ADCON1bits.ADFM = 1; //  Justificación derecha (modo-8bits).
    ADCON1bits.VCFG0 = 0; //  Selecciona Voltajes de Referencia (5v-0v).
    ADCON1bits.VCFG1 = 0; //  Selecciona Voltajes de Referencia (5v-0v). 
    ADCON0bits.ADCS = 0b01; //  Tiempo de Conversión Fosc/8.
} 
/**
 * @brief Realiza una conversión ADC en el canal especificado.
 *
 * @param channel Número del canal analógico.
 * @return Valor digital resultante de la conversión.
 */
int ConversionADC (int channel){
    ADCON0bits.CHS = (0x0f & channel); //  Selecciona el Canal Analógico AN0.
     ADCON0bits.ADON = 1; //  Habilita el Módulo AD.
     __delay_us(30);
     ADCON0bits.GO = 1; //  Inicia la COnversión AD.
     while (ADCON0bits.GO); //  Espera a que termine la conversión AD.
     ADCON0bits.ADON = 0; //  Habilita el Módulo AD.
     return ((ADRESH << 8) | ADRESL);
    
    return  ((ADRESH<<8) | ADRESL);
}

