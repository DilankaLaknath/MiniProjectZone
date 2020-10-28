#include "timer_interrupts_hal.h"
#include <util/delay.h>


typedef enum  {WGMn0, WGMn1, COMnC0, COMnC1, COMnB0, COMnB1, COMnA0, COMnA1} TCCRnA_bits;
typedef enum {CSn0, CSn1, CSn2, WGMn2, WGMn3, ICESn = 6, ICNCn} TCCRnB_bits;
typedef enum {TOIEn, OCIEnA, OCIEnB, OCIEnC, ICIEn = 5} TIMSKn_bits;

timer_int_callback_t g_timer_int_callback = NULL;

typedef struct {
	volatile uint8_t *TCCRnB ;
	volatile uint8_t *TCCRnA ;
	volatile uint8_t *TIMSKn ;
	volatile uint16_t *OCRnA ;
	}interrupt_registers;


interrupt_registers g_interrupt_registers; //global structure of interrupt registers is defined

void select_timer_registers(timer_number timer);
void set_values_of_timer_registers();


uint32_t init_timer_interrupts(timer_number timer, timer_int_callback_t tic)
{
	uint32_t err = 0;
	g_timer_int_callback = tic;
	do 
	{
		if ((timer != TIMER_1) && (timer != TIMER_3) && (timer != TIMER_4) && (timer != TIMER_5))
		{
			err = WRONG_TIMER;
			break;
		}
		if (g_timer_int_callback == NULL)
		{
			err = NULL_ERROR;
			break;
		}
		sei(); //enable global interrupts
		select_timer_registers(timer);
		set_values_of_timer_registers();
				
	} while (0);

	return err;	
}

void select_timer_registers(timer_number timer)
{
	
	switch (timer){
		case TIMER_1:
			g_interrupt_registers.TCCRnA = &TCCR1A;
			g_interrupt_registers.TCCRnB = &TCCR1B;
			g_interrupt_registers.TIMSKn = &TIMSK1;
			g_interrupt_registers.OCRnA = &OCR1A;
			
			break;
			
		case TIMER_3:
			g_interrupt_registers.TCCRnA = &TCCR3A;
			g_interrupt_registers.TCCRnB = &TCCR3B;
			g_interrupt_registers.TIMSKn = &TIMSK3;	
			g_interrupt_registers.OCRnA = &OCR3A;	
			break;
			
		case TIMER_4:
			g_interrupt_registers.TCCRnA = &TCCR4A;
			g_interrupt_registers.TCCRnB = &TCCR4B;
			g_interrupt_registers.TIMSKn = &TIMSK4;
			g_interrupt_registers.OCRnA = &OCR4A;
			break;
		
		case TIMER_5:
			g_interrupt_registers.TCCRnA = &TCCR5A;
			g_interrupt_registers.TCCRnB = &TCCR5B;
			g_interrupt_registers.TIMSKn = &TIMSK5;
			g_interrupt_registers.OCRnA = &OCR5A;
			break;
	}
}

void set_values_of_timer_registers()  //to toggle at frequency of 2Hz OCRnA == 31250 and N == 256;
{
	//to set the CRC mode
	*g_interrupt_registers.TCCRnA &= ~(1<<WGMn1) & ~(1<<WGMn2);
	*g_interrupt_registers.TCCRnB |= (1 << WGMn2);
	*g_interrupt_registers.TCCRnB &= ~(1 << WGMn3);
	
	
	//to set N = 256
	*g_interrupt_registers.TCCRnB |= (1 << CSn2);
	*g_interrupt_registers.TCCRnB &= ~(1 << CSn1) & ~(1 << CSn0);
	
	//to set interrupt_delay to 0.5s 
	*(g_interrupt_registers.OCRnA) = 31250;
	
	
	//set interrupts
	*g_interrupt_registers.TIMSKn |= (1 << OCIEnA); // other 2 compare interrupts and TOV interrupt are neglected.		
}


//////////////////////interrupts callings



ISR(TIMER1_COMPA_vect)
{
	g_timer_int_callback(TIMER_1);
}

ISR(TIMER3_COMPB_vect)
{
	g_timer_int_callback(TIMER_3);
}

ISR(TIMER4_COMPC_vect)
{
	g_timer_int_callback(TIMER_4);
}

ISR(TIMER5_COMPA_vect)
{
	g_timer_int_callback(TIMER_5);
}




