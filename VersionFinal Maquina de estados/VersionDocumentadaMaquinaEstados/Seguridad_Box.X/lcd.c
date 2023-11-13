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
#include "lcd.h"
#include "fuses.h"

/**
 * @brief Inicializa la pantalla LCD 16x2.
 *
 * Esta función inicializa la pantalla LCD 16x2 con configuraciones específicas.
 */
void LCD_Init(void)
{
    LCD_Port = 0;         /* PORT como puerto de salida */
    __delay_ms(15);       /* Retardo de encendido de 15 ms */
    LCD_Command(0x02);    /* Envía inicialización de LCD con método de nibble */
    LCD_Command(0x28);    /* Utiliza 2 líneas e inicializa matriz 5x7 en (modo de 4 bits) */
    LCD_Command(0x01);    /* Limpia la pantalla */
    LCD_Command(0x0c);    /* Muestra con cursor apagado */
    LCD_Command(0x06);    /* Incrementa el cursor (mueve el cursor a la derecha) */	
}

/**
 * @brief Envía un comando a la pantalla LCD 16x2.
 *
 * @param cmd Comando a ejecutar en la pantalla LCD (ejemplo: 0x01 para limpiar la pantalla, 0x0C para mostrar con cursor apagado).
 */
void LCD_Command(unsigned char cmd )
{
	ldata = (ldata & 0x0f) |(0xF0 & cmd);  /* Envía el nibble superior del comando primero a PORT */ 
	RS = 0;  /* Se selecciona el Registro de Comandos (RS=0) */ 
	EN = 1;  /* Pulso de alto a bajo en el pin Enable para bloquear los datos */
	NOP();
	EN = 0;
	__delay_ms(1);
    ldata = (unsigned char)((ldata & 0x0f) | (cmd<<4));  /* Envía el nibble inferior del comando a PORT */
	EN = 1;
	NOP();
	EN = 0;
	__delay_ms(3);
}

/**
 * @brief Muestra un carácter en la pantalla LCD 16x2.
 *
 * @param dat Carácter a mostrar en la pantalla LCD.
 */

void LCD_Char(unsigned char dat)
{
	ldata = (ldata & 0x0f) | (0xF0 & dat);  /* Envía el nibble superior de los datos primero a PORT */
	RS = 1;  /* Se selecciona el Registro de Datos (RS=1) */
	EN = 1;   /* Pulso de alto a bajo en el pin Enable para bloquear los datos */
	NOP();
	EN = 0;
	__delay_ms(1);
    ldata = (unsigned char)((ldata & 0x0f) | (dat<<4));  /* Envía el nibble inferior de los datos a PORT */
	EN = 1;  
	NOP();
	EN = 0;
	__delay_ms(3);
}
/**
 * @brief Muestra una cadena en la pantalla LCD 16x2.
 *
 * @param msg Cadena a mostrar en la pantalla LCD.
 */

void LCD_String(const char *msg)
{
	while((*msg)!=0)
	{		
	  LCD_Char(*msg);
	  msg++;	
    }
}
/**
 * @brief Muestra una cadena en una posición específica de la pantalla LCD 16x2.
 *
 * @param row Fila en la que se mostrará el mensaje (0 o 1).
 * @param pos Posición en la que se mostrará el mensaje (0-15).
 * @param msg Cadena a mostrar en la pantalla LCD.
 */

void LCD_String_xy(char row,char pos,const char *msg)
{
    char location=0;
    if(row<=1)
    {
        location=(0x80) | ((pos) & 0x0f);  /* Imprime el mensaje en la primera fila y la ubicación deseada */
        LCD_Command(location);
    }
    else
    {
        location=(0xC0) | ((pos) & 0x0f);  /* Imprime el mensaje en la segunda fila y la ubicación deseada */
        LCD_Command(location);    
    }  
    

    LCD_String(msg);

}

/**
 * @brief Limpia la pantalla de la LCD 16x2.
 *
 * Esta función limpia la pantalla de la LCD 16x2.
 */
void LCD_Clear(void)
{
   	LCD_Command(0x01);
    __delay_ms(3);
}