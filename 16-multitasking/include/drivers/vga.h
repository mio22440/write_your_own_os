#ifndef __MYOS__DRIVERS__VGA_H
#define __MYOS__DRIVERS__VGA_H

#include "common/types.h"
#include "hardwarecommunication/interrupts.h"
#include "hardwarecommunication/port.h"
#include "drivers/driver.h"

namespace myos
{
    namespace drivers
    {
        class VideoGraphicsArray
        {
            protected:
                hardwarecommunication::Port8Bit miscPort;
                hardwarecommunication::Port8Bit crtcIndexPort;
                hardwarecommunication::Port8Bit crtcDataPort;
                hardwarecommunication::Port8Bit sequencerIndexPort;
                hardwarecommunication::Port8Bit sequencerDataPort;
                hardwarecommunication::Port8Bit graphicsControlIndexPort;
                hardwarecommunication::Port8Bit graphicsControlDataPort;
                hardwarecommunication::Port8Bit attributeControllerIndexPort;
                hardwarecommunication::Port8Bit attributeControllerReadPort;
                hardwarecommunication::Port8Bit attributeControllerWritePort;
                hardwarecommunication::Port8Bit attributeControllerResetPort;
            
                void WriteRegisters(myos::common::uint8_t *registers);
                myos::common::uint8_t *GetFrameBufferSegment();

                virtual myos::common::uint8_t GetColorIndex(myos::common::uint8_t r, myos::common::uint8_t g, myos::common::uint8_t b);

            public:
                VideoGraphicsArray();
                ~VideoGraphicsArray();

                virtual bool SupportsMode(myos::common::uint32_t width, myos::common::uint32_t height, myos::common::uint32_t colordepth);
                virtual bool SetMode(myos::common::uint32_t width, myos::common::uint32_t height, myos::common::uint32_t colordepth);
                virtual void PutPixel(myos::common::int32_t x, myos::common::int32_t y, myos::common::uint8_t r, myos::common::uint8_t g, myos::common::uint8_t b);
                virtual void PutPixel(myos::common::int32_t x, myos::common::int32_t y, myos::common::uint8_t colorIndex);

                virtual void FillRectangle(common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h, common::uint8_t r, common::uint8_t g, common::uint8_t b);
        };
    }
}

#endif
