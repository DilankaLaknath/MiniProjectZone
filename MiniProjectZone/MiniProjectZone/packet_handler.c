#include "packet_handler.h"
#include "UART_HAL.h"

#define RX_PKT_BUFF_LEN 20
#define TX_PKT_BDY_BUFF_LEN 20

/*! Structure which holds application callbacks. Callbacks has been set to null */
PH_CALLBACKS_t g_app_callbacks = {0 ,0};

/*! Holds the current state of the packet collector */
PACKET_COLLECTOR_STATE_t g_packet_collector_state;

/*! Holds the newest byte received via UART */
uint8_t g_rx_Byte;

/*! Used to calculate temporary CRC values */
uint16_t g_cal_crc;

/*! Used to store the received packet */
uint8_t g_rx_packet_buffer[RX_PKT_BUFF_LEN];

/*! Used to keep the transmitting packet */
uint8_t g_tx_buff[sizeof(TX_PKT_BDY_BUFF_LEN) + 8];   //8 is the packet handler overhead

/*! Used to store the received CRC */
uint16_t g_received_crc;

/*! Used to store the current status of a packet handler */
PH_STATUS_t g_ph_status;


void ph_init(uint8_t uart_number, PH_CALLBACKS_t * cb)
{
	hal_uart_init(uart_number, 9600, 8, NO_PARITY);
	
}

void ph_transmit_packet(uint8_t uart_number, uint8_t *tx_packet_body)
{
	
}

PH_STATUS_t ph_get_status(void)
{
	
}



void when_byte_received(uint8_t uart_number, char data, bool parity_error)
{
	if ((uart_number == 1) && (data == 5) && (parity_error == false) )
	{
	
	}
}