/*! Structure which holds application callbacks. Callbacks has been set to null */
PH_CALLBACKS_t g_app_callbacks = {0 ,0};

/*! Holds the current state of the packet collector */
PACKET_COLLECTOR_STATE_t g_packet_collector_state;

/*! Holds the newest byte received via UART */
uint8_t g_rx_Byte;

/*! Used to calculate temporary CRC values */
uint16_t g_cal_crc;

/*! Used to store the received packet */
SERIAL_RX_PACKET_t g_rx_packet;

/*! Used to keep the transmitting packet */
uint8_t g_tx_buff[sizeof(SERIAL_TX_PACKET_t) + 8];   //8 is the packet handler overhead

/*! Used to store the received CRC */
uint16_t g_received_crc;

/*! Used to store the current status of a packet handler */
PH_STATUS_t g_ph_status;






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