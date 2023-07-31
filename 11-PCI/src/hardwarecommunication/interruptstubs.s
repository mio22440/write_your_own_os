
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
// .extern _ZN16InterruptManager15handleInterruptEhj
/* 整理结构之后用了 namespace 这里得改 */
.extern _ZN4myos21hardwarecommunication16InterruptManager15handleInterruptEhj

.macro HandleException num
.global _ZN4myos21hardwarecommunication16InterruptManager19HandleException\num\()Ev
_ZN4myos21hardwarecommunication16InterruptManager19HandleException\num\()Ev:
    movb $\num, (interruptnumber)
    jmp int_bottom
.endm

.macro HandleInterruptRequest num
.global _ZN4myos21hardwarecommunication16InterruptManager26HandleInterruptRequest\num\()Ev
_ZN4myos21hardwarecommunication16InterruptManager26HandleInterruptRequest\num\()Ev:
    movb $\num + IRQ_BASE, (interruptnumber)
    jmp int_bottom
.endm

HandleException 0x00
HandleException 0x01
HandleException 0x02
HandleException 0x03
HandleException 0x04
HandleException 0x05
HandleException 0x06
HandleException 0x07
HandleException 0x08
HandleException 0x09
HandleException 0x0A
HandleException 0x0B
HandleException 0x0C
HandleException 0x0D
HandleException 0x0E
HandleException 0x0F
HandleException 0x10
HandleException 0x11
HandleException 0x12
HandleException 0x13

HandleInterruptRequest 0x00
HandleInterruptRequest 0x01
HandleInterruptRequest 0x02
HandleInterruptRequest 0x03
HandleInterruptRequest 0x04
HandleInterruptRequest 0x05
HandleInterruptRequest 0x06
HandleInterruptRequest 0x07
HandleInterruptRequest 0x08
HandleInterruptRequest 0x09
HandleInterruptRequest 0x0A
HandleInterruptRequest 0x0B
HandleInterruptRequest 0x0C /* 鼠标 */
HandleInterruptRequest 0x0D
HandleInterruptRequest 0x0E
HandleInterruptRequest 0x0F
HandleInterruptRequest 0x31

HandleInterruptRequest 0x80

int_bottom:
    
    pusha
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs

    pushl %esp
    push (interruptnumber)
    call _ZN4myos21hardwarecommunication16InterruptManager15handleInterruptEhj
    # addl $5, %esp #不用执行这个
    movl %eax, %esp

    popl %gs
    popl %fs
    popl %es
    popl %ds
    popa

.global _ZN4myos21hardwarecommunication16InterruptManager15InterruptIgnoreEv
_ZN4myos21hardwarecommunication16InterruptManager15InterruptIgnoreEv:

    iret

.data
    interruptnumber: .byte 0

