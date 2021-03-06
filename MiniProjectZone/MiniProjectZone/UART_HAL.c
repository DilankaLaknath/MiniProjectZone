#include "UART_HAL.h"
#include "error.h"
#include <stdbool.h>

typedef enum {MPCMn, U2Xn, UPEn, DORn, FEn, UDREn, TXCn, RXCn} UCSRnA_bits ;
typedef enum {TXB8n, RXB8n, UCSZn2, TXENn, RXENn, UDRIEn, TXCIEn, RXCIEn} UCSRnB_bits;
typedef enum {UCPOLn, UCSZn0, UCSZn1, USBSn, UPMn0, UPMn1, UMSELn0, UMSELn1} UCSRnC_bits; 

on_byte_received g_on_byte_received = NULL;
on_transmission_complete g_on_transmission_complete = NULL;

volatile uint8_t *UCSRnA = NULL;
volatile uint8_t *UCSRnB = NULL;
volatile uint8_t *UCSRnC = NULL;
volatile uint8_t *UBRRnL = NULL;
volatile uint8_t *UBRRnH = NULL;
volatile uint8_t *UDRn   = NULL;

#define INT_TX_BUFF_LEN	 20
uint8_t g_internal_tx_buff[INT_TX_BUFF_LEN];
bool g_tx_busy = false;
uint8_t g_bytes_to_be_tx;
uint8_t g_tx_buff_idx;


/////////////////////////Function definitions////////////////////

/**
 * @brief Function to call when an transmit buffer empty interrupt occurs.
 *
 * Use this function to add the content in the INT_TX_BUFF_LEN byte transmit buffer we made into the 1 byte transmit 
 * buffer on the board one by one as soon as the transmit buffer is ready to receive more data until all the data 
 * added to the transmit buffer we made has been transfered.
 *
 * @param uart_number - UART system ID (0/1/2/3)
 *
 * @return None
 */
void on_trasmit_buffer_empty (uint8_t uart_number);


/**
 * @brief Function to map the UART registers.
 *
 * Use this function to map the UART registers relavant to a UART system 0/1/2/3 in to the global register used.
 *
 * @param uart_number - UART system ID (0/1/2/3)
 *
 * @return NO_ERROR - no error in the input parameters.  
 *
 * @return UNSUPPORTED_UART - UART number is not 0/1/2/3.
 */
uint32_t map_uart_registers(uint8_t uart_number);


/**
 * @brief Function to set the baudrate.
 *
 * @note Should be called after a map_uart_registers function to make sure we are writing to the correct registers.
 *
 * @param baudrate - baudrate of the UART system (2400/4800/9600/19200)
 *
 * @return NO_ERROR - no error in the input parameters. 
 *
 * @return UNSUPPORTED_BAUDRATE - baudrate is not 2400/4800/9600/19200. 
 */
uint32_t set_baudrate_registers(uint32_t baudrate);


/**
 * @brief Function to set the frame size.
 *
 * @note Should be called after a map_uart_registers function to make sure we are writing to the correct registers.
 *
 * @param frame_size  - can use 5/6/7/8 as the size of a transmitted or received frames.
 *
 * @return NO_ERROR - no error in the input parameters. 
 *
 * @return UNSUPPORTED_FRAME_SIZE - frame size is not 5/6/7/8.
 */
uint32_t set_frame_size(uint8_t frame_size);


/**
 * @brief Function to select the parity type to be used.
 *
 * @note Should be called after a map_uart_registers function to make sure we are writing to the correct registers.
 *
 * @param parity_type - can give EVEN / ODD / NO_PARITY as the parity type
 *
 * @return None
 */
void set_parity_register_values(parity_types parity_type);



/////////////////////////Public Functions////////////////////

