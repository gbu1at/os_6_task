
kernel: kernel.o regs.o utils.o
	ld -melf_i386 -T link.ld kernel.o regs.o utils.o -o kernel

kernel.o: kernel.S
	nasm -f elf32 kernel.S -o kernel.o

regs.o: regs.S
	nasm -f elf32 regs.S -o regs.o

utils.o: utils.c
	gcc -ffreestanding -m32 -c utils.c -o utils.o

clean:
	rm *.o kernel

run: kernel
	qemu-system-i386 -kernel kernel


