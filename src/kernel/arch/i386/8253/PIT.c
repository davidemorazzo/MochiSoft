#include "dev/8253/PIT.h"
#include "kernel/PIC-8259.h"
#include "kernel/microcode.h"


void pit_set_count(uint8_t ch_addr, uint16_t count){
    outb(ch_addr, count & 0xFF);          // Low-byte
    outb(ch_addr, (count >> 8) & 0xFF);     // High-byte
    return;
}


uint16_t pit_read_count(uint8_t ch_addr){
    uint8_t read_back = 0x00;
    uint8_t ch_select = 0x00;
    uint16_t count = 0;
    switch (ch_addr){
        case PIT_CH0_DATA: ch_select = 0x2; break;
        case PIT_CH1_DATA: ch_select = 0x4; break;
        case PIT_CH2_DATA: ch_select = 0x8; break;
    }
    read_back |= 0xC0 | ch_select | 0x10; 
    outb(PIT_CMD_REG, read_back);
    count = inb(ch_addr);
    count |= inb(ch_addr) << 8;
    return count;
}


void pit_init(uint8_t mode, uint16_t freq){
    /* PIT init */
    uint8_t init_cmd = 0x0;
    // CH0, lobyte/hibyte access, mode rate gen
    init_cmd |= 0x30 | (PIT_MODE_RATE_GEN << 1);
    pit_send_command(init_cmd);
    uint16_t reload_value = PIT_BASE_FREQ / 50; // 100Hz for IRQ0
    pit_set_count(PIT_CH0_DATA, reload_value);

    return;
}


void pit_send_command(uint8_t command){
    outb(PIT_CMD_REG, command);
}

uint8_t pit_readback_ch0(){
    outb(PIT_CMD_REG, 0b11100010);
    return inb(PIT_CH0_DATA);;
}
