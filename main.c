#include "LCD.h"
#include "uart.h"
int main(){
	serial_begin();
	inicializar_keypadLCD();
    while(1){
       if((PIND &(1 << PD3))){
	  PORTB |= (1<<PB2);
	  enviar_raspi3();
          move_to_zero();
       }
       else{
	  PORTB &= ~(1<<PB2);
	  ejecutar_keypadLCD();
	  
       }
    }
	return 0;
}
