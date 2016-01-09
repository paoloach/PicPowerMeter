
#include <xc.h>
#include "SPI.h"

void initSPI(void){
    LATB4=1;
    TRISB4=0;
    TRISC7=0;
    TRISB1=0;
    SSPCON1bits.SSPM=1;
    SSPSTATbits.CKE=0;
    SSPCON1bits.CKP=0;
    SSPCON1bits.SSPEN=1;
}

uint8_t writeByte(uint8_t){
    while(SSPSTATbits.BF);
}
