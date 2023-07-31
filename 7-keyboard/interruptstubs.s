
.set IRQ_BASE, 0x20

.section .text

/**
 * 为了使用 interrupts.cpp 中的函数，又不想改成 extern "C"
 * mio@ubt_vm:~/write_os/7-interrupt$ nm interrupts.o
 *          U _GLOBAL_OFFSET_TABLE_
 * 00000000 T __x86.get_pc_thunk.ax
 *          U _Z6printfPc
 * 00000000 T _ZN16InterruptManager15handleInterruptEhj
*/
.extern _ZN16InterruptManager15handleInterruptEhj

.global _ZN16InterruptManager22IgnoreInterruptRequestEv

.macro HandleException num
.global _ZN16InterruptManager16HandleException\num\()Ev
_ZN16InterruptManager16HandleException\num\()Ev:
    movb $\num, (interruptnumber)
    jmp int_bottom
.endm

.macro HandleInterruptRequest num
.global _ZN16InterruptManager26HandleInterruptRequest\num\()Ev
_ZN16InterruptManager26HandleInterruptRequest\num\()Ev:
    movb $\num + IRQ_BASE, (interruptnumber)
    jmp int_bottom
.endm

HandleInterruptRequest 0x00
HandleInterruptRequest 0x01

int_bottom:
    
    pusha
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs

    pushl %esp
    push (interruptnumber)
    call _ZN16InterruptManager15handleInterruptEhj
    # addl $5, %esp #不用执行这个
    movl %eax, %esp

    popl %gs
    popl %fs
    popl %es
    popl %ds
    popa

_ZN16InterruptManager22IgnoreInterruptRequestEv:

    iret

.data
    interruptnumber: .byte 0

