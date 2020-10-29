#include "packet_handler.h"
#include "UART_HAL.h"
#include "error.h"

/** Polynomial used to calculate the CRC 
 * (@ref http://www.lammertbies.nl/comm/info/crc-calculation.html)
 */
#define POLY        0x1021
#define CRC_INIT    0xFFFF


/** Starting sequence of a packet */
#define STX1    0x02
#define STX2    0x02
#define STX3    0x02
#define STX4    0x02

/** States of packet collector state machine */
typedef enum{
    /** Waiting for start byte 1 */
    PACKET_COLLECTOR_STATE_STATE_STX_1 = 0,   
    /** Waiting for start byte 2 */
    PACKET_COLLECTOR_STATE_STATE_STX_2,   
    /** Waiting for start byte 3 */
    PACKET_COLLECTOR_STATE_STATE_STX_3,
    /** Waiting for start byte 4 */    
    PACKET_COLLECTOR_STATE_STATE_STX_4, 
    /** Waiting for byte of packet length */    
    PACKET_COLLECTOR_STATE_STATE_LENGTH, 
    /** Collecting data bytes */  
    PACKET_COLLECTOR_STATE_STATE_DATA, 
    /** Waiting for low byte of 16bit CRC */      
    PACKET_COLLECTOR_STATE_STATE_CRC_L, 
    /** Waiting for high byte of 16bit CRC */     
    PACKET_COLLECTOR_STATE_STATE_CRC_H,       
}PACKET_COLLECTOR_STATE_t;


/*! Holds the current state of the packet collector */
uint8_t g_packet_collector_uartnum = NULL;

/*! Structure which holds application callbacks. Callbacks has been set to null */
PH_CALLBACKS_t g_app_callbacks = {NULL ,NULL};

/*! Holds the current state of the packet collector */
PACKET_COLLECTOR_STATE_t g_packet_collector_state;

/*! Holds the length of data to be received via UART */
uint8_t g_rx_buff_idx;

/*! Used to calculate temporary CRC values */
uint16_t g_cal_crc;

/*! Used to store the received packet */
packet_t g_rx_packet_buffer;

/*! Used to keep the transmitting packet */
uint8_t g_tx_buff[MAX_PACKET_SIZE + 7];   //7 is the packet handler overhead

/*! Used to store the CRC error of received packet*/
PACKET_CRC_ERR_STATE_t g_crc_err = NO_CRC_ERROR;

/*! Used to store the current status of a packet handler */
PH_STATUS_t g_ph_status[4];

uint16_t crc16(uint8_t * data_p, uint8_t length);
void when_byte_received(uint8_t uart_number, uint8_t data, bool parity_error);
void when_transmission_complete(uint8_t uart_number);


uint32_t ph_init(uint8_t uart_number, PH_CALLBACKS_t * cb )
{
	uint32_t err = NO_ERROR;
	do
	{
		err = hal_uart_init(uart_number, 9600, 8, NO_PARITY);
		if (err)
		{
			break;
		}
		if (cb->tx_complete_cb ==NULL || cb->rx_complete_cb == NULL)  
		{
			err = APP_CALLBACK_MISSING;
			break;
		}
		//memcpy(&g_app_callbacks, cb, 2);
		g_app_callbacks.rx_complete_cb = cb->rx_complete_cb;
		g_app_callbacks.tx_complete_cb = cb->tx_complete_cb;
		err = hal_uart_set_callbacks(when_byte_received, when_transmission_complete);
		if (err)
		{
			break;
		}
		g_ph_status[uart_number] = PH_STATUS_READY_TO_SEND;
		g_packet_collector_state = PACKET_COLLECTOR_STATE_STATE_STX_1;
		g_rx_buff_idx=0;
		uint8_t STX [4]= {STX1,STX2,STX3,STX4};
		memcpy(g_tx_buff, STX, 4);
	} while (0);
	return err;
}

