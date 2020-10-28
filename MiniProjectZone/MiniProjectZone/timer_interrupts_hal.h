#include "GPIO_HAL.h"
#include "error.h"

typedef enum {TIMER_1, TIMER_3, TIMER_4, TIMER_5} timer_number;

typedef void (*timer_int_callback_t)(timer_number timer);   

uint32_t init_timer_interrupts(timer_number timer, timer_int_callback_t tic);

