#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <avr/io.h>
#include <avr/interrupt.h>

typedef enum {EVEN, ODD,NO_PARITY}parity_types;

typedef void (*on_byte_received)(uint8_t uart_number, char data, bool parity_error);
typedef void (*on_transmission_complete)(uint8_t uart_number);

/**
 * @brief Function to initialize a UART system.
 *
 * Use this function to initialize a UART system 0/1/2/3. Can not initialize as a synchronize port. Use 1 stop bit. 
 * Can be used only for the baudrate values given below.  
 *
 * @note The "hal_uart_set_callbacks" should be called next to this function before other functions
 * to set the call back functions of the corresponding UART system (0/1/2/3).
 *
 * @param uart_number - UART system ID (0/1/2/3)
 *
 * @param baudrate	  - baudrate of the UART system (2400/4800/9600/19200)
 *
 * @param frame_size  - can use 5/6/7/8 as the size of a transmitted or received frames.
 *
 * @param parity_type - can give EVEN / ODD / NO_PARITY as the parity type
 *
 * @return NO_ERROR               - no error in the input parameters.
 *
 * @return UNSUPPORTED_UART       - UART number is not 0/1/2/3.
 *
 * @return UNSUPPORTED_BAUDRATE   - baudrate is not 2400/4800/9600/19200.
 *
 * @return UNSUPPORTED_FRAME_SIZE - frame size is not 5/6/7/8.
 */
uint32_t hal_uart_init(uint8_t uart_number, uint32_t baudrate, uint8_t frame_size, parity_types parity_type);

/**
 * @brief Function to give the call-back functions for a particular UART system.
 *
 * Use this function to give the call-back functions for a UART system 0/1/2/3. 
 *
 * @note This function should be called right next to the "hal_uart_init" function before other functions
 * to initialize the corresponding UART system (0/1/2/3).
 *
 * @param obr - the call-back function when interrupt occur due to complete byte reception.
 *		  
 * @param otc - the call-back function when interrupt occur due to complete byte transmission.
 *
 * @return NO_ERROR         - no error in the input parameters.
 *
 * @return CALLBACK_MISSING - one or both callback functions are missing in the input parameters.
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
 *		 
 * @param tx_buffer	  - data stream. (a list of data bytes)
 *		 
 * @param length	  - length of tx_buffer (should be less than 20)
 *
 * @return NO_ERROR                     - no error in the input parameters.
 *
 * @return UNSUPPORTED_UART             - UART number is not 0/1/2/3.
 *
 * @return TX_ARRAY_LENGTH_ZERO_WARNING - array length zero warning
 *
 * @return UART_ERROR_TX_BUSY           - UART is still busy transmitting a previous data.
 *
 * @return TX_ARRAY_ERROR               - array length greater than the transmit buffer we have.
 */
uint32_t hal_uart_send(uint8_t uart_number, uint8_t *tx_buffer, uint8_t length);