uint32_t hal_uart_init(uint8_t uart_number, uint32_t baudrate, uint8_t frame_size, parity_types parity_type)
{	
	uint32_t err = NO_ERROR;
	do 
	{
		sei();  // to enable global interrupts
		
		if (map_uart_registers(uart_number))  //choose correct register set for the given UART number
		{
			err=UNSUPPORTED_UART;
			break;
		}
		
		if (set_baudrate_registers(baudrate))  // to fill the UBRRn register according to the given baudrate
		{
			err=UNSUPPORTED_BAUDRATE;
			break;
		}
		
		if (set_frame_size(frame_size)) // set register values belongs to frame size configuration
		{
			err=UNSUPPORTED_FRAME_SIZE;
			break;
		}
		
		set_parity_register_values(parity_type);
		
		(*UCSRnC) &= ~(1<<UMSELn1) & ~(1<<UMSELn0); // set mode to asynchronous UART
		(*UCSRnC) &= ~(1<<USBSn); // to use only 1 stop bit
		(*UCSRnC) &= ~(1<<UCPOLn); //don't use clock parity as asynchronous mode is used
		
		(*UCSRnA) |= (1<<TXCn);  // to clear the flag should give 1
		(*UCSRnA) &= ~(1<<FEn) & ~(1<<DORn) & ~(1<<UPEn) & ~(1<<U2Xn) & ~(1<<MPCMn);
		
		(*UCSRnB) |= (1<<RXCIEn); //enable Rx complete and Tx complete interrupts
		(*UCSRnB) &= ~(1<<UDRIEn) & ~(1<<TXCIEn); //disable Tx buffer empty interrupt
		(*UCSRnB) |= (1<<RXENn) | (1<<TXENn); // enable UART transmitter and receiver
		
	} while (0);
		
	return err;
}

uint32_t hal_uart_set_callbacks(on_byte_received obr, on_transmission_complete otc)
{	
	uint32_t err = NO_ERROR;
	do 
	{
		if (obr == NULL || otc == NULL)
		{
			err=CALLBACK_MISSING;
			break;
		}
		g_on_byte_received = obr;
		g_on_transmission_complete = otc;
	}while (0);
		
	return err;
}

uint32_t hal_uart_send(uint8_t uart_number, uint8_t *tx_buffer, uint8_t length)
{	
	uint32_t err = NO_ERROR;
	do 
	{	
		if(length == 0)
		{
			err = TX_ARRAY_LENGTH_ZERO_WARNING;
			if (g_on_transmission_complete != NULL)
			{
				g_on_transmission_complete(uart_number);
			}
			break;
		}		
		if(g_tx_busy)
		{
			err = UART_ERROR_TX_BUSY;
			break;
		}
		if (map_uart_registers(uart_number))
		{
			err = UNSUPPORTED_UART;
			break;
		}
		if (INT_TX_BUFF_LEN < length)
		{
			err = TX_ARRAY_ERROR;
			break;
		}
		g_tx_busy = true;
		memcpy(g_internal_tx_buff, tx_buffer, length);
		g_bytes_to_be_tx = length;
		g_tx_buff_idx = 0;
		on_trasmit_buffer_empty (uart_number);
		(*UCSRnB) |= (1<<UDRIEn) | (1<<TXCIEn); //enable Tx buffer empty interrupt
	} while (0);
	
	return err;
}


/////////////////////////Private Functions////////////////////

void on_trasmit_buffer_empty (uint8_t uart_number)
{	
	cli();
	map_uart_registers(uart_number);
	if (g_tx_buff_idx < g_bytes_to_be_tx  && g_tx_busy)
	{	
		if ( (*UCSRnA) & (1<<UDREn))
		{
			*UDRn = g_internal_tx_buff[g_tx_buff_idx++];
			(*UCSRnA) &= ~(1<<UDREn);
		}
	}
	else if (g_tx_buff_idx == g_bytes_to_be_tx  && g_tx_busy)
	{
		(*UCSRnB) &= ~(1<<UDRIEn); //disable Tx buffer empty interrupt
	}
	sei();
}

