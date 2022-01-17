#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "uart.h"

void LCD_Init(void);
void LCD_Action(unsigned char cmnd);
void LCD_Clear();
void LCD_Print(char *str);
void LCD_Printpos(char row, char pos, char *str);
void ejecutar_keypadLCD();
void inicializar_keypadLCD();
void enviar_raspi();
void move_to_ninety();
void move_to_zero();
void enviar_raspi2();
void enviar_raspi3();
