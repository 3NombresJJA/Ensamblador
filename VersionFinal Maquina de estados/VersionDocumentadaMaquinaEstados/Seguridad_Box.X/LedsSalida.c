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
#include "Salida.h"
#include "fuses.h"

void LED_INIT (void){
    PortE= 0;
}
/**
 * @brief Enciende el LED RGB con el color especificado.
 *
 * @param red Valor para el componente rojo del LED (0-255).
 * @param green Valor para el componente verde del LED (0-255).
 * @param blue Valor para el componente azul del LED (0-255).
 */
void LED_COLOR_ON(unsigned char red,unsigned char green, unsigned char blue){
    ROJO = red;
    VERDE= green;
    AZUL = blue;
}