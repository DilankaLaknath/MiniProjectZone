#include "packet_handler.h"
#include "UART_HAL.h"
<<<<<<< HEAD
#include "error.h"
#include <util/delay.h>

/**
 * @brief
 * This function initialize the packet handler. 
 * <BR>
 * It uses UART1 for packet transmission and reception. Packet reception is done  
 * inside UART RX interrupt. After a valid packet received, it notifies the  
 * application via rx_complete_cb. For the [callback table](@ref PH_CALLBACKS_t.  
 * It has to be called before any other application/module/driver related functions.
 *
 * @param
 * cb - pointer to a structure where holds the callback functions to be called at
 *      different occasions. For the [callback table](@ref PH_CALLBACKS_t)
 *
 * @return
 * None
 */
uint32_t ph_init(uint8_t uart_number, transmit_cmplt_cb_t txcb, receive_cmplt_cb_t  rxcb)
=======

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
>>>>>>> master
{
	
}

<<<<<<< HEAD
/**
 * @brief
 * This function transmit a packet. It adds packet headers and CRC and then transmit. 
 *
 * @return
 * None
 */
uint32_t ph_transmit_packet(uint8_t uart_number, packet_t * packet)
=======
PH_STATUS_t ph_get_status(void)
>>>>>>> master
{
	
}

<<<<<<< HEAD
/**
 * @brief
 * This function returns the current status of the packet handler. 
 *
 * @return
 * Status of the packet handler.
 * For the [status](@ref PH_STATUS_t)
 */
PH_STATUS_t ph_get_status(uint8_t uart_number)
{
	
=======


void when_byte_received(uint8_t uart_number, char data, bool parity_error)
{
	if ((uart_number == 1) && (data == 5) && (parity_error == false) )
	{
	
	}
>>>>>>> master
}