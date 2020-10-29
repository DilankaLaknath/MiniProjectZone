///*
 //* MiniProjectZone.c
 //*
 //* Created: 10/27/2020 1:32:33 PM
 //* Author : Kavish Ranawella
 //*/ /
//
#define F_CPU 16000000L // Specify oscillator frequency
#include <avr/io.h>
#include "LEDdriver.h"
#include "Buttondriver.h"
#include "packet_handler.h"
#include "UART_HAL.h"

#define g_led_port PortB		//LED port number
#define g_led_pin 7			    //LED pin number
#define g_btn_port PortE		//Button port number
#define g_btn_pin 4				//Button pin number   //arduino board pin 2 
#define g_uart_number 0

int main(void)
{
	led_init ( g_led_port, g_led_pin);
	btn_init (g_btn_port,g_btn_pin);
	PORTE |= (1<<4);   // pull up the button 
	
	
}















































//
//#define g_led_port PortB		//LED port number
//#define g_led_pin 7			    //LED pin number
//#define g_btn_port PortE		//Button port number
//#define g_btn_pin 4				//Button pin number
//#define g_uart_number 0
//
//#define buffer_length 5
//
//void when_byte_received(uint8_t uart_number, char data, bool parity_error);
//void when_transmission_complete(uint8_t uart_number);
//void on_button_pressed(portx buttonPort, uint8_t buttonPin, btn_state buttonState);
//
//
//int main(void)
//{
	//btn_init (g_btn_port,g_btn_pin);
	//btn_set_event_callback(on_button_pressed);
	//PORTE |= (1<<4);
	//led_init (g_led_port, g_led_pin);
	//led_off (g_led_port, g_led_pin);
	//hal_uart_init(g_uart_number, 9600, 8, NO_PARITY);
	//hal_uart_set_callbacks(when_byte_received, when_transmission_complete );
	//
	///* Replace with your application code */
	//while (1)
	//{
		//
	//}
//}
//
//void when_transmission_complete(uint8_t uart_number)
//{
	//if(uart_number == g_uart_number)
	//{
		//led_toggle(g_led_port, g_led_pin);
	//}
//}
//
//
//
//void on_button_pressed(portx buttonPort, uint8_t buttonPin, btn_state buttonState)
//{
	////uint8_t buffer [buffer_length] = {3, 5, 6, 10, 24};
	//if( (buttonPort == g_btn_port) && (buttonPin == g_btn_pin) && (buttonState == pressed) )
	//{
		////hal_uart_send(g_uart_number, &buffer[0], buffer_length);
		//
		//led_blink(g_led_port, g_led_pin, 3);
		////led_toggle(g_led_port, g_led_pin);
	//}
//}

