#include "linked_list/list.h"
#include "kernel/proc.h"
#include "kernel/threads.h"
#include "kernel/idt.h"

void sched_init(){
	__proc_list = list_new();
	__proc_active = NULL;
	extern void sched_isr();
	InterruptDescriptor32 genericIsrDesc;
    SET_IT_VEC(genericIsrDesc, sched_isr, 100); //FIXME: mettere quello giusto
}