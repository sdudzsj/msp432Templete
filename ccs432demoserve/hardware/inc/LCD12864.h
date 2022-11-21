#ifndef _LCD12864_H_
#define _LCD12864_H_


#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include "delay.h"

extern void IO_Init();
extern void LCD12864_Delay(uint32_t z);
extern void LCD12864_SendByte(uint8_t zdata);
extern void LCD12864_WriteCom(uint8_t com);
extern void LCD12864_WriteDate(uint8_t date);
extern void LCD12864_Init();
extern void init_picture();
extern void init_char();
extern void show_Pic(uint8_t *address);
extern void LCD12864_CleanScans();
extern void LCD12864_SetPos(uint8_t x,uint8_t y);
extern void LCD12864_DisplayString(uint8_t *s,uint8_t n);
extern void set_pos(uint8_t addr,uint8_t x);
extern void lcd_send_data(uint8_t x,uint8_t y,uint8_t *s);


#endif