uint32_t ph_transmit_packet(uint8_t uart_number, packet_t * packet)
{
	uint32_t err = NO_ERROR;
	do
	{
		if (g_ph_status [uart_number] == PH_STATUS_TRANSMITTING)
		{
			err = UART_ERROR_TX_BUSY;
			break;
		}
		g_ph_status [uart_number] = PH_STATUS_TRANSMITTING;
		if (uart_number < 0 || uart_number > 3)
		{
			err=UNSUPPORTED_UART;
			g_ph_status [uart_number] = PH_STATUS_READY_TO_SEND;
			break;
		}
		if (packet->length > MAX_PACKET_SIZE)
		{
			err=PACKET_BODY_LENGTH_ERROR;
			g_ph_status[uart_number] = PH_STATUS_READY_TO_SEND;
			break;
		}
		g_tx_buff[4] = packet->length;
		memcpy(g_tx_buff+5, &(packet->data), packet->length);
		uint8_t *CRC_arr;
		uint16_t CRC_val= crc16(&packet->data, packet->length);
		CRC_arr = &CRC_val;
		memcpy(g_tx_buff+5+(packet->length), CRC_arr, 2);
		err = hal_uart_send(uart_number, &g_tx_buff, (packet->length)+7);
		
		
	} while (0); 
	return err;
}

PH_STATUS_t ph_get_status(uint8_t uart_number)
{
	return g_ph_status[uart_number]; 
}



