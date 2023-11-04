/* 
 * File:   ADC.h
 * Author: KADDEX
 *
 * Created on 18 de octubre de 2023, 10:37 AM
 */

#ifndef ADC_H
#define	ADC_H
#define beta 4090


#ifdef	__cplusplus
extern "C" {
#endif

int ConversionADC (int channel);
void ADC_init(void);


#ifdef	__cplusplus
}
#endif

#endif	/* ADC_H */

