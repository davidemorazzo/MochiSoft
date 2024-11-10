/* Novembre 2024 - Davide Morazzo

Handler per Interrupt IRQ8 (RTC)
Indirizzo 0x28 (40)
*/
#include "kernel/PIC-8259.h"
#include "dev/RTC.h"
#include "kernel/kstdio.h"
#include "time.h"
#include "kernel/kglobals.h"

void irq8(){
	struct tm now;
	rtc_get_time(&now);
	SYS_TIME_RTC = mktime(&now);
	kprint("<%s> IRQ#8\n", asctime(&now));

	// Clear register C to ack the interrupt to the RTC
	outb(RTC_ADDRESS, 0x0C);
	inb(RTC_DATA);
    PIC_sendEOI(8);
}