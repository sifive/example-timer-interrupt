/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <metal/cpu.h>
#include <metal/timer.h>

#ifndef METAL_WAIT_CYCLE
#define METAL_WAIT_CYCLE 0
#endif

volatile int intr_count = 0;

void metal_riscv_cpu_intc_mtip_handler() {
    intr_count++;

    metal_cpu_disable_timer_interrupt();
}

int main(void) {
    struct metal_cpu cpu = metal_cpu_get(metal_cpu_get_current_hartid());

    metal_cpu_set_mtimecmp(cpu, 0);

    metal_cpu_enable_timer_interrupt();
    metal_cpu_enable_interrupts();

    uint64_t cycles;
    metal_timer_get_cyclecount(metal_cpu_get_current_hartid(), &cycles);
    const uint64_t wait_cycles = cycles + METAL_WAIT_CYCLE;
    do {
        metal_timer_get_cyclecount(metal_cpu_get_current_hartid(), &cycles);
    } while(cycles < wait_cycles);
    
    if (intr_count != 1) {
        return 1; 
    }   
    return 0;
}

