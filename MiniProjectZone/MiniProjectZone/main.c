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
#include "UART_HAL.h"

#define g_led_port PortB		//LED port number
#define g_led_pin 7			    //LED pin number
#define g_btn_port PortD		//Button port number
#define g_btn_pin 0				//Button pin number
#define g_uart_number 0

#define buffer_length 5

void on_button_pressed(portx buttonPort, uint8_t buttonPin, btn_state buttonState);
void on_packet_receive_complete(uint8_t uart_number, packet_t * packet , PACKET_CRC_ERR_STATE_t error);
void on_packet_transmission_complete(uint8_t uart_number ,uint32_t status);


int main(void)
{
	btn_init (g_btn_port,g_btn_pin);
	btn_set_event_callback(on_button_pressed);
	PH_CALLBACKS_t callbacks;
	callbacks.tx_complete_cb = on_packet_transmission_complete;
	callbacks.rx_complete_cb= on_packet_receive_complete;
	ph_init(g_uart_number, &callbacks);
	led_init (g_led_port, g_led_pin);
	led_off (g_led_port, g_led_pin);

	
	/* Replace with your application code */
	while (1)
	{
		
	}
}

void on_packet_receive_complete(uint8_t uart_number, packet_t * packet , PACKET_CRC_ERR_STATE_t error)
{
	
	//uint8_t buffer [buffer_length] = {3, 5, 6, 10, 24};
	if(uart_number == g_uart_number && error == NO_CRC_ERROR)// && tl_packet==buffer)
	{
		
		
			if (packet->data[0]==3 )
			{
			 if (packet->data[1]==5)
			 { 
			  if (packet->data[2]==6)
			  {
			   if (packet->data[3]==10)
			   {
				   if (packet->data[4]==24)
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
	packet_t temp;
	packet_t * packet;
	packet=&temp;
	uint8_t len = 3;
	uint8_t arr[3]={2,8,1};
	
 	packet->length= len;
	 memcpy(packet->data,arr,len);	
	if( (buttonPort == g_btn_port) && (buttonPin == g_btn_pin) && (buttonState == pressed) )
	{
		ph_transmit_packet(g_uart_number, packet);
		
	}
}

