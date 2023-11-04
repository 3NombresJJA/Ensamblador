/* 
 * File:   Timer0_1ms.h
 * Author: Compumax
 *
 * Created on 3 de noviembre de 2023, 09:36 PM
 */

#ifndef TIMER0_1MS_H
#define	TIMER0_1MS_H

#ifdef	__cplusplus
extern "C" {
#endif


void Timer1_start();
void __interrupt() Timer1_ISR();

#ifdef	__cplusplus
}
#endif

#endif	/* TIMER0_1MS_H */