void when_byte_received(uint8_t uart_number, uint8_t data, bool parity_error)
{
	switch (g_packet_collector_state)
	{
		 case PACKET_COLLECTOR_STATE_STATE_STX_1:
		 {
			 if (data == STX1)
			 {
				 g_packet_collector_uartnum = uart_number;
				 g_crc_err = NO_CRC_ERROR;
				 g_packet_collector_state = PACKET_COLLECTOR_STATE_STATE_STX_2;
			 }
			 break;
		 }
		 case PACKET_COLLECTOR_STATE_STATE_STX_2:
		 {
			 if (data == STX2 && uart_number == g_packet_collector_uartnum)
			 {
				 g_packet_collector_state = PACKET_COLLECTOR_STATE_STATE_STX_3;
			 }
			 else
			 {
				 if (data == STX1)
				 {
					 g_packet_collector_uartnum = uart_number;
					 g_packet_collector_state = PACKET_COLLECTOR_STATE_STATE_STX_2;
				 }
				 else
				 {
					 g_packet_collector_uartnum = NULL;
					 g_packet_collector_state = PACKET_COLLECTOR_STATE_STATE_STX_1;
				 }
			 }
			 break;
		 }
		 case PACKET_COLLECTOR_STATE_STATE_STX_3:
		 {
			 if (data == STX3 && uart_number == g_packet_collector_uartnum)
			 {
				 g_packet_collector_state = PACKET_COLLECTOR_STATE_STATE_STX_4;
			 }
			 else
			 {
				 if (data == STX1)
				 {
					 g_packet_collector_uartnum = uart_number;
					 g_packet_collector_state = PACKET_COLLECTOR_STATE_STATE_STX_2;
				 }
				 else
				 {
					 g_packet_collector_uartnum = NULL;
					 g_packet_collector_state = PACKET_COLLECTOR_STATE_STATE_STX_1;
				 }
			 }
			 break;
		 }
		 case PACKET_COLLECTOR_STATE_STATE_STX_4:
		 {
			 if (data == STX4 && uart_number == g_packet_collector_uartnum)
			 {
				 g_packet_collector_state = PACKET_COLLECTOR_STATE_STATE_LENGTH;
			 }
			 else
			 {
				 if (data == STX1)
				 {
					 g_packet_collector_uartnum = uart_number;
					 g_packet_collector_state = PACKET_COLLECTOR_STATE_STATE_STX_2;
				 }
				 else
				 {
					 g_packet_collector_uartnum = NULL;
					 g_packet_collector_state = PACKET_COLLECTOR_STATE_STATE_STX_1;
				 }
			 }
			 break;
		 }
		 case PACKET_COLLECTOR_STATE_STATE_LENGTH:
		 {
			 if (data < MAX_PACKET_SIZE && data != 0 && uart_number == g_packet_collector_uartnum)
			 {
				 g_rx_buff_idx = 0;
				 g_rx_packet_buffer.length = data;
				 g_packet_collector_state = PACKET_COLLECTOR_STATE_STATE_DATA;
			 }
			 else
			 {
				 if (data == STX1)
				 {
					 g_packet_collector_uartnum = uart_number;
					 g_packet_collector_state = PACKET_COLLECTOR_STATE_STATE_STX_2;
				 }
				 else
				 {
					 g_packet_collector_uartnum = NULL;
					 g_packet_collector_state = PACKET_COLLECTOR_STATE_STATE_STX_1;
				 }
			 }
			 break;
		 }
		 case PACKET_COLLECTOR_STATE_STATE_DATA:
		 {
			 if (g_rx_buff_idx < (g_rx_packet_buffer.length-1) && uart_number == g_packet_collector_uartnum)
			 {
				 g_rx_packet_buffer.data[g_rx_buff_idx++] = data;
			 }
			 else if (g_rx_buff_idx == (g_rx_packet_buffer.length-1) && uart_number == g_packet_collector_uartnum)
			 {
				 g_rx_packet_buffer.data[g_rx_buff_idx] = data;
				 g_rx_buff_idx = 0;
				 g_packet_collector_state = PACKET_COLLECTOR_STATE_STATE_CRC_L;
				 g_cal_crc = crc16(&g_rx_packet_buffer.data, g_rx_packet_buffer.length);
			 }
			 else
			 {
				 if (data == STX1)
				 {
					 g_packet_collector_uartnum = uart_number;
					 g_packet_collector_state = PACKET_COLLECTOR_STATE_STATE_STX_2;
					 
				 }
				 else
				 {
					 g_packet_collector_uartnum = NULL;
					 g_packet_collector_state = PACKET_COLLECTOR_STATE_STATE_STX_1;
				 }
			 }
			 break;
		 }
		 case PACKET_COLLECTOR_STATE_STATE_CRC_L:
		 {
			 if (uart_number == g_packet_collector_uartnum)
			 {
				 if (data != (0x00FF & g_cal_crc))
				 {
					 g_crc_err=CRC_ERROR_DETECTED;
				 }
				 g_packet_collector_state = PACKET_COLLECTOR_STATE_STATE_CRC_H;
			 }
			 else
			 {
				 if (data == STX1)
				 {
					 g_packet_collector_uartnum = uart_number;
					 g_packet_collector_state = PACKET_COLLECTOR_STATE_STATE_STX_2;
				 }
				 else
				 {
					 g_packet_collector_uartnum = NULL;
					 g_packet_collector_state = PACKET_COLLECTOR_STATE_STATE_STX_1;
				 }
			 }
			 break;
		 }
		 case PACKET_COLLECTOR_STATE_STATE_CRC_H:
		 {
			 if (uart_number == g_packet_collector_uartnum)
			 {
				 if (data != ((0xFF00 & g_cal_crc)>>8))
				 {
					 g_crc_err=CRC_ERROR_DETECTED;
				 }
				 g_packet_collector_state = PACKET_COLLECTOR_STATE_STATE_STX_1;
				 if (g_app_callbacks.rx_complete_cb != NULL)
				 {
					 g_app_callbacks.rx_complete_cb(uart_number,&g_rx_packet_buffer,g_crc_err);
				 }
				 g_crc_err=NO_CRC_ERROR;
			 }
			 else
			 {
				 if (data == STX1)
				 {
					 g_packet_collector_uartnum = uart_number;
					 g_packet_collector_state = PACKET_COLLECTOR_STATE_STATE_STX_2;
				 }
				 else
				 {
					 g_packet_collector_uartnum = NULL;
					 g_packet_collector_state = PACKET_COLLECTOR_STATE_STATE_STX_1;
				 }
			 }
			 break;
		 }
	}
}

void when_transmission_complete(uint8_t uart_number)
{
	g_ph_status[uart_number] = PH_STATUS_READY_TO_SEND;
	g_app_callbacks.tx_complete_cb(uart_number,1);
}

uint16_t crc16(uint8_t* data_p, uint8_t length)
{
	unsigned char x;
	uint16_t crc = CRC_INIT;

	while (length--)
	{
		x = crc >> 8 ^ *data_p++;
		x ^= x>>4;
		crc = (crc << 8) ^ ((uint16_t)(x << 12)) ^ ((uint16_t)(x <<5)) ^ ((uint16_t)x);
	}
	return crc;
}