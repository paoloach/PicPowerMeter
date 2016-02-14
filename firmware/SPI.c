
#include <xc.h>
#include "SPI.h"

void initSPI(void) {
    LATB4 = 1;
    TRISB4 = 0;
    TRISC7 = 0;
    TRISB1 = 0;
    SSPCON1bits.SSPM = 1;
    SSPSTATbits.CKE = 0;
    SSPCON1bits.CKP = 0;
    LATB4 = 1;
}

uint8_t SPIWriteByte1(uint8_t data) {
    LATB4 = 0;
    SSPBUF = data;
    while (!SSPSTATbits.BF);

    data = SSPBUF;
    LATB4 = 1;
    return data;
}

uint8_t SPIWriteByte(uint8_t reg) {
    uint8_t i;
    uint8_t byte1 = 0;
    SSPCON1bits.SSPEN = 0;
    PORTB = 0;
    LATB = 0;
    ADCON1 = 0x0E;
    LATB4 = 1;
    TRISB4 = 0;
    TRISC7 = 0;
    TRISB1 = 0;
    LATB1 = 1;
    LATB4 = 0;
    for (i = 0; i < 8; i++) {
        LATB1 = 0;
        if (reg & 0x80)
            LATC7 = 1;
        else
            LATC7 = 0;
        byte1 = byte1 << 1;
        if (PORTBbits.RB0)
            byte1 |= 1;

        LATB1 = 1;
        reg = reg << 1;
    }
    LATB4 = 1;
    return byte1;
}

uint24_t readRegister(uint8_t reg) {
    uint8_t i;
    uint8_t byte1 = 0, byte2 = 0, byte3 = 0;
    SSPCON1bits.SSPEN = 0;
    PORTB = 0;
    LATB = 0;
    ADCON1 = 0x0E;
    LATB4 = 1;
    TRISB4 = 0;
    TRISC7 = 0;
    TRISB1 = 0;
    LATB1 = 1;
    reg = reg << 1;
    LATB4 = 0;
    for (i = 0; i < 8; i++) {
        LATB1 = 0;
        if (reg & 0x80)
            LATC7 = 1;
        else
            LATC7 = 0;
        LATB1 = 1;
        reg = reg << 1;
    }
    LATC7 = 1;
    for (i = 0; i < 8; i++) {
        LATB1 = 0;
        byte1 = byte1 << 1;
        if (PORTBbits.RB0)
            byte1 |= 1;
        LATB1 = 1;

    }
    for (i = 0; i < 8; i++) {
        LATB1 = 0;
        byte2 = byte2 << 1;
        if (PORTBbits.RB0)
            byte2 |= 1;
        LATB1 = 1;

    }
    for (i = 0; i < 8; i++) {
        LATB1 = 0;
        byte3 = byte3 << 1;
        if (PORTBbits.RB0)
            byte3 |= 1;
        LATB1 = 1;

    }

    LATB4 = 1;
    SSPCON1bits.SSPEN = 1;
    return ((uint24_t) byte1 << 16) + (((uint24_t) byte2) << 8) + byte3;
}

void writeRegister(uint8_t reg, uint24_t dataToWrite) {
    uint8_t i;
    SSPCON1bits.SSPEN = 0;
    PORTB = 0;
    LATB = 0;
    ADCON1 = 0x0E;
    LATB4 = 1;
    TRISB4 = 0;
    TRISC7 = 0;
    TRISB1 = 0;
    LATB1 = 1;
    reg = (reg << 1) | 0x40;
    LATB4 = 0;
    for (i = 0; i < 8; i++) {
        LATB1 = 0;
        if (reg & 0x80)
            LATC7 = 1;
        else
            LATC7 = 0;
        LATB1 = 1;
        reg = reg << 1;
    }
    for (i = 0; i < 24; i++) {
        LATB1 = 0;
        if (dataToWrite & 0x800000)
            LATC7 = 1;
        else
            LATC7 = 0;
        LATB1 = 1;
        dataToWrite = dataToWrite << 1;
    }
    LATB4 = 1;
}

void startCSConversion() {
    SPIWriteByte(0xE8);
}

void resetCS() {
    SPIWriteByte(0x80);
}
