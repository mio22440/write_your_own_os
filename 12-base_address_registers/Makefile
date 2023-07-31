
# DBGFLAGS = -g

GPPPARAMS = $(DBGFLAGS) -Iinclude -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
ASPARAMS = $(DBGFLAGS) --32
LDPARAMS = $(DBGFLAGS) -melf_i386 -Map=mykernel.map

objects = obj/loader.o \
		  obj/gdt.o \
		  obj/drivers/driver.o \
		  obj/hardwarecommunication/port.o \
		  obj/hardwarecommunication/interruptstubs.o \
		  obj/hardwarecommunication/interrupts.o \
		  obj/hardwarecommunication/pci.o \
		  obj/drivers/keyboard.o \
		  obj/drivers/mouse.o \
		  obj/kernel.o

all: mykernel.iso

obj/%.o: src/%.cpp
	mkdir -p $(@D)
	g++ $(GPPPARAMS) -o $@ -c $<

obj/%.o: src/%.s
	mkdir -p $(@D)
	as $(ASPARAMS) -o $@ $<

mykernel.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

mykernel.iso: mykernel.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp $< iso/boot/
	echo 'set timeout=0' >> iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo '' >> iso/boot/grub/grub.cfg
	echo 'menuentry "My Operating System" {' >> iso/boot/grub/grub.cfg
	echo '  multiboot /boot/mykernel.bin' >> iso/boot/grub/grub.cfg
	echo '  boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=$@ iso
	rm -rf iso

.PHONY: all run clean

run: mykernel.iso
	sz mykernel.iso

clean:
	rm -rf obj mykernel.bin mykernel.iso
