#include "LCD.h"
#define LCD_Port PORTB
#define LCD_BPin DDRB
#define RSPIN PB0
#define ENPIN PB1

const char clave1[]="246";
char clave_ingresada[3];
int i=0, j=0, datos=0, key=0, key1=0;
int nf=0, nc=0;
const char digitos[4][4]={
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'*', '0', '#', 'D'}
};

void LCD_Init(void){
	LCD_BPin = 0xFF;
	_delay_ms(15);
	LCD_Action(0x02);
	LCD_Action(0x28);
	LCD_Action(0x0c);
	LCD_Action(0x06);
	_delay_ms(2);
}

void LCD_Action(unsigned char cmnd){
	LCD_Port = (LCD_Port & 0x0F) | (cmnd & 0xF0); //11110000 OR
	LCD_Port &= ~(1<<RSPIN);
	LCD_Port |= (1<<ENPIN);
	_delay_us(1);
	LCD_Port &= ~(1<<ENPIN);
	_delay_us(200);
	LCD_Port = (LCD_Port & 0x0F) | (cmnd << 4);
	LCD_Port |= (1<<ENPIN);
	_delay_us(1);
	LCD_Port &= ~(1<<ENPIN);
	_delay_ms(2);
}

void LCD_Clear(){
	LCD_Action(0x01);	//Clear LCD
	_delay_ms(2);			//Wait to Clean LCD
	LCD_Action(0x80);	//Move to Position Line 1, Position 1
}

void LCD_Print(char *str){
	int i;
	for(i=0; str[i]!=0; i++){
		LCD_Port = (LCD_Port & 0x0F) | (str[i] & 0xF0); //11110000 OR
		LCD_Port |= (1<<RSPIN);
		LCD_Port |= (1<<ENPIN);
		_delay_us(1);
		LCD_Port &= ~(1<<ENPIN);
		_delay_us(200);
		LCD_Port = (LCD_Port & 0x0F) | (str[i] << 4);
		LCD_Port |= (1<<ENPIN);
		_delay_us(1);
		LCD_Port &= ~(1<<ENPIN);
		_delay_ms(2);
	}
}

void LCD_Printpos(char row, char pos, char *str){
	if((row=0) && (pos<16)){
		LCD_Action((pos & 0x0F)|0x80);
	}
	else if(row==1 && pos<16){
		LCD_Action((pos & 0x0F)|0xC0);
		LCD_Print(str);
	}
}

void ejecutar_keypadLCD(){
		LCD_Action(0XC0);
		LCD_Print("Clave: ");
		for (nf = 0; nf <=3; nf++){
			PORTD &= ~(1<<(7-nf));
			for (nc = 0; nc <= 3; nc++){
				if(((PINC>>(3-nc))&1) == 0){
					clave_ingresada[i]=digitos[nf][nc];
					_delay_ms(250);
					LCD_Action(0XC0);
					LCD_Print("Clave: ");
					LCD_Print(clave_ingresada);
					_delay_ms(100);
				
					
					if(i>=2){
						for(j=0; j<3; j++){
							if(clave_ingresada[j]==clave1[j]){
								key++;
							}
						}
						
						LCD_Clear();
						LCD_Action(0xC0);
						_delay_ms(100);

						if ((key==3)||(key1==3)){
							LCD_Print("ABRIENDO PUERTA");
							_delay_ms(1000);
							LCD_Action(0x01); //Clear
							LCD_Print("  *BIENVENIDO");
							LCD_Action(0xC1);
							LCD_Print("  A CASA*");
							LCD_Action(0xC0);						   

							//ServoMotor
						   	move_to_ninety(); //DEZPLAZA EL SERVO MOTOR A 90°
						   	enviar_raspi2();
							//PORTB |= (1<<PB2);
						   	//_delay_ms(300);
							//PORTB &= ~(1<<PB2);
							_delay_ms(250);				   
							LCD_Action(0x01);

							for (i=0; i<3;i++){
								clave_ingresada[i]=0;
							}
							LCD_Print("INGRESE CLAVE");
						}
						else{
							LCD_Print("ACCESO DENEGADO");
							_delay_ms(500);
							LCD_Action(0x01);
						        enviar_raspi(); //SE ACTIVA LA CÁMARA
						   
							for (i=0;i<3;i++){
								clave_ingresada[i]=0;
							}
							LCD_Print("INGRESE CLAVE:");
						}
						key=0;
						key1=0;
						i=-1;
					}
					i++;
				}
			}
			PORTD |= (1<< (7-nf));
		}
		_delay_ms(10);
}

void inicializar_keypadLCD(){
	LCD_Init();
	LCD_Print("INGRESE CLAVE:");
	DDRD= 0xF0;
	PORTD |= 0xFF;
	DDRC |= ((1 << DDC4));
        PORTC |= 0xF;
	DDRB |= ((1 << DDB2));
}

void enviar_raspi(){
   _delay_ms(1500);
   serial_println_str("1");
   _delay_ms(1500);
}

void enviar_raspi2(){
   _delay_ms(1500);
   serial_println_str("2");
   _delay_ms(1500);
}

void enviar_raspi3(){
   _delay_ms(1500);
   serial_println_str("3");
   _delay_ms(1500);
}

void move_to_ninety(){
   int i;
   for(i = 0; i<50; i++){
      PORTC = 0x10;
      _delay_us(1500);
      PORTC = 0x00;
      _delay_ms(19);
    }
}

void move_to_zero(){
   int i;
   for(i = 0; i<50; i++){
      PORTC = 0x10;
      _delay_us(700);
      PORTC = 0x0;
      _delay_ms(20);
    }
}
