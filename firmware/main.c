/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include <xc.h>        /* XC8 General Include File */
#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */
#include <string.h>
#include <stdio.h>


// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// CONFIG1L
#pragma config PLLDIV = 1       // PLL Prescaler Selection bits (No prescale (4 MHz oscillator input drives PLL directly))
#pragma config CPUDIV = OSC1_PLL2// System Clock Postscaler Selection bits ([Primary Oscillator Src: /1][96 MHz PLL Src: /2])
#pragma config USBDIV = 1       // USB Clock Selection bit (used in Full-Speed USB mode only; UCFG:FSEN = 1) (USB clock source comes directly from the primary oscillator block with no postscale)

// CONFIG1H
#pragma config FOSC = INTOSC_XT // Oscillator Selection bits (Internal oscillator, XT used by USB (INTXT))
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
#include "LCD.h"
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

void calibration(void);

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
    tmp = tmp * 2200;
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
    tmp = tmp * 10;
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
    tmp = tmp * 10;
    tmp = tmp /4;
    return tmp;
}

double convertCurrentGain(uint24_t gain) {
    double tmp = gain;
    tmp = tmp / 0x400000;
    return tmp;
}

char buffer[20];

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

union data_t {
    uint24_t lData;
    unsigned char bytes[3];
} data;

void main(void) {

    /* Configure the oscillator for the device */
    ConfigureOscillator();



    /* Initialize I/O and Peripherals for application */
    InitApp();

    resetCS();

    startCSConversion();

    line1();
    writeLCD("START CALIBRATION");

    calibration();

    writeRegister(MODE, 0x00006000);
    writeRegister(VOLTAGE_GAIN, 0x2F9000);
    writeRegister(CURRENT_GAIN, 0x3Fe0000);

    startCSConversion();
    while (1) {
        uint24_t status = readRegister(STATUS_REG);
        line2();
        if (!(status & 0x1)) {
            convertExe(status);
            writeLCD(buffer);
            resetCS();
            startCSConversion();
            continue;
        }
        double readVal;
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

        line1();
        writeLCD(buffer);
        line2();
        writeLCD(measure);
        if (PORTBbits.RB2 == 0) {
            if (PORTBbits.RB2 == 0) {
                showStatus++;
                if (showStatus == ssLast) {
                    showStatus = ssConf;
                }
                while (1) {
                    if (PORTBbits.RB2 == 1) {
                        if (PORTBbits.RB2 == 1) {
                            break;
                        }
                    }
                }
            }
        }
    }

}

void calibration() {
    line2();
    writeLCD("DC OFFSET");

    while (1) {
        uint24_t status = readRegister(STATUS_REG);
        if (status & 0x800000) {
            break;
        }
    }

    LATC0 = 1;
    LATC1 = 1;
    TRISC0 = 0;
    TRISC1 = 0;


    SPIWriteByte(0xD9);

    while (1) {
        uint24_t status = readRegister(STATUS_REG);
        if (status & 0x800000) {
            break;
        }
    }

    writeLCD("AC OFFSET");
    SPIWriteByte(0xDD);

    while (1) {
        uint24_t status = readRegister(STATUS_REG);
        if (status & 0x800000) {
            break;
        }
    }


    LATC0 = 0;
    LATC1 = 0;
    line2();
    writeLCD("DONE");
}