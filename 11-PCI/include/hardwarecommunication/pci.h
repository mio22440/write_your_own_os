
#ifndef __MYOS_HARDWARECOMMUNICATION__PCI_H
#define __MYOS_HARDWARECOMMUNICATION__PCI_H

#include "common/types.h"
#include "hardwarecommunication/port.h"
#include "hardwarecommunication/interrupts.h"
#include "drivers/driver.h"

namespace myos
{
    namespace hardwarecommunication
    {
        class PeripheralComponentInterconnectDeviceDescriptor
        {
            public:
                PeripheralComponentInterconnectDeviceDescriptor();
                ~PeripheralComponentInterconnectDeviceDescriptor();

                myos::common::uint32_t portBase;
                myos::common::uint32_t interrupt;

                myos::common::uint16_t bus;
                myos::common::uint16_t device;
                myos::common::uint16_t function;

                myos::common::uint16_t vendor_id;
                myos::common::uint16_t device_id;

                myos::common::uint16_t class_id;
                myos::common::uint16_t subclass_id;
                myos::common::uint16_t interface_id;

                myos::common::uint16_t revision;
        };

        class PeripheralComponentInterconnectController
        {
            Port32Bit dataPort;
            Port32Bit commandPort;

            public:
                PeripheralComponentInterconnectController();
                ~PeripheralComponentInterconnectController();

                myos::common::uint32_t Read(myos::common::uint16_t bus, myos::common::uint16_t device, myos::common::uint16_t function, myos::common::uint32_t registeroffset);
                void Write(myos::common::uint16_t bus, myos::common::uint16_t device, myos::common::uint16_t function, myos::common::uint32_t registeroffset, myos::common::uint32_t value);
                bool DeviceHasFunctions(myos::common::uint16_t bus, myos::common::uint16_t device);

                void SelectDrivers(myos::drivers::DriverManager *driverManager);
                PeripheralComponentInterconnectDeviceDescriptor GetDeviceDescriptor(myos::common::uint16_t bus, myos::common::uint16_t device, myos::common::uint16_t function);
        };
    }
}

#endif
