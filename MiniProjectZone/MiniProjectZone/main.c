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
#include "UART_HAL.h"

#define g_led_port PortB		//LED port number
#define g_led_pin 7			    //LED pin number
#define g_btn_port PortD		//Button port number
#define g_btn_pin 0				//Button pin number
#define g_uart_number 0

#define buffer_length 5

packet_t g_rx_packet;
bool g_is_packet_new = false;
bool g_processing_request = false;

void on_packet_received (uint8_t uart_number, packet_t * packet , PACKET_CRC_ERR_STATE_t error);
void when_transmission_complete(uint8_t uart_number);
void on_button_pressed(enum portx buttonPort, uint8_t buttonPin, bool buttonState);


int main(void)
{
	g_is_packet_new = false;
	g_processing_request = false;
	
	btn_init (g_btn_port,g_btn_pin);
	btn_set_event_callback(on_button_pressed);
	led_init (g_led_port, g_led_pin);
	led_off (g_led_port, g_led_pin);
	
	/* Replace with your application code */
	while (1)
	{
		if(g_is_packet_new)
		{
			g_processing_request = true;
			packet_t response;
			cmd_proc_process_request(&g_rx_packet, &response);
			ph_transmit_packet(uint8_t uart_number, &response);
			g_processing_request = false;
		}
	}
}

void when_transmission_complete(uint8_t uart_number)
{
	if(uart_number == g_uart_number)
	{
		led_toggle(g_led_port, g_led_pin);
	}
}

void on_packet_received (uint8_t uart_number, packet_t * packet , PACKET_CRC_ERR_STATE_t error)
{
	if(!g_processing_request)
	{
		g_is_packet_new = true;
		memcpy(&g_rx_packet, packet, sizeof(packet_t));
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

