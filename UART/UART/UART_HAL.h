#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <avr/io.h>
#include <avr/interrupt.h>

enum parity_types{EVEN, ODD,NO_PARITY};

typedef void (*on_byte_received)(uint8_t uart_number, char data, bool parity_error);
typedef void (*on_transmission_complete)(uint8_t uart_number);

/**
 * @brief Function to initialize a UART system.
 *
 * Use this function to initialize a UART system 0/1/2/3. Can not initialize as a synchronize port. Use 1 stop bit. 
 * Can be used only for the boudrate values given below.  
 *
 * @note The "hal_uart_set_callbacks" should be called next to this function before other functions
 * to set the call back functions of the corresponding UART system (0/1/2/3).
 *
 * @param uart_number - UART system ID (0/1/2/3)
 *		  boudrate	  - boudrate of the UART system (2400/4800/9600/19200)
 *		  frame_size  - can use 5/6/7/8 as the size of a transmitted or received frames.
 *		  parity_type - can give EVEN / ODD / NO_PARITY as the parity type
 *
 * @return The error code. 
 */
uint32_t hal_uart_init(uint8_t uart_number, uint32_t baudrate, uint8_t frame_size, uint8_t parity_type);

/**
 * @brief Function to give the call-back functions for a particular UART system.
 *
 * Use this function to give the call-back functions for a UART system 0/1/2/3. 
 *
 * @note This function should be called right next to the "hal_uart_init" function before other functions
 * to initialize the corresponding UART system (0/1/2/3).
 *
 * @param obr - the call-back function when interrupt occur due to complete byte reception.
 *		  otc - the call-back function when interrupt occur due to complete byte transmission.
 *
 * @return The error code. 
 */
uint32_t hal_uart_set_callbacks(on_byte_received obr, on_transmission_complete otc);

/**
 * @brief Function to send a data steam using an UART system.
 *
 * Use this function to send data stream using "Tx" pin of an UART system 0/1/2/3. 
 *
 * @note To call this function, the corresponding UART system should 
 * already been initialized using "hal_uart_init" function.
 *
 * @param uart_number - UART system ID (0/1/2/3)
 *		  tx_buffer	  - data stream. (a list of data bytes)
 *		  length	  - length of tx_buffer (data list)
 *
 * @return The error code. 
 */
uint32_t hal_uart_send(uint8_t uart_number, uint8_t *tx_buffer, uint8_t length);

