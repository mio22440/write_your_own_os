
#include "mouse.h"

MouseDriver::MouseDriver(InterruptManager *manager)
: InterruptHandler(0x2c, manager),
  dataport(0x60),
  commandport(0x64)
{
    offset = 0;
    buttons = 0;

    uint16_t *VideoMemory = (uint16_t *)0xb8000;
    VideoMemory[80*12+40] = (
                                ((VideoMemory[80*12+40] & 0xf000) >> 4) |
                                ((VideoMemory[80*12+40] & 0x0f00) << 4) |
                                ((VideoMemory[80*12+40] & 0x00ff))
                            );
    /* active interrupts */
    commandport.Write(0xab);
    /* 获取当前状态 */
    commandport.Write(0x20);
    uint8_t status = dataport.Read() | 2;

    /* 写回状态 */
    commandport.Write(0x60);
    dataport.Write(status);

    commandport.Write(0xd4);
    dataport.Write(0xf4);
    dataport.Read();
}
MouseDriver::~MouseDriver()
{

}

void printf(char *);
uint32_t MouseDriver::HandleInterrupt(uint32_t esp)
{
    uint8_t status = commandport.Read();
    
    if(!(status & 0x20))
    {
        /* 没有数据就直接返回 */
        return esp;
    }

    static int8_t x=40, y=12;

    buffer[offset] = dataport.Read();
    offset = (offset + 1) % 3;

    if(offset == 0)
    {
        static uint16_t *VideoMemory = (uint16_t *)0xb8000;

        VideoMemory[80*y+x] = (
                                ((VideoMemory[80*y+x] & 0xf000) >> 4) |
                                ((VideoMemory[80*y+x] & 0x0f00) << 4) |
                                ((VideoMemory[80*y+x] & 0x00ff))
                              );

        x += buffer[1];
        if(x < 0) x = 0;
        if(x >= 80) x = 79;

        y -= buffer[2];
        if(y < 0) y = 0;
        if(y >= 25) y=24;

        VideoMemory[80*y+x] = (
                                ((VideoMemory[80*y+x] & 0xf000) >> 4) |
                                ((VideoMemory[80*y+x] & 0x0f00) << 4) |
                                ((VideoMemory[80*y+x] & 0x00ff))
                              );

        for(uint8_t i=0; i<3; i++)
        {
            if((buffer[0] & (0x01 << i)) != (buttons & (0x01 << i)))
            {
                VideoMemory[80*y+x] = (
                                ((VideoMemory[80*y+x] & 0xf000) >> 4) |
                                ((VideoMemory[80*y+x] & 0x0f00) << 4) |
                                ((VideoMemory[80*y+x] & 0x00ff))
                              );
            }
        }
        buttons = buffer[0];
    }

    return esp;
}
