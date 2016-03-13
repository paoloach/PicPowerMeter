/* 
 * File:   KS0108.h
 * Author: paolo
 *
 * Created on February 14, 2016, 9:05 PM
 */

#ifndef KS0108_H
#define	KS0108_H

#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif
    
   void  initKS0108(void);
   void setPos(uint8_t x, uint8_t y);
   void writeChar(char c);
   void writeString(const char *);
   void verticalLine(uint8_t x, uint8_t h);

#ifdef	__cplusplus
}
#endif

#endif	/* KS0108_H */