uint32_t map_uart_registers(uint8_t uart_number)
{
	uint32_t err = NO_ERROR;
	do 
	{	
		if (uart_number > 3)
		{
			err = UNSUPPORTED_UART;
			break;
		}
		
		switch (uart_number){
			case 0:
			{
				UCSRnA = &UCSR0A;
				UCSRnB = &UCSR0B;
				UCSRnC = &UCSR0C;
				UBRRnL = &UBRR0L;
				UBRRnH = &UBRR0H;
				UDRn = &UDR0;
				break;
			}
			case 1:
			{
				UCSRnA = &UCSR1A;
				UCSRnB = &UCSR1B;
				UCSRnC = &UCSR1C;
				UBRRnL = &UBRR1L;
				UBRRnH = &UBRR1H;
				UDRn = &UDR1;
				break;
			}
			case 2:
			{
				UCSRnA = &UCSR2A;
				UCSRnB = &UCSR2B;
				UCSRnC = &UCSR2C;
				UBRRnL = &UBRR2L;
				UBRRnH = &UBRR2H;
				UDRn = &UDR2;
				break;
			}
			case 3:
			{
				UCSRnA = &UCSR3A;
				UCSRnB = &UCSR3B;
				UCSRnC = &UCSR3C;
				UBRRnL = &UBRR3L;
				UBRRnH = &UBRR3H;
				UDRn = &UDR3;
				break;
			}
		}
	} while (0);
		
	return err;
}

uint32_t set_baudrate_registers(uint32_t baudrate)
{	
	uint32_t err = NO_ERROR;
	uint16_t regValue = 0;
	
	do
	{	
		if (baudrate != 2400 && baudrate != 4800 && baudrate != 9600 && baudrate != 19200)
		{
			err = UNSUPPORTED_BAUDRATE;
			break;
		}
		
		//considering 16MHz clock rate
		switch (baudrate){
			case 2400   :
			{
				regValue = 416;
				break;
			}
			case 4800   :
			{
				regValue = 207;
				break;
			}
			case 9600   :
			{
				regValue = 103;
				break;
			}
			case 19200	:
			{
				regValue = 51;
				break;
			}
			default		:
			{
				regValue = 103;
				break;  // baudrate = 9600
			}
		}
		
		*UBRRnL = (regValue & 0x00FF);   // set the upper 8 bits
		*UBRRnH = ((regValue & 0x0F00)>>8); // set the lower 8 bits
	}while(0);
	
	return err;
}

uint32_t set_frame_size(uint8_t frame_size)  // can not configure for 9 bit version.
{	
	uint32_t err = NO_ERROR;
	do
	{	
		if (frame_size > 8 || frame_size < 5)
		{
			err = UNSUPPORTED_FRAME_SIZE;
			break;
		}
		switch (frame_size){
			case 5 :
			{
				(*UCSRnB) &= ~(1<<UCSZn2);
				(*UCSRnC) &= ~(1<<UCSZn1);
				(*UCSRnC) &= ~(1<<UCSZn0);
				break;
			}
			case 6 :
			{
				(*UCSRnB) &= ~(1<<UCSZn2);
				(*UCSRnC) &= ~(1<<UCSZn1);
				(*UCSRnC) |=  (1<<UCSZn0);
				break;
			}
			case 7 :
			{
				(*UCSRnB) &= ~(1<<UCSZn2);
				(*UCSRnC) |=  (1<<UCSZn1);
				(*UCSRnC) &= ~(1<<UCSZn0);
				break;
			}
			case 8 :
			{
				(*UCSRnB) &= ~(1<<UCSZn2);
				(*UCSRnC) |=  (1<<UCSZn1);
				(*UCSRnC) |=  (1<<UCSZn0);
				break;
			}
			default:
			{
				(*UCSRnB) &= ~(1<<UCSZn2);
				(*UCSRnC) |=  (1<<UCSZn1);
				(*UCSRnC) |=  (1<<UCSZn0);
				break; // default is 8 bit version
			}
		}
	} while (0);
	
	return err;
}

