/* Novembre 2024 - Davide Morazzo

Libreria per gestire il dispositivo PIT (Programmable Interrupt 
Controller) modello 8253 o 8254.
Fonti: 
    - https://wiki.osdev.org/Programmable_Interval_Timer
    - https://www.intel.com/content/dam/www/public/us/en/documents/manuals/64-ia-32-architectures-software-developer-vol-3a-part-1-manual.pdf

I/O port     Usage
0x40         Channel 0 data port (read/write) => Unico canale utilizzabile
0x41         Channel 1 data port (read/write)
0x42         Channel 2 data port (read/write)
0x43         Mode/Command register (write only, a read is ignored)

*/
#ifndef INCLUDE_DEV_8253_PIT
#define INCLUDE_DEV_8253_PIT 1

#include <stdint.h>

#define PIT_CH0_DATA 0x40
#define PIT_CH1_DATA 0x41
#define PIT_CH2_DATA 0x42
#define PIT_CMD_REG  0x43

#define PIT_MODE_INT_ON_TERMINAL_COUNT 0
#define PIT_MODE_HW_RETRIGG_ONESHOT 1
#define PIT_MODE_RATE_GEN 2
#define PIT_MODE_SQUARE_WAVE_GEN 3
#define PIT_MODE_SW_TRIGG_STROBE 4
#define PIT_MODE_HW_TRIGG_STROBE 5

#define PIT_BASE_FREQ 1193182  // Hz

void pit_set_count(uint8_t ch_addr, uint16_t count);
uint16_t pit_read_count(uint8_t ch_addr);
void pit_init();
void pit_send_command(uint8_t command);

#endif


