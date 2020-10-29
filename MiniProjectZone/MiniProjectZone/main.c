/*
 * MiniProjectZone.c
 *
 * Created: 10/27/2020 1:32:33 PM
 * Author : Kavish Ranawella
 */ 
//
//#define F_CPU 16000000L // Specify oscillator frequency
//#include <avr/io.h>
//#include "LEDdriver.h"
//#include "Buttondriver.h"
//#include "command_processor.h"
//#include "packet_handler.h"
//#include <string.h>
//
//
//#define g_led_port PortB		//LED port number
//#define g_led_pin 7			    //LED pin number
//#define g_btn_port PortE		//Button port number
//#define g_btn_pin 4				//Button pin number
//#define g_uart_number 0

//#define buffer_length 5

//packet_t g_rx_packet;
//bool g_is_packet_new = false;
//bool g_processing_request = false;
//
//void on_packet_received (uint8_t uart_number, packet_t * packet , PACKET_CRC_ERR_STATE_t error);
//void when_transmission_complete(uint8_t uart_number);
//void on_button_pressed(enum portx buttonPort, uint8_t buttonPin, bool buttonState);
//
//
//int main(void)
//{
	//g_is_packet_new = false;
	//g_processing_request = false;
	//
	//btn_init (g_btn_port,g_btn_pin);
	//PORTE |= (1<<PE4); //PULL UP THE button
	//btn_set_event_callback(on_button_pressed);
	//
	//led_init (g_led_port, g_led_pin);
	//
	//while (1)
	//{
		//if(g_is_packet_new)
		//{
			//g_processing_request = true;
			//packet_t response;
			//cmd_proc_process_request(&g_rx_packet, &response);
			//ph_transmit_packet(g_uart_number, &response);
			//g_processing_request = false;
		//}
	//}
//}


//void when_transmission_complete(uint8_t uart_number)
//{
	//if(uart_number == g_uart_number)
	//{
		//led_toggle(g_led_port, g_led_pin);
	//}
//}
//
//void on_packet_received (uint8_t uart_number, packet_t * packet , PACKET_CRC_ERR_STATE_t error)
//{
	//if(!g_processing_request)
	//{
		//g_is_packet_new = true;
		//memcpy(&g_rx_packet, packet, sizeof(packet_t));
	//}
//}
//
//void on_button_pressed(enum portx buttonPort, uint8_t buttonPin, btn_state buttonState)
//{
	//if( (buttonPort == g_btn_port) && (buttonPin == g_btn_pin) && (buttonState == 0) )
	//{
		//cmd_blink_led_function(g_led_port, g_led_pin);
	//}
//}



#define F_CPU 16000000L // Specify oscillator frequency
#include <avr/io.h>
#include "LEDdriver.h"
#include "Buttondriver.h"
#include "command_processor.h"
#include "packet_handler.h"
#include <string.h>


#define g_led_port PortB		//LED port number
#define g_led_pin 7			    //LED pin number
#define g_btn_port PortE		//Button port number
#define g_btn_pin 4				//Button pin number
#define g_uart_number 0


#define  g_max_processing_rx_packets  5
packet_t g_rx_packet_array[g_max_processing_rx_packets]; //use this circular array to buffer the received packets
uint8_t g_read_array_index = 0;
uint8_t g_write_array_index = 0;
uint8_t g_array_filled_length = 0;

void on_packet_received (uint8_t uart_number, packet_t * packet , PACKET_CRC_ERR_STATE_t error);
void when_ph_tx_complete(uint8_t uart_number, uint32_t status);
void on_button_pressed(portx buttonPort, uint8_t buttonPin, btn_state buttonState);


int main(void)
{
	
	g_read_array_index = 0;
	g_write_array_index = 0;
	g_array_filled_length = 0;
	
	//to initialize buttons
	btn_init (g_btn_port,g_btn_pin);
	PORTE |= (1<<PE4); //PULL UP THE button
	btn_set_event_callback(on_button_pressed);
	
	//to initialize the LEDs
	led_init (g_led_port, g_led_pin);
	
	//to initialize the packet handler
	PH_CALLBACKS_t g_ph_callbacks = {.rx_complete_cb = on_packet_received, .tx_complete_cb = when_ph_tx_complete};	
	ph_init( g_uart_number, &g_ph_callbacks);
	
	while (1)
	{
		if(g_array_filled_length != 0)
		{
			packet_t response;	 //create a packet for response			 
			cmd_proc_process_request( &(g_rx_packet_array[g_read_array_index]), &response);
			//cmd_proc_process_request( g_rx_packet_array, &response);

			while(ph_get_status(g_uart_number) == PH_STATUS_TRANSMITTING); // wait until packet handler is free
			
			ph_transmit_packet(g_uart_number, &response);
			
			g_array_filled_length --;  
			g_read_array_index = (g_read_array_index == (g_max_processing_rx_packets-1))? 0:(g_read_array_index + 1);  // reset read point			
		}
	}
}

void when_ph_tx_complete(uint8_t uart_number, uint32_t status)
{
	if(uart_number == g_uart_number)    // didn't consider about the 'status'
	{
		led_toggle(g_led_port, g_led_pin);
	}
}

void on_packet_received (uint8_t uart_number, packet_t * packet , PACKET_CRC_ERR_STATE_t error)
{
	if(g_array_filled_length < g_max_processing_rx_packets)
	{ 
		memcpy(&(g_rx_packet_array[g_write_array_index]), packet, sizeof(packet_t));
		g_array_filled_length ++;
		g_write_array_index = (g_write_array_index == (g_max_processing_rx_packets-1))? 0: g_write_array_index + 1; //reset write point
	}
}

void on_button_pressed(portx buttonPort, uint8_t buttonPin, btn_state buttonState)
{
	if( (buttonPort == g_btn_port) && (buttonPin == g_btn_pin) && (buttonState == 0) )
	{
		//cmd_blink_led_function(g_led_port, g_led_pin);
	}
}

