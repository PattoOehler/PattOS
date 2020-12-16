section .multiboot
	MAGIC dd 0x1badb002
	FLAGS dd (1<<0 | 1<<1)
	CHECKSUM dd -(0x1badb002 + (1<<0 | 1<<1))
	
section .text
	extern kernelMain
	extern callConstructors
	global loader



loader:
	mov esp, kernel_stack
	call callConstructors
	
	push eax
	push ebx
	call kernelMain
	
	
section .bss
	resb 2*1024*1024
kernel_stack:
