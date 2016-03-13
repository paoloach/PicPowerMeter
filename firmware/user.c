/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>    /* C18 General Include File */
#endif

#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#endif

#include "user.h"
#include "SPI.h"
#include "KS0108.h"
/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

#define TMR0_LOAD_L 0xE6
#define TMR0_LOAD_H 0x48

/* <Initialize variables in user.h and insert code for user algorithms.> */

void InitApp(void){
    initKS0108();
    initSPI();
    TRISA5=1;
    NOT_RBPU =0;
    TRISC=0xFC;
    INTCON2bits.INTEDG2=1;
    INTCON3bits.INT2IE=0;
    INTCON3bits.INT2IP=1;
    INTCON3bits.INT2F=0;
    
    TMR0L=TMR0_LOAD_L;
    TMR0H=TMR0_LOAD_H;
    INTCON2bits.T0IP=1;
    INTCONbits.TMR0IF=0;
    INTCONbits.TMR0IE=1;
    T0CON = 0x85;
}