void set_parity_register_values(parity_types parity_type)
{
	switch (parity_type){
		case (NO_PARITY): 
		{
			(*UCSRnC) &= ~(1<<UPMn1);	 
			(*UCSRnC) &= ~(1<<UPMn0);	
			break;
		}
		case (EVEN)		: 
		{
			(*UCSRnC) |=  (1<<UPMn1);	 
			(*UCSRnC) &= ~(1<<UPMn0);	
			break;
		}
		case (ODD)		: 
		{
			(*UCSRnC) |=  (1<<UPMn1);  
			(*UCSRnC) |=  (1<<UPMn0);	
			break;
		}
		default			: 
		{
			(*UCSRnC) &= ~(1<<UPMn1);	 
			(*UCSRnC) &= ~(1<<UPMn0);	
			break;  //default is NO_PARITY
		}
	}
}

////////////////////////////received complete interrupt callings/////////////
ISR(USART0_RX_vect)
{
	bool parity_error = (UCSR0A & (1<<UPE0))>>UPE0;
	if (g_on_byte_received != NULL)
	{
		g_on_byte_received(0, UDR0, parity_error);
	}
}

ISR(USART1_RX_vect)
{
	bool parity_error = (UCSR1A & (1<<UPE1))>>UPE1;
	if (g_on_byte_received != NULL)
	{
		g_on_byte_received(1, UDR1, parity_error);
	}
}

ISR(USART2_RX_vect)
{
	bool parity_error = (UCSR2A & (1<<UPE2))>>UPE2;
	if (g_on_byte_received != NULL)
	{
		g_on_byte_received(2, UDR2, parity_error);
	}
}

ISR(USART3_RX_vect)
{
	bool parity_error = (UCSR3A & (1<<UPE3))>>UPE3;
	if (g_on_byte_received != NULL) 
	{
		g_on_byte_received(3, UDR3, parity_error);
	}
}

/////////////////////////transmission complete interrupt callings //////////////
ISR(USART0_TX_vect)
{
	g_tx_buff_idx=0;
	g_tx_busy=false;
	map_uart_registers(0);
	(*UCSRnB) &= ~(1<<TXCIEn);
	if (g_on_transmission_complete != NULL)
	{
		g_on_transmission_complete(0);
	}
}

ISR(USART1_TX_vect)
{
	g_tx_buff_idx=0;
	g_tx_busy=false;
	map_uart_registers(1);
	(*UCSRnB) &= ~(1<<TXCIEn);
	if (g_on_transmission_complete != NULL)
	{
		g_on_transmission_complete(1);
	}
}

ISR(USART2_TX_vect)
{	
	g_tx_buff_idx=0;
	g_tx_busy=false;
	map_uart_registers(2);
	(*UCSRnB) &= ~(1<<TXCIEn);
	if (g_on_transmission_complete != NULL)
	{
		g_on_transmission_complete(2);
	}
}

ISR(USART3_TX_vect)
{
	g_tx_buff_idx=0;
	g_tx_busy=false;
	map_uart_registers(3);
	(*UCSRnB) &= ~(1<<TXCIEn);
	if (g_on_transmission_complete != NULL)
	{		
		g_on_transmission_complete(3);
	}
}


/////////////////////////transmit buffer empty interrupt callings //////////////
ISR(USART0_UDRE_vect)
{	
	on_trasmit_buffer_empty (0);
}

ISR(USART1_UDRE_vect)
{
	on_trasmit_buffer_empty (1);
}

ISR(USART2_UDRE_vect)
{
	on_trasmit_buffer_empty (2);
}

ISR(USART3_UDRE_vect)
{
	on_trasmit_buffer_empty (3);
}