#include <stdint.h>
#include "LCD.h"

#include <xc.h>         /* XC8 General Include File */

static void waitForLCDStart(void);
static void waitFor30ms(void);
static void waitFor39us(void);
static void waitFor2ms(void);
static void writeCmdLCD(uint8_t data);
static void waitNotBusy(void);

#define RS LATEbits.LE0
#define RW LATEbits.LE1
#define E  LATEbits.LE2

void initLCD(void){
        /* Enable interrupts */
    TRISD=0;
    TRISE0 = 0;
    TRISE1 = 0;
    TRISE2 = 0;
    RS=0;
    RW=0;
    E=0;
    waitForLCDStart();
    line1();
    writeLCD("line1");
    line2();
    writeLCD("line2");
}

void line1(void) {
    writeCmdLCD(0x80);
}

void line2(void) {
    writeCmdLCD(0xC0);
}

void writeLCD(const char * str){
    while(*str != 0){
        writeDataLCD(*str);
        str++;
    }
}

void writeLCDC(const char * str, unsigned char len){
    for (int i=0; i < len; i++){
        char data  = *str;
        if (data < 32){
            data = ' ';
        }
        writeDataLCD(data);
        str++;
    }
}



static void waitForLCDStart(void){
    waitFor30ms();
    writeCmdLCD(0x38);
    waitFor39us();
    writeCmdLCD(0x0F);
    waitFor39us();
    writeCmdLCD(0x01);
    waitFor2ms();
    writeCmdLCD(0x06);
}

static void waitFor30ms(void) {
    __delay_ms(30);
}

static void waitFor2ms(void){
    __delay_ms(2);
}

static void waitFor39us(void) {
    __delay_us(39);
}

void writeDataLCD(char data){
    waitNotBusy();
    RS=1;
    RW=0;
    E=1;
    PORTD=data;
    E=0;
}


static void writeCmdLCD(uint8_t data){
    waitNotBusy();
    RS=0;
    RW=0;
    E=1;
    PORTD=data;
    E=0;
}

static void waitNotBusy(void){
    TRISD=0xFF;
    uint8_t res=0;
    RS=0;
    RW=1;
    do {
        E=1;
        _delay(2);
        if (PORTDbits.RD7 == 0){
            res=1;
        }
        E=0;
    } while (res == 0);
    TRISD=0;
    return;
}