GCCPARAMS = -m32 -fno-use-cxa-atexit -Iinclude -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -fpermissive
NASMPARAMS = -f elf32
LDPARAMS = -melf_i386
ASPARAMS = --32

objects = obj/loader.o \
		  obj/gdt.o \
		  obj/kernel.o \
		  obj/hardwarecommunication/port.o \
		  obj/hardwarecommunication/interrupts.o \
		  obj/hardwarecommunication/interruptstubs.o \
		  obj/hardwarecommunication/display.o \
		  obj/hardwarecommunication/filesystem.o \
		  obj/drivers/keyboard.o \
		  obj/drivers/mouse.o \
		  obj/drivers/driver.o \
		  obj/drivers/ata.o \
		  obj/api/commandline.o \
		  obj/api/commands.o
		  


obj/%.o: src/%.s 
	mkdir -p $(@D)
	as $(ASPARAMS) -o $@ $< 

obj/%.o: src/%.cpp
	mkdir -p $(@D)
	gcc $(GCCPARAMS) -c -o $@ $<

obj/%.o: src/%.asm
	mkdir -p $(@D)
	nasm $(NASMPARAMS) -o $@ $<

mykernel.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

install: mykernel.bin
	sudo cp $< /boot/mykernel.bin
	
pattos.iso: mykernel.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp $< iso/boot/
	echo 'set timeout=0' > iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo 'menuentry "My Operating System" {' >> iso/boot/grub/grug.cfg
	echo '	multiboot /boot/mykernel.bin' >> iso/boot/grub/grub.cfg
	echo '	boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=$@ iso
	rm -r iso
	
	
clean:
	rm -r obj
	rm *.bin
	rm *.iso
