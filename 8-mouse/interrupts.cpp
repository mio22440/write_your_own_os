
#include "interrupts.h"

void printf(char *str);

InterruptHandler::InterruptHandler(uint8_t interruptNumber, InterruptManager *interruptManager)
{
    this->interruptNumber = interruptNumber;
    this->interruptManager = interruptManager;
    interruptManager->handlers[interruptNumber] = this;
}
InterruptHandler::~InterruptHandler()
{
    if(interruptManager->handlers[interruptNumber] == this)
    {
        interruptManager->handlers[interruptNumber] = 0;
    }
}
uint32_t InterruptHandler::HandleInterrupt(uint32_t esp)
{
    return esp;
}

InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256];

InterruptManager *InterruptManager::ActiveInterruptManager = 0;

void InterruptManager::SetInterruptDescriptorTableEntry(
            uint8_t interruptNumber,
            uint16_t codeSegmentSelectorOffset,
            void (*handler)(),
            uint8_t DescriptorPrivilegeLevel,
            uint8_t DescriptorType
)
{
    const uint8_t IDT_DESC_PRESENT = 0x80;

     interruptDescriptorTable[interruptNumber].handlerAddressLowBits = ((uint32_t)handler) & 0xffff;
     interruptDescriptorTable[interruptNumber].handlerAddressHighBits = ((uint32_t)handler >> 16) & 0xffff;
     interruptDescriptorTable[interruptNumber].gdt_codeSegmentSelector = codeSegmentSelectorOffset;
     interruptDescriptorTable[interruptNumber].access = IDT_DESC_PRESENT | DescriptorType | ((DescriptorPrivilegeLevel & 3) << 5);
     interruptDescriptorTable[interruptNumber].reserved = 0;
}

InterruptManager::InterruptManager(GlobalDescriptorTable *gdt)
: picMasterCommand(0x20),
  picMasterData(0x21),
  picSlaveCommand(0xa0),
  picSlaveData(0xa1)
{
    uint32_t CodeSegment = gdt->CodeSegmentSelector();
    const uint8_t IDT_INTERRUPT_GATE = 0xe;

    /* 初始化中断为默认不处理 */
    for(uint16_t i=0; i<256; i++)
    {
        handlers[i] = 0;
        SetInterruptDescriptorTableEntry(i, CodeSegment, &IgnoreInterruptRequest, 0, IDT_INTERRUPT_GATE);
    }

    /* 初始化 0 号中断 (IRQ_BASE + 0) */
    SetInterruptDescriptorTableEntry(0x20, CodeSegment, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
    /* 初始化 1 号中断 (IRQ_BASE + 1) */
    SetInterruptDescriptorTableEntry(0x21, CodeSegment, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);
    /* 初始化鼠标中断 */
    SetInterruptDescriptorTableEntry(0x2c, CodeSegment, &HandleInterruptRequest0x0c, 0, IDT_INTERRUPT_GATE);

    picMasterCommand.Write(0x11);
    picSlaveCommand.Write(0x11);

    picMasterData.Write(0x20); /* 20-27 */
    picSlaveData.Write(0x28);  /* 28-30? */

    picMasterData.Write(0x04);
    picSlaveData.Write(0x02);

    picMasterData.Write(0x01);
    picSlaveData.Write(0x01);

    picMasterData.Write(0x00);
    picSlaveData.Write(0x00);

    /* 告诉 cpu 用这个中断描述表 */
    InterruptDescriptorTablePointer idt;
    idt.size = 256 * sizeof(GateDescriptor) - 1;
    idt.base = (uint32_t)interruptDescriptorTable;
    asm volatile("lidt %0" : : "m" (idt));

}
InterruptManager::~InterruptManager()
{

}

void InterruptManager::Activate()
{
    if(ActiveInterruptManager != 0)
    {
        ActiveInterruptManager->Deactivate();
    }
    ActiveInterruptManager = this;

    asm("sti");
}

void InterruptManager::Deactivate()
{
    if(ActiveInterruptManager == this)
    {
        ActiveInterruptManager = 0;
        asm("cli");
    }
}

uint32_t InterruptManager::handleInterrupt(uint8_t interruptNumber, uint32_t esp)
{
    if(ActiveInterruptManager != 0)
    {
        return ActiveInterruptManager->DoHandleInterrupt(interruptNumber, esp);
    }

    return esp;
}

uint32_t InterruptManager::DoHandleInterrupt(uint8_t interruptNumber, uint32_t esp)
{
    if(handlers[interruptNumber] != 0)
    {
        /* 有处理函数则处理 */
        esp = handlers[interruptNumber]->HandleInterrupt(esp);
    }
    else if(interruptNumber != 0x20)
    {
        /* 不是时钟中断才打印 */
        char *foo = "UNHANDLED INTERRUPT 0x00";
        char *hex = "0123456789ABCDEF";

        foo[22] = hex[(interruptNumber >> 4) & 0x0F];
        foo[23] = hex[interruptNumber & 0x0F];

        printf(foo);
    }

    if(0x20 <= interruptNumber && interruptNumber < 0x30)
    {
        /* 响应中断 */
        picMasterCommand.Write(0x20);

        if(0x28 <= interruptNumber)
        {
            picSlaveCommand.Write(0x20);
        }
    }

    return esp;
}
