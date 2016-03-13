

#include <xc.h>
#include <stdint.h>
#include "KS0108.h"

#define DI LATEbits.LE0
#define RW LATEbits.LE1
#define E  LATEbits.LE2
#define CS1 LATCbits.LC0
#define CS2 LATCbits.LC1
#define D LATD

extern const uint8_t font[][7];

static uint8_t getStatus();
static void setCol(uint8_t x);
static void setRow(uint8_t y);
static void write(uint8_t data, uint8_t cs, uint8_t cmd);
static void setZ(uint8_t z);
static uint8_t cursorX;
static uint8_t cursorY;
static uint8_t cs;
#define ON 0
#define OFF 1

#define wait3us asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");

#define wait5us asm("nop");\
                asm("nop");\
                asm("nop");\
                asm("nop");\
                asm("nop");\
                asm("nop");\
                asm("nop");\
                asm("nop");\
                asm("nop");\
                asm("nop");\
                asm("nop");\
                asm("nop");\
                asm("nop");\
                asm("nop");\
                asm("nop");\
                asm("nop");\
                asm("nop");

static void enablePulse(void) {
    E = 1;
    wait5us;
    E = 0;
    wait5us;
}

static void on() {
    CS1 = ON;
    CS2 = ON;
    DI = 0;
    RW = 0;
    D = 0x3F;
    enablePulse();
}

static void clearLine(uint8_t line) {
    CS1 = ON;
    CS2 = ON;
    DI = 0;
    RW = 0;
    D = 0x40;
    wait5us;
    enablePulse();
    setRow(line);
    for (uint8_t i = 0; i < 64; i++) {
        DI = 1;
        RW = 0;
        D = 0;
        wait3us;
        enablePulse();
    }
}

void initKS0108() {
    TRISE0 = 0;
    TRISE1 = 0;
    TRISE2 = 0;
    TRISC0 = 0;
    TRISC1 = 0;
    TRISD = 0;
    on();
    setZ(0);
    //    CS1=0;
    //    CS2=1;
    //    uint8_t status = getStatus();
    //    while (status & 0x80){
    //        status = getStatus();
    //    }
    //    CS1=1;
    //    CS2=0;
    //    status = getStatus();
    //    while (status & 0x80){
    //        status = getStatus();
    //    }
    for (uint8_t i = 0; i < 8; i++) {
        clearLine(i);
    }
}

static void setCol(uint8_t x) {
    uint8_t data;
    DI = 0;
    RW = 0;
    if (x < 64) {
        CS1 = ON;
        CS2 = OFF;
        cs = 0;
        data = x;
    } else {
        CS2 = ON;
        CS1 = OFF;
        data = x - 64;
        cs = 1;
    }
    cursorX = x;
    data = (data | 0x40) & 0x7F;
    D = data;
    enablePulse();
}

static void setRow(uint8_t y) {
    CS1 = ON;
    CS2 = ON;
    uint8_t data;
    DI = 0;
    RW = 0;
    data = (y | 0xb8) & 0xbf;
    D = data;
    enablePulse();
}

static void setZ(uint8_t z) {
    CS1 = ON;
    CS2 = ON;
    uint8_t data;
    DI = 0;
    RW = 0;
    data = (z & 0x3F) | 0xC0;
    D = data;
    enablePulse();
}

void setPos(uint8_t x, uint8_t y) {
    setRow(y);
    setCol(x);
}

void writeString(const char *str) {
    char c = *str;
    while (c != 0) {
        writeChar(c);
        str++;
        c = *str;
    }
}

void writeChar(char c) {
    const uint8_t * bitmap = font[c];
    uint8_t line = *bitmap;
    while (line != 0xFF) {
        if (cursorX < 64) {
            if (cs == 1) {
                cs = 0;
                setCol(cursorX);
            }
        } else {
            if (cs == 0) {
                cs = 1;
                setCol(cursorX);
            }
        }
        DI = 1;
        RW = 0;
        D = line;
        wait3us;
        enablePulse();
        cursorX++;
        bitmap++;
        line = *bitmap;
    }
}

void verticalLine(uint8_t x, uint8_t h) {
    uint8_t piece = 0;
    for (uint8_t y = 7; y > 3; y--) {
        setCol(x);
        setRow(y);
        if (h > 8) {
            D = 0xFF;
            h -= 8;
        } else if (h > 0) {
            while (h) {
                piece = (piece >> 1) | 0x80;
                h--;
            }
            D=piece;
        } else {
            D = 0;
        }
        if (x < 64) {
            CS1 = ON;
            CS2 = OFF;
        } else {
            CS2 = ON;
            CS1 = OFF;
        }
        DI = 1;
        RW = 0;
        wait3us;
        enablePulse();
    }
}

static uint8_t getStatus() {
    di();
    TRISD = 0xFF;
    E = 0;
    RW = 1;
    DI = 0;
    wait5us
    E = 1;
    ei();
    wait5us
    return PORTD;
}

static void write(uint8_t data, uint8_t cs, uint8_t cmd) {
    di();
    RW = 1;
    DI = 0;
    wait3us
    E = 1;
    wait3us
    while (PORTDbits.RD7) {
        wait3us
        E = 0;
        wait3us
        E = 1;
    }
    E = 0;
    RW = 0;
    DI = cmd;
    if (cs == 0) {
        CS1 = 1;
        CS2 = 0;
    } else {
        CS1 = 0;
        CS2 = 1;
    }
    D = data;
    E = 1;
    wait3us
    TRISD = 0;
    wait3us
    E = 0;
    wait3us
    RW = 1;
    TRISD = 0xFF;
    ei();
}
