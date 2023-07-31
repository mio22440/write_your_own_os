
#ifndef __INTERRUPTS_H
#define __INTERRUPTS_H

#include "types.h"
#include "port.h"
#include "gdt.h"

class InterruptManager
{
    protected:
        struct GateDescriptor
        {
            uint16_t handlerAddressLowBits;
            uint16_t gdt_codeSegmentSelector;
            uint8_t  reserved;
            uint8_t  access;
            uint16_t handlerAddressHighBits;

        } __attribute__((packed));

        static GateDescriptor interruptDescriptorTable[256];

        struct InterruptDescriptorTablePointer
        {
            uint16_t size;

            /**
             * 我服了，这里的 base 整成 16 位了，我说怎么一启动就爆炸
            */
            // uint16_t base;
            uint32_t base;
        } __attribute__((packed));

        static void SetInterruptDescriptorTableEntry(
            uint8_t interruptNumber,
            uint16_t codeSegmentSelectorOffset,
            void (*handler)(),
            uint8_t DescriptorPrivilegeLevel,
            uint8_t DescriptorType
        );

        Port8bitSlow picMasterCommand;
        Port8bitSlow picMasterData;
        Port8bitSlow picSlaveCommand;
        Port8bitSlow picSlaveData;


    public:
        InterruptManager(GlobalDescriptorTable *gdt);
        ~InterruptManager();

        void Activate();

        static uint32_t handleInterrupt(uint8_t interruptNumber, uint32_t esp);
        
        static void IgnoreInterruptRequest();
        static void HandleInterruptRequest0x00();
        static void HandleInterruptRequest0x01();
};

#endif