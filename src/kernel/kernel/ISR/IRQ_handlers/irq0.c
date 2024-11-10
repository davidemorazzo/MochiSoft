/* Novembre 2024 - Davide Morazzo

Handler per Interrupt IRQ0
Indirizzo 0x20 (32)
*/
#include "time.h"
#include <stdint.h>
#include "kernel/PIC-8259.h"
#include "dev/RTC.h"
#include "kernel/kstdio.h"

volatile int count_tmp = 0;
void irq0(){
	// uint32_t isr_reg = pic_get_isr();
    // uint32_t irr_reg = pic_get_irr();
    // kprint("IRQ0!\n ISR: 0x%X \n IRR: 0x%X\n", &isr_reg, &irr_reg);
    count_tmp++;
    struct tm now;
    // rtc_get_time(&now);
    // uint64_t t = (uint64_t)mktime(&now);
    if (count_tmp >=100){
        count_tmp = 0;
        // kprint("<%s> IRQ#0\n", asctime(&now));
    }
    PIC_sendEOI(0);
}