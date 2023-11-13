/* 
 * File:   Salida.h
 * Author: José Parra <jrparra@unicauca.edu.co>
 *
 * Created on 28 de septiembre de 2023, 09:57 PM
 */

#ifndef SALIDA_H
#define	SALIDA_H

#ifdef	__cplusplus
extern "C" {
#endif

#define PortE TRISE
#define LED PORTE
#define ROJO   RE0
#define VERDE  RE1
#define AZUL   RE2
    


void LED_INIT(void);
void LED_COLOR_ON(unsigned char red,unsigned char green, unsigned char blue);


#ifdef	__cplusplus
}
#endif

#endif	/* SALIDA_H */

