/*
 * UART.c
 *
 * Created: 10/22/2020 5:39:39 PM
 * Author : Dilanka Wickramasinghe
 */ 

#define F_CPU 16000000L // Specify oscillator frequency
#include <avr/io.h>
#include "LEDdriver.h"
#include "Buttondriver.h"
#include "UART_HAL.h"

enum portx {A,B,C,D,E,F,G,H,J,K,L};

#define g_led_port B			//LED port number
#define g_led_pin 7			    //LED pin number
#define g_btn_port D			//Button port number
#define g_btn_pin 0				//Button pin number
#define g_uart_number 0         

#define buffer_length 5

void when_byte_received(uint8_t uart_number, char data, bool parity_error);
void when_transmission_complete(uint8_t uart_number);
void on_button_pressed(enum portx buttonPort, uint8_t buttonPin, bool buttonState);


int main(void)
{	
	btn_init (g_btn_port,g_btn_pin);
	btn_set_event_callback(on_button_pressed);
	led_init (g_led_port, g_led_pin);
	led_off (g_led_port, g_led_pin);
	hal_uart_init(g_uart_number, 9600, 8, NO_PARITY);
	hal_uart_set_callbacks(when_byte_received, when_transmission_complete );
	
    /* Replace with your application code */
    while (1) 
    {
		
    }
}

void when_transmission_complete(uint8_t uart_number)
{
	if(uart_number == g_uart_number)
	{		
		led_toggle(g_led_port, g_led_pin);
		_delay_ms(500);
	}	
}

void when_byte_received(uint8_t uart_number, char data, bool parity_error)
{
	if ((uart_number == g_uart_number) && (data == 5) && (parity_error == false) )
	{
		led_toggle(g_led_port, g_led_pin);
		_delay_ms(500);
		led_toggle(g_led_port, g_led_pin);
		_delay_ms(500);
		led_toggle(g_led_port, g_led_pin);
		_delay_ms(500);
		led_toggle(g_led_port, g_led_pin);
		_delay_ms(500);
	}
}

void on_button_pressed(enum portx buttonPort, uint8_t buttonPin, bool buttonState)
{	
	uint8_t buffer [buffer_length] = {3, 5, 6, 10, 24};
	if( (buttonPort == g_btn_port) && (buttonPin == g_btn_pin) && (buttonState == 0) )
	{
		hal_uart_send(g_uart_number, &buffer[0], buffer_length);
	}
}

