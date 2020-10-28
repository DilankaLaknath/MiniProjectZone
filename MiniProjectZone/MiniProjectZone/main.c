/*
 * MiniProjectZone.c
 *
 * Created: 10/27/2020 1:32:33 PM
 * Author : Kavish Ranawella
 */ 

#define F_CPU 16000000L // Specify oscillator frequency
#include <avr/io.h>
#include "LEDdriver.h"
#include "Buttondriver.h"
#include "packet_handler.h"

#define g_led_port PortB		//LED port number
#define g_led_pin 7			    //LED pin number
#define g_btn_port PortD		//Button port number
#define g_btn_pin 0				//Button pin number
#define g_uart_number 0

#define buffer_length 5

void on_button_pressed(portx buttonPort, uint8_t buttonPin, btn_state buttonState);
void on_packet_receive_complete(uint8_t uart_number, uint8_t tl_packet [] , PACKET_CRC_ERR_STATE_t error);
void on_packet_transmission_complete(uint8_t uart_number ,uint32_t status);


int main(void)
{
	btn_init (g_btn_port,g_btn_pin);
	btn_set_event_callback(on_button_pressed);
	//PH_CALLBACKS_t * callbacks = {on_packet_transmission_complete, on_packet_receive_complete};
	ph_init(g_uart_number, on_packet_transmission_complete, on_packet_receive_complete);
	led_init (g_led_port, g_led_pin);
	led_off (g_led_port, g_led_pin);

	
	/* Replace with your application code */
	while (1)
	{
		
	}
}

void on_packet_receive_complete(uint8_t uart_number, uint8_t tl_packet [] , PACKET_CRC_ERR_STATE_t error)
{
	uint8_t buffer [buffer_length] = {3, 5, 6, 10, 24};
	if(uart_number == g_uart_number && error == NO_CRC_ERROR)// && tl_packet==buffer)
	{
		
			if (tl_packet[0]==3 )
			{
			 if (tl_packet[1]==5)
			 { 
			  if (tl_packet[2]==6)
			  {
			   if (tl_packet[3]==10)
			   {
				   if (tl_packet[4]==24)
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
			  }
			 }
				
			
		}
		
	}
}

void on_packet_transmission_complete(uint8_t uart_number ,uint32_t status)
{
	if(uart_number == g_uart_number)
	{
		led_toggle(g_led_port, g_led_pin);
	}	
}


void on_button_pressed(portx buttonPort, uint8_t buttonPin, btn_state buttonState)
{
	uint8_t buffer [buffer_length] = {3, 5, 6, 10, 24};
	if( (buttonPort == g_btn_port) && (buttonPin == g_btn_pin) && (buttonState == pressed) )
	{
		ph_transmit_packet(g_uart_number, buffer);
	}
}

