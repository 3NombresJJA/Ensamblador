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
#include <pic16f887.h>  /**< Archivo de definiciones de PIC16f887 */
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "fuses.h"

#include "lcd.h"
#include "keypad.h"
#include "Salida.h"
#include "ADC.h"
#include "Procesos.h"
#define SENSOR_IR RC1
#define SENSOR_HALL RC0
#define SENSOR_METAL RC2
/** Enumeración para los posibles estados del sistema. */
enum State
{
    seguridad,
    monitoreo,
    bloqueado,
    alarma,
    ambiente
};
enum State Estado;

const char password[6] ={'2','0','2','3','9'};
char pass_user[6];
unsigned char idx = 0;
unsigned char idx2 = 0;
char buffer [17];
char buffer2 [17];

unsigned char var_sensor_ir = 0;
unsigned char var_sensor_hall =0;
unsigned char var_sensor_metal =0;
int var_sensor_Temp = 0;
int var_sensor_Photo =0;
int var_sensor_Gas =0;
unsigned char intentos = 0;
unsigned char intent=0;
unsigned char intent2=0;
/**
 * @brief Función principal del programa.
 */

void main() {
    /**Configura Fosc = 8Mhz interno, Fuente de Fosc del sistema = interno*/
    OSCCON = 0x71; /**< Configura oscilador interno (FOSC = 8Mhz) */
    TRISC =0x07;
    TRISD = 0x00;
    TRISA = 0x23;
    ANSEL = 0x13;
    
    LED_INIT();

    LCD_Init(); /**< Inicializa el LCD */
    
    keypad_init(); /**< Inicializa el keypad */
    ADC_init();/**< Inicializa el ADC */
    
    
    var_sensor_ir= SENSOR_IR;
    var_sensor_hall= SENSOR_HALL;
    var_sensor_metal= SENSOR_METAL;
    /*
    if (SENSOR_IR == 1){
                LCD_String_xy(0,0,"sIr=ON");
            }
    if(SENSOR_HALL == 1){
                LCD_String_xy(0,6,"sHALL=ON");
            }
    if(SENSOR_METAL ==1){
                LCD_String_xy(0,14,"sMETAL=ON");
            }
  */
    LCD_String_xy(0,0,"Intento 15");
    __delay_ms(2000);
    LCD_Clear();
    Estado = seguridad;
    while(1){
        if(Estado == seguridad){
            function_clave_Seguridad();
        }
        else if(Estado == monitoreo){
            function_eMonitoreo();                  
        }
        else if(Estado == bloqueado){
            function_eBloqueado();                  
        }
        else if( Estado == alarma){
            function_eAlarma();
        }
        else if( Estado == ambiente){
            function_ambiental();
        }
        else{
            function_clave_Seguridad();
        }
    }
}
/**
 * @brief Función para el manejo de la clave de seguridad.
 */
void function_clave_Seguridad(void){
    idx=0;
    LED_COLOR_ON(0,0,0);
    char key = '0';
    if ( intentos < 3) {
        LCD_Clear();
        LCD_String_xy(0,0,"digite clave");
        LCD_Command(0xC0); 
        do{
            key = keypad_getkey();
            if(key != 0){
                LCD_Char('*'); /**Muestra el asterisco en el LCD16x2 cuando se presiona una tecla*/
                pass_user[idx++] = key;
            }
            __delay_ms(100);
        }while(idx < 5);
        
        if(strncmp(pass_user,password,4)==0){
            LCD_Clear();
            LCD_String_xy(0,0,"Clave Correcta");
            LED_COLOR_ON(0,1,0);
            intentos = 0;
            __delay_ms(2000);
            LED_COLOR_ON(0,0,0);
            Estado=monitoreo;
        }
        else{
            LCD_Clear();
            LCD_String_xy(0,0,"Clave Incorrecta");
            LED_COLOR_ON(0,0,1);
            intentos++;
            __delay_ms(800);
            
        }       
    } 
    else {
        Estado= bloqueado;
    } 
 }
/**
 * @brief Función para el manejo del estado bloqueado.
 */
void function_eBloqueado(void) {
    LCD_Clear();
    LCD_String_xy(0,0,"Sis Bloqueado");
    LED_COLOR_ON(1,0,0);
    __delay_ms(5000);
    LCD_Clear();
    intentos=0;
    Estado = seguridad;
    
}
/**
 * @brief Función para el manejo del estado de alarma.
 */
void function_eAlarma(void) {
    LCD_Clear();
    LCD_String_xy(0,0,"Alerta Intruso");
    for ( int i = 0; i<5; i++){
        LED_COLOR_ON(1,0,0);
        __delay_ms(500);
        LED_COLOR_ON(0,0,0);
        __delay_ms(500);
    }
    
    LCD_Clear();
    Estado = monitoreo;
}
/**
 * @brief Función para el manejo del estado de monitoreo.
 */
void function_eMonitoreo(void) {
    /*LCD_Clear();
    int kj = sprintf(buffer2, "intent: %d", intent);
    LCD_String_xy(0,0,buffer2);
    */
    
    if(intent <3){
            LCD_Clear();
            var_sensor_ir= SENSOR_IR;
            var_sensor_hall= SENSOR_HALL;
            var_sensor_metal= SENSOR_METAL;
            int n= sprintf (buffer, "ir=%d hl=%d mt=%d", var_sensor_ir, var_sensor_hall, var_sensor_metal);
            LCD_String_xy(0,0,buffer);
        
            if( /*var_sensor_ir==1 || var_sensor_hall==1 || var_sensor_metal ==1*/ 0){
                
                intent = 0;
                __delay_ms(10);
                Estado=alarma;
            }
            else{
                intent++;
                Estado = monitoreo;
                __delay_ms(1000);
            }
    } else {
        intent = 0;
        Estado = ambiente;
        
    }
}
        
/**
 * @brief Función para el manejo del estado ambiente.
 */

void function_ambiental(void){
    if(intent2 <3){
        var_sensor_Temp= ConversionADC(0);
        __delay_ms(30);
        long a =1023 - var_sensor_Temp;
        int tempC = ((beta /(log((1025.0 * 10 / a - 10) / 10) + beta / 298.0) - 273.0)+36);
        /** Lectura photo*/
        var_sensor_Photo = ConversionADC(1);
         __delay_ms(30);
        
        /** Lectura Humo*/
        var_sensor_Gas = ConversionADC(4);
         __delay_ms(30);
        
        LCD_Clear();
   
        int n1= sprintf (buffer, "T=%d Ph=%d Fl= %d", tempC , var_sensor_Photo,var_sensor_Gas);
        //int n2= sprintf (buffer2, "Gs=%d", var_sensor_Gas);
        LCD_String_xy(0,0,buffer);
        //LCD_String_xy(1,0,buffer2);
        if( tempC >30 ){
                intent2 = 0;
                __delay_ms(10);
                Estado=alarma;
            }
            else{
                intent2++;
                Estado = ambiente;
                __delay_ms(1000);
            }
    } else {
        intent2 = 0;
        Estado = monitoreo;
        
    }
}   
