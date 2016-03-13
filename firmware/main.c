/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include <xc.h>        /* XC8 General Include File */
#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */
#include <string.h>
#include <stdio.h>

#include "KS0108.h"

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// CONFIG1L
#pragma config PLLDIV = 1       // PLL Prescaler Selection bits (No prescale (4 MHz oscillator input drives PLL directly))
#pragma config CPUDIV = OSC1_PLL2// System Clock Postscaler Selection bits ([Primary Oscillator Src: /1][96 MHz PLL Src: /2])
#pragma config USBDIV = 1       // USB Clock Selection bit (used in Full-Speed USB mode only; UCFG:FSEN = 1) (USB clock source comes directly from the primary oscillator block with no postscale)

// CONFIG1H
#pragma config FOSC = HS // Oscillator Selection bits (Internal oscillator, XT used by USB (INTXT))
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOR = ON         // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown-out Reset Voltage bits (Minimum setting 2.05V)
#pragma config VREGEN = OFF     // USB Voltage Regulator Enable bit (USB voltage regulator disabled)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = ON      // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = ON      // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = OFF     // Stack Full/Underflow Reset Enable bit (Stack full/underflow will not cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config ICPRT = OFF       // Dedicated In-Circuit Debug/Programming Port (ICPORT) Enable bit (ICPORT enabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) is not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) is not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) is not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) is not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) is not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM is not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) is not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) is not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) is not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) is not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) are not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) is not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM is not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) is not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) is not protected from table reads executed in other blocks)


#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp */
#include "SPI.h"

#define CONFIGURATION 0
#define CURRENT_DC_OFFSET 1
#define CURRENT_GAIN 2
#define VOLTAGE_DC_OFFSET  3
#define VOLTAGE_GAIN 4
#define CYCLE_COUNT 5
#define PULSE_RATE 6
#define INST_CURRENT 7
#define INST_VOLT   8
#define INST_POWER  9
#define ACTIVE_POWER   10
#define RMS_CURRENT 11
#define RMS_VOLT    12
#define EPSILON    13
#define POWER_OFFSET 14
#define STATUS_REG  15
#define CURRENT_AC_OFFSET 16
#define VOLTAGE_AC_OFFSET 17
#define MODE 18
#define TEMP 19
#define MEAN_REACTIVE_POWER 20
#define INST_REACTIVE_POWER 21
#define PEAK_CURRENT 22
#define PEAK_VOLTAGE 23
#define CALCULATE_REACTIVE_POWER 24
#define POWER_FACTOR 25
#define INTERRUPT_MASK 26
#define APPARENT_POWER 27
#define CONTROL 28

enum ShowStatus {
    ssConf = 0,
    ssCurrentDCOffset = 1,
    ssCurrentGain,
    ssVoltageOffset,
    ssVoltageGain,
    ssCycleCount,
    ssPulseRate,
    ssCurrent,
    ssVolt,
    ssPower,
    ssActivePower,
    ssRMSCurrent,
    ssRMSVoltage,
    ssEpsilon,
    ssPowerOffset,
    ssStatus,
    ssCurrentACOffset,
    ssVoltageAcOffset,
    ssMode,
    ssTemperature,
    ssAverageReactivePower,
    ssInstReactivePower,
    ssPeakCurrent,
    ssPeakVoltage,
    ssCalculatedReactivePower,
    ssPowerFactor,
    ssIntMask,
    ssApparemtPower,
    ssControl,
    ssLast

};

enum ShowStatus showStatus = ssActivePower;
uint32_t totalSeconds = 0;
uint8_t newMeasure;
double power;

uint8_t history[256];
uint8_t * iterSrc;
uint8_t * iterDst;
uint8_t * iterEnd = history + 256;
uint16_t historySize = 0;
uint32_t sampleGraph;
uint32_t measurePerSampleGraph = 1;
uint32_t counterMeasureGraph = 0;

#define MEASURE_FOR_HOUR    3600
#define MEASURE_FOR_SAMPLE    10
#define HISTORY_SIZE          MEASURE_FOR_HOUR / MEASURE_FOR_SAMPLE
uint16_t lastHourHistory[HISTORY_SIZE];
uint16_t indexHistory = 0;
uint16_t sample;
uint8_t sampleCount = 0;
uint32_t powerTot;
static char buffer[20];

union data_t {
    uint24_t lData;
    unsigned char bytes[3];
} data;

void calibration(void);
static void writeTime(void);
static void displayGraph(void);

double convertCurrent(uint24_t cur) {
    double tmp;
    if (cur & 0x800000) {
        cur = ~cur;
        cur++;
        tmp = -1;
    } else {
        tmp = 1;
    }
    tmp = tmp * cur;
    tmp = tmp / 0x7FFFFF;
    tmp = tmp * 0.25;
    return tmp;
}

double convertOffset(uint24_t offset) {
    double tmp = 1;
    if (offset & 0x800000) {
        offset = ~offset;
        offset++;
        tmp = -1;
    }
    double tmp = tmp * offset;
    tmp = tmp / 0x7FFFFF;
    return tmp;
}

