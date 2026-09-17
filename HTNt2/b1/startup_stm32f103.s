.syntax unified
.cpu cortex-m3
.thumb

.global _estack
.global Reset_Handler

.extern main

/* Vector Table */
.section .isr_vector, "a", %progbits

.word _estack
.word Reset_Handler

.word NMI_Handler
.word HardFault_Handler
.word MemManage_Handler
.word BusFault_Handler
.word UsageFault_Handler

.word 0
.word 0
.word 0
.word 0

.word SVC_Handler
.word DebugMon_Handler
.word 0
.word PendSV_Handler
.word SysTick_Handler


/* Reset Handler */
.section .text.Reset_Handler
.type Reset_Handler, %function

Reset_Handler:

    /* Copy .data from Flash to RAM */
    ldr r0, =_sidata
    ldr r1, =_sdata
    ldr r2, =_edata

copy_data:
    cmp r1, r2
    bcc copy_loop
    b init_bss

copy_loop:
    ldr r3, [r0]
    str r3, [r1]
    adds r0, r0, #4
    adds r1, r1, #4
    b copy_data


/* Clear .bss */
init_bss:

    ldr r1, =_sbss
    ldr r2, =_ebss
    movs r3, #0

clear_bss:
    cmp r1, r2
    bcc clear_loop
    b call_main

clear_loop:
    str r3, [r1]
    adds r1, r1, #4
    b clear_bss


/* Call main */
call_main:

    bl main

loop:
    b loop


.size Reset_Handler, .-Reset_Handler


/* Default Handler */
.section .text.Default_Handler
.type Default_Handler, %function

Default_Handler:
    b Default_Handler


/* Weak Interrupt Handlers */

.weak NMI_Handler
.set NMI_Handler, Default_Handler

.weak HardFault_Handler
.set HardFault_Handler, Default_Handler

.weak MemManage_Handler
.set MemManage_Handler, Default_Handler

.weak BusFault_Handler
.set BusFault_Handler, Default_Handler

.weak UsageFault_Handler
.set UsageFault_Handler, Default_Handler

.weak SVC_Handler
.set SVC_Handler, Default_Handler

.weak DebugMon_Handler
.set DebugMon_Handler, Default_Handler

.weak PendSV_Handler
.set PendSV_Handler, Default_Handler

.weak SysTick_Handler
.set SysTick_Handler, Default_Handler
