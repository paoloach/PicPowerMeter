/* 
 * File:   SPI.h
 * Author: paolo
 *
 * Created on December 24, 2015, 8:06 AM
 */

#ifndef __SPI_H__
#define	__SPI_H__

#include <stdint.h>

 void   initSPI(void);
  
 uint24_t readRegister(uint8_t reg);
 void writeRegister(uint8_t reg, uint24_t);
 void startCSConversion();
 void resetCS();
 uint8_t SPIWriteByte(uint8_t reg);
 
 
#endif	/* SPI_H */