double convertInstVolt(uint24_t volt) {
    double tmp;
    if (volt & 0x800000) {
        volt = ~volt;
        volt++;
        tmp = -1;
    } else {
        tmp = 1;
    }
    tmp = tmp * volt;
    tmp = tmp / 0x7FFFFF;
    tmp = tmp * 0.25;
    tmp = tmp * 2273;
    return tmp;
}

double convertRMSVolt(uint24_t volt) {
    double tmp = volt;
    tmp = tmp / 0xFFFFFF;
    tmp = tmp * 0.25;
    tmp = tmp * 2200;
    return tmp;
}

double convertRMSCurrent(uint24_t volt) {
    double tmp = volt;
    tmp = tmp / 0xFFFFFF;
    tmp = tmp * 0.25;
    tmp = tmp / 0.04;
    return tmp;
}

double convertPower(uint24_t power) {
    double tmp;
    if (power & 0x800000) {
        power = ~power;
        power++;
        tmp = -1;
    } else {
        tmp = 1;
    }
    tmp = tmp * power;
    tmp = tmp / 0x7FFFFF;
    tmp = tmp * 0.25;
    tmp = tmp * 2200;
    tmp = tmp * 0.25;
    tmp = tmp / 0.04;
    return tmp;
}

double convertCurrentGain(uint24_t gain) {
    double tmp = gain;
    tmp = tmp / 0x400000;
    return tmp;
}

void convertExe(uint24_t data) {
    for (int i = 0; i < 6; i++) {
        uint8_t nible = data & 0x04;
        if (nible < 10) {
            buffer[5 - i] = nible + '0';
        } else {
            buffer[5 - i] = nible + 'A';
        }
    }
    buffer[6] = 0;
}

void main(void) {

    InitApp();

    resetCS();
    setPos(0, 0);
    writeString("CALIBRATING...");

    startCSConversion();

    calibration();

    writeRegister(MODE, 0x000060);
    writeRegister(VOLTAGE_GAIN, 0x380000);
    writeRegister(CURRENT_GAIN, 0x480000);

    setPos(0, 0);
    writeString("life time: ");
    setPos(0, 1);
    writeString("Power Inst:");
    setPos(0, 2);
    writeString("last hour:");
    setPos(0, 3);
    writeString("mean:");

    startCSConversion();
    ei();
    uint24_t status;
    power=0;
    while (1) {
        status=0;
        
        while(newMeasure==0);
        newMeasure = 0;
        writeTime();
        data.lData = readRegister(10);
        power = convertPower(data.lData)+0.5;
        sprintf(buffer, "%.0f   ", power);
        setPos(66, 1);
        writeString(buffer);

        sample += power;
        powerTot += power;
        sampleCount++;
        if (sampleCount == MEASURE_FOR_SAMPLE) {
            uint32_t totPowerSample = 0;
            sampleCount = 0;
            sample /= MEASURE_FOR_SAMPLE;
            lastHourHistory[indexHistory] = sample;
            indexHistory++;
            if (indexHistory == HISTORY_SIZE) {
                indexHistory = 0;
            }
            for (uint16_t index = 0; index < HISTORY_SIZE; index++) {
                totPowerSample += lastHourHistory[index];
            }
            uint16_t powerHour = totPowerSample / HISTORY_SIZE;
            sprintf(buffer, "%d      ", powerHour);
            setPos(66, 2);
            writeString(buffer);
            uint16_t powerMean = powerTot / totalSeconds;
            sprintf(buffer, "%d       ", powerMean);
            setPos(66, 3);
            writeString(buffer);
        }
        displayGraph();
    }
}

static void displayGraph(void) {
    sampleGraph += power;
    counterMeasureGraph++;
    if (counterMeasureGraph == measurePerSampleGraph) {
        counterMeasureGraph = 0;
        sampleGraph /= 60;
       // sampleGraph /= 30;
        if (sampleGraph > 31) {
            sampleGraph = 31;
        }
        history[historySize] = sampleGraph;
        historySize++;
        if (historySize >= 256) {
            uint16_t mean;
            iterSrc = history;
            iterDst = history;
            while (iterSrc < iterEnd) {
                mean = *iterSrc;
                iterSrc++;
                mean += *iterSrc;
                iterSrc++;
                mean /= 2;
                *iterDst = mean;
                iterDst++;
            }
            historySize = 128;
            measurePerSampleGraph *= 2;
            sampleGraph=0;
        }
    }
    if (historySize > 10) {
        uint16_t pointsXvalue = 128 / historySize;
        pointsXvalue = pointsXvalue * historySize + 128 % historySize;
        uint16_t points;
        uint8_t x = 0;
        uint16_t value;
        uint16_t remainPoints;
        uint16_t meanValue;
        uint8_t meanCount;
        iterSrc = history;
        iterEnd = history+historySize;
        points = pointsXvalue;
        value = *iterSrc;
        while (iterSrc < iterEnd) {
            for (; points > historySize; points -= historySize) {
                verticalLine(x, value);
                x++;
            }
            meanCount = 2;
            meanValue = value * points;
            remainPoints = historySize - points;
            iterSrc++;
            value = *iterSrc;
            if (remainPoints > pointsXvalue) {
                meanValue += value * pointsXvalue;
                remainPoints -= pointsXvalue;
                iterSrc++;
                value = *iterSrc;
                meanCount++;
            }
            meanValue += value * remainPoints;
            meanValue /= ( historySize);
            verticalLine(x, meanValue);
            x++;
            points = pointsXvalue - remainPoints;
        }
    }
}

