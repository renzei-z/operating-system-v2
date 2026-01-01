ASM = nasm
QEMU = qemu-system-i386
ASMFLAGS = -fbin
LD=$(HOME)/opt/cross/bin/i686-elf-ld
CC=$(HOME)/opt/cross/bin/i686-elf-gcc
CFLAGS=-ffreestanding -m32 -fno-pie -fno-stack-protector -nostdlib -Iinclude
LDFLAGS=-melf_i386 -T linker.ld

CSOURCES=$(shell find kernel -name "*.c")
OBJ=${CSOURCES:.c=.o} kernel/arch/i386/kernel_entry.o kernel/arch/i386/isr.o

.PHONY: all clean run

all: os_image.bin

os_image.bin: boot/boot.bin kernel.bin
	cat $^ > $@
	truncate -s 1440k $@

kernel.bin: ${OBJ}
	${LD} -o $@ ${LDFLAGS} $^ --oformat binary

%.o: %.c
	${CC} ${CFLAGS} -c $< -o $@

%.o: %.asm
	${ASM} -felf32 $< -o $@

boot/boot.bin: boot/boot.asm
	${ASM} ${ASMFLAGS} $< -o $@

run: os_image.bin
	$(QEMU) -drive format=raw,file=os_image.bin $(QEMU_FLAGS)

clean:
	rm -rf *.bin *.o ${OBJ} os-image.bin boot/boot.bin $(shell find . -name "*~")

