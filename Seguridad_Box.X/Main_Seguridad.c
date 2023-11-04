/*!
\file mainport.c
\date 2023-xx-xx
\author no conocido <jrparra@unicauca.edu.co>
\brief Example test MCU PIC16F887.
\par Copyright
Information contained herein is proprietary to and constitutes valuable confidential trade secrets of unicauca, and
is subject to restrictions on use and disclosure.
\par
Copyright (c) unicauca 2023. All rights reserved.
\par
The copyright notices above do not evidence any actual or intended publication of this material.
*/
#include <xc.h>
#include <pic16f887.h>  /*Header file PIC16f887 definitions*/
#include <stdio.h>
#include <string.h>
#include "fuses.h"
#include "lcd.h"
#include "keypad.h"
#include "Salida.h"

const char password[6] ={'2','0','2','3','9'};
char pass_user[6];
unsigned char idx = 0;



void main() {

    char key = '0';
    //Configura Fosc = 8Mhz interno, Fuente de Fosc del sistema = interno
    OSCCON = 0x71; //Configura oscilador interno (FOSC = 8Mhz)
   
    LED_INIT();

    LCD_Init(); //Inicializa el LCD
    
    keypad_init(); //Inicializa el keypad
    
    
    
    unsigned char k = 0;
    
    while (1) {
    
        if ( k<3) {    
            LED_COLOR_ON(0,0,0);
            LCD_String_xy(0,0,"Press a Key");
            LCD_Command(0xC0); 
            do{
                key = keypad_getkey();
                if(key != 0){
                    LCD_Char('*');/* display pressed key on LCD16x2 */
                    //LCD_Char(key);/* display pressed key on LCD16x2 */
                    pass_user[idx++] = key;
                }
                __delay_ms(100);
            }while(idx < 5);
        
            if(strncmp(pass_user,password,4)==0){
                LCD_Clear();
                LCD_String_xy(0,0,"Clave Correcta");
                LED_COLOR_ON(0,1,0);
                k = 0;
            }
            else{
                LCD_Clear();
                LCD_String_xy(0,0,"Clave Incorrecta");
                LED_COLOR_ON(0,0,1);
                k=k+1;
            }
            __delay_ms(2000);
            LCD_Clear();
            idx = 0;
        } 
        else {
        LCD_Clear();
        LCD_String_xy(0,0,"Alerta Intruso");
        LED_COLOR_ON(1,0,0);
        __delay_ms(8000);
        k=0;
        LCD_Clear();
        }
        
    }
}
