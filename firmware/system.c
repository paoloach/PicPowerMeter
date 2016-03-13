/******************************************************************************/
/*Files to Include                                                            */
/******************************************************************************/


#include <p18cxxx.h>    /* C18 General Include File */


#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#include "system.h"

extern uint32_t totalSeconds;
extern uint8_t newMeasure;

#define TMR0_LOAD_L 0xE6
#define TMR0_LOAD_H 0x48

/* 
 * For PIC18 devices the high interrupt vector is found at
 * 00000008h. The following code will branch to the
 * high_interrupt_service_routine function to handle
 * interrupts that occur at the high vector.
 */
void interrupt high_priority high_isr(void) {
    if (INTCONbits.T0IF) {
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        TMR0H = TMR0_LOAD_H;
        TMR0L = TMR0_LOAD_L;
        totalSeconds++;
        newMeasure = 1;
        INTCONbits.T0IF = 0;
    }
    if (INTCON3bits.INT2IF) {
        totalSeconds++;
        newMeasure = 1;
        INTCON3bits.INT2F = 0;
    }
}
