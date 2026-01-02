#include <terminal.h>
#include <idt.h>
#include <io.h>
#include <gdt.h>

extern unsigned int _kernel_start, _kernel_end;

// NOTE: This needs to be changed manually,
// as we do not "link" with boot.asm, so we
// can't just make the value global.
static int loaded_sectors = 30;

// TODO: Move this somewhere better?
void remap_pic() {
  // ICW1: Start initialization
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    // ICW2: Set the offsets (Remap!)
    // Master starts at 32 (0x20), Slave starts at 40 (0x28)
    outb(0x21, 0x20); 
    outb(0xA1, 0x28);

    // ICW3: Tell them how they are wired together
    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    // ICW4: Give additional information about environment
    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    // Mask: Enable all interrupts (for now)
    outb(0x21, 0x00);
    outb(0xA1, 0x00);
}

void kmain() {
  terminal_init();
  
  kclear();

  kprintf("[INFO] Loaded 30 sectors of kernel.\n");

  kprintf("[INFO] _kernel_start: %X; _kernel_end: %X\n",
	  (uint32_t)&_kernel_start,
	  (uint32_t)&_kernel_end);
  
  kprintf("[INFO] Kernel Size: %X/%X\n",
	  ((uint32_t)&_kernel_end)-((unsigned int)&_kernel_start),
	  (uint32_t)(loaded_sectors * 512));
  
  load_gdt();
  
  kprintf("[INFO] GDT loaded successfully.\n");

  remap_pic();
  load_idt();

  kprintf("[INFO] IDT loaded successfully.\n");
  
  __asm__ volatile("sti");

  /* kclear(); */
  kprintf("> ");
  
  while(1);
}