void calibration() {
    while (1) {
        uint24_t status = readRegister(STATUS_REG);
        if (status & 0x800000) {
            break;
        }
    }

    LATC2 = 1;
    TRISC2 = 0;


    SPIWriteByte(0xD9);
    setPos(0, 1);
    writeString("STEP 1 ...");
    while (1) {
        uint24_t status = readRegister(STATUS_REG);
        if (status & 0x800000) {
            break;
        }
    }
    setPos(0, 1);
    writeString("STEP 1 done");

    SPIWriteByte(0xDD);

    while (1) {
        uint24_t status = readRegister(STATUS_REG);
        if (status & 0x800000) {
            break;
        }
    }
    setPos(0, 2);
    writeString("STEP 2 done");


    LATC2 = 0;
}

static void writeTime(void) {
    uint32_t minutes = totalSeconds / 60;
    uint8_t sec = totalSeconds % 60;
    uint16_t hour = minutes / 60;
    uint8_t min = minutes % 60;
    uint16_t day = hour / 24;
    uint8_t h = hour % 24;
    setPos(60, 0);
    sprintf(buffer, "%02d:%02d:%02d     ", h, min, sec);
    writeString(buffer);
}




/*
const char * measure;
        switch (showStatus) {
            case ssConf:
                data.lData = readRegister(showStatus);
                sprintf(buffer, "%02X%02X%02X    ", data.bytes[2], data.bytes[1], data.bytes[0]);
                measure = "CONF    ";
                break;
            case ssCycleCount:
                data.lData = readRegister(showStatus);
                sprintf(buffer, "%d    ", data.lData);
                measure = "CYCLE    ";
                break;
            case ssStatus:
                data.lData = readRegister(showStatus);
                sprintf(buffer, "%02X%02X%02X    ", data.bytes[2], data.bytes[1], data.bytes[0]);
                measure = "STATUS    ";
                break;
            case ssMode:
                data.lData = readRegister(showStatus);
                sprintf(buffer, "%02X%02X%02X    ", data.bytes[2], data.bytes[1], data.bytes[0]);
                measure = "MODE    ";
                break;
            case ssTemperature:
                data.lData = readRegister(showStatus);
                sprintf(buffer, "%02X%02X%02X    ", data.bytes[2], data.bytes[1], data.bytes[0]);
                measure = "TEMP    ";
                break;
            case ssCurrentGain:
                data.lData = readRegister(showStatus);
                readVal = convertCurrentGain(data.lData);
                measure = "I GAIN  ";
                sprintf(buffer, "%f    ", readVal);
                break;
            case ssVoltageOffset:
                data.lData = readRegister(showStatus);
                readVal = convertOffset(data.lData);
                measure = "VDC offset";
                sprintf(buffer, "%f    ", readVal);
                break;
            case ssVoltageGain:
                data.lData = readRegister(showStatus);
                readVal = convertCurrentGain(data.lData);
                measure = "V GAIN  ";
                sprintf(buffer, "%f    ", readVal);
                break;
            case ssVoltageAcOffset:
                data.lData = readRegister(showStatus);
                readVal = convertOffset(data.lData);
                measure = "VAC offset";
                sprintf(buffer, "%f    ", readVal);
                break;

            case ssCurrent:
                data.lData = readRegister(showStatus);
                readVal = convertCurrent(data.lData);
                measure = "CURRENT ";
                sprintf(buffer, "%f    ", readVal);
                break;
            case ssVolt:
                data.lData = readRegister(INST_VOLT);
                readVal = convertInstVolt(data.lData);
                measure = "VOLT    ";
                sprintf(buffer, "%f    ", readVal);
                break;
            case ssRMSVoltage:
                data.lData = readRegister(ssRMSVoltage);
                readVal = convertRMSVolt(data.lData);
                sprintf(buffer, "%f    ", readVal);
                measure = "RMS VOLT";
                break;
            case ssRMSCurrent:
                data.lData = readRegister(ssRMSCurrent);
                readVal = convertRMSCurrent(data.lData);
                sprintf(buffer, "%f    ", readVal);
                measure = "RMS I";
                break;
            case ssPower:
                data.lData = readRegister(INST_POWER);
                readVal = convertPower(data.lData);
                measure = "I POWER  ";
                sprintf(buffer, "%f    ", readVal);
                break;
            case ssActivePower:
                data.lData = readRegister(ssActivePower);
                readVal = convertPower(data.lData);
                measure = "I POWER  ";
                sprintf(buffer, "%f    ", readVal);
                break;
            default:
                data.lData = readRegister(showStatus);
                sprintf(buffer, "%02X%02X%02X    ", data.bytes[2], data.bytes[1], data.bytes[0]);
                measure = "        ";
                break;
        }
 * */