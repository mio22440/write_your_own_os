
#include "keyboard.h"

KeyboardDriver::KeyboardDriver(InterruptManager *manager)
: InterruptHandler(0x21, manager),
  dataport(0x60),
  commandport(0x64)
{
    while(commandport.Read() & 0x1)
    {
        dataport.Read();
    }

    /* active interrupts */
    commandport.Write(0xae);
    /* 获取当前状态 */
    commandport.Write(0x20);
    uint8_t status = (dataport.Read() | 1) & ~0x10;

    /* 写回状态 */
    commandport.Write(0x60);
    dataport.Write(status);

    /* 打开键盘中断？ */
    dataport.Write(0xf4);
}
KeyboardDriver::~KeyboardDriver()
{

}

void printf(char *);
uint32_t KeyboardDriver::HandleInterrupt(uint32_t esp)
{
    uint8_t key = dataport.Read();

    if(key < 0x80)
    {
        switch(key)
        {
            case 0xfa: break;
            case 0x45: case 0xc5: break;
            case 0x1e:
            {
                printf("a");
                break;
            }
            default:
            {
                char *foo = "KEYBOARD 0x00";
                char *hex = "0123456789ABCDEF";

                foo[11] = hex[(key >> 4) & 0x0F];
                foo[12] = hex[key & 0x0F];

                printf(foo);
            }
        }
    }

    return esp;
}
