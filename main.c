/*
 * GccApplication5.c
 *
 * Created: 4/6/2023 1:08:33 PM
 * Author : genel
 */ 
#define F_CPU 16000000UL
#define BAUD_RATE 9600
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)

//add more games and apps here
#define HOME 0
#define TRAWN 1
#define GAME2 2

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "UTFT.h"

extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];

char buffer[25];
short curr_app = HOME;
short game_select = 0;

void UART_init(void)
{
	UBRR0H = (unsigned char) (BAUD_PRESCALER>>8);
	UBRR0L = (unsigned char)BAUD_PRESCALER;
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
	UCSR0C |= (1<<USBS0);
}

void UART_send(unsigned char data)
{
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}

void UART_putstring(char* buf)
{
	while (*buf != 0x00)
	{
		UART_send(*buf);
		buf++;
	}
}

void init() {
	 cli();
	 UART_init();
	 //adc setup
	 PRR0 &= ~(1<<PRADC);
	 ADMUX |= (1<<REFS0) | (1<<REFS1); //2.56 v comp
	 
	 //adc timer /64
	 ADCSRA &= ~(1<<ADPS0);
	 ADCSRA |= (1<<ADPS1);
	 ADCSRA |= (1<<ADPS2);
	 
	 //mux adc pin selection (initial 0)
	 ADMUX &= ~(1<<MUX4 | 1<<MUX3 | 1<<MUX2 | 1<<MUX1 | 1<<MUX0);
	 ADCSRB &= ~(1<<MUX5);
	 
	 //autotrigger, free running
	 ADCSRA |= (1<<ADATE);
	 ADCSRB &= ~(1<<ADTS0);
	 ADCSRB &= ~(1<<ADTS1);
	 ADCSRB &= ~(1<<ADTS2);
	 
	 //disable digital input buffer, enable adc, start conversion
	 DIDR0 |= (1<<ADC0D);
	 ADCSRA |= (1<<ADEN);
	 ADCSRA |= (1<<ADSC);
	 
	 //p1, p2 joystick digital input pins
	 DDRF &= ~(1<<DDF7 | 1<<DDF6 | 1<<DDF5 | 1<<DDF4 | 1<<DDF1 | 1<<DDF2 | 1<<DDF3);
	 DDRK &= ~(1<<DDK0);
	 //p1, p2 xy button inputs
	 //k1 = p1 x
	 //k2 = p1 y
	 //k3 = p2 x
	 //k4 = p2 y
	 DDRK &= ~(1<<DDK1 | 1<<DDK2 | 1<<DDK3 | 1<<DDK4);
	 
	 //buzzer output and pwm setup
	 //prescale timer1 by 256
	 TCCR1B |= (1<<CS12);
	 TCCR1A |= (1<<WGM10);
	 TCCR1A &= ~(1<<WGM11);
	 TCCR1B |= (1<<WGM12);
	 TCCR1A |= 1<<COM1B0;
	 TCCR1A |= 1<<COM1B1;
	 
	 OCR1AH = 255;
	 OCR1AL = 255;
	 OCR1BH = 0;
	 
	 //home button
	 DDRH &= ~(1<< DDH5);
	 
	 sei();
}

void sound_buzzer() {
	DDRB |= (1<<DDB6);
}

void homescreen() {
	//homescreen with game icons
	//navigate with joysticks, select with x
	fillScr_rgb(0, 0, 0);
	//draw trawn game icon
	setColor_rgb(255, 255, 255);
	//120x80 icon size
	drawRect(20, 60, 140, 140);
	setFont(BigFont);
	print("Trawn", 40, 145, 0);
	
	//trawn snakes in logo
	setColor_rgb(255, 151, 15);
	fillRect(32, 80, 48, 84);
	fillRect(48, 80, 52, 124);
	fillRect(48, 120, 116, 124);
	fillRect(112, 100, 116, 124);
	setColor_rgb(15, 143, 255);
	fillRect(53, 72, 100, 76);
	fillRect(53, 72, 56, 112);
	fillRect(53, 108, 111, 112);
	fillRect(53, 108, 111, 112);
	
	//draw 2nd game icon
	setColor_rgb(255, 255, 255);
	drawRect(180, 60, 300, 140);
	setFont(BigFont);
	print("Game2", 200, 145, 0);
	setColor_rgb(255, 151, 15);
	fillRect(200, 118, 212, 139);
	fillRect(200, 118, 212, 139);
	fillRect(212, 125, 217, 127);
	fillRect(272, 125, 280, 127);
	fillRect(230, 125, 238, 127);
	setColor_rgb(255, 255, 255);
	drawHLine(250, 100, 49);
	setColor_rgb(15, 143, 255);
	fillRect(260, 78, 272, 99);
	fillRect(260, 78, 272, 99);
	fillRect(255, 85, 260, 87);
	fillRect(200, 85, 208, 87);
	
	game_select = 0;
}


//define control macros
#define P1LEFT PINF & (1<<PINF2)
#define P1RIGHT PINF & (1<<PINF3)
#define P1UP PINF & (1<<PINF4)
#define P1DOWN PINF & (1<<PINF1)
#define P1X PINK & (1<<PINK1)
#define P1Y PINK & (1<<PINK2)

#define P2LEFT PINF & (1<<PINF6)
#define P2RIGHT PINF & (1<<PINF5)
#define P2UP PINK & (1<<PINK0)
#define P2DOWN PINF & (1<<PINF7)
#define P2X PINK & (1<<PINK3)
#define P2Y PINK & (1<<PINK4)

void update_homescreen() {
	//p1 controls select with left/right
	if (P1LEFT) {
		game_select = 1;
	} else if (P1RIGHT) {
		game_select = 2;
	}
	setColor_rgb(255, 255, 255);
	drawRect(180, 60, 300, 140);
	setColor_rgb(255, 255, 255);
	drawRect(20, 60, 140, 140);
	switch (game_select) {
		case 1:
			setColor_rgb(255, 0, 0);
			drawRect(20, 60, 140, 140);
			break;
		case 2:
			setColor_rgb(255, 0, 0);
			drawRect(180, 60, 300, 140);
			break;
	}
	//p1 x selects game
	if (P1X) {
		switch (game_select) {
			case (1):
				curr_app = TRAWN;
				reset_snake_game();
				break;
			case (2):
				curr_app = GAME2;
				setup_game2();
				break;
		}
	}
}

//game imports should go after global vars(compiler paste)
#include "snake.h"
#include "game2.h"

int main(void)
{
    //init, init lcd
	init();
	setupLCDPorts(17,38,39,40,41,0);
	InitLCD(LANDSCAPE);
	
	//initial game always snake
	//reset_snake_game();
	homescreen();
	int loop_counter = 0;
    while (1) 
    {
		//home button
		if (PINH & (1<<PINH5)) {
			curr_app = HOME;
			homescreen();
		}
		if (loop_counter % 20 == 0) {
			DDRB &= ~(1<<DDB6);
		}
		int duty_cycle = (int)(ADC/1024.f * 50);
		OCR1BL = OCR1AL - (0.1f*OCR1AL*duty_cycle)/100 - 1;
		switch (curr_app) {
			case (TRAWN):
				if (loop_counter % 10 == 0) {
					update_snake_game();
				}
				_delay_ms(7);
				break;
			case (HOME):
				update_homescreen();
				break;
			case (GAME2):
				update_game2();
				_delay_ms(10);
				break;
		}
		loop_counter++;
    }
}

