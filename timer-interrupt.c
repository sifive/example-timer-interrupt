/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <metal/cpu.h>

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
    
    if (intr_count != 1) {
        return 1; 
    }   
    return 0;
}

