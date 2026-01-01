#include <screen.h>
#include <types.h>

extern uint8_t inb(unsigned short port);
extern void outb(unsigned short port, unsigned short val);

static int count = 0;

struct regs {
  uint32_t ds;
  uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
  uint32_t int_no, err_code;
  uint32_t eip, cs, eflags, useresp, ss;
};

void isr_handler(struct regs *r) {
  if (r->int_no < 32) {
    kprints("EXCEPTION: ");
    kprintx(r->int_no);
    kprintc('\n');
    while(1);
  }
}

unsigned char kbd_us[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',   
  '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',   
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',   0,       
 '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',   0, '*',    0, ' ',   
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-', 0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 
};

void keyboard_callback() {
  uint8_t scancode = inb(0x60);
  //kprintx(scancode);

  if (scancode & 0x80) {

  } else {
    char c = kbd_us[scancode];
    if (c > 0) kprintc(c);
  }
}

void timer_callback() {
  //  kprints("TIMER\n");
}

void irq_handler(struct regs *r) {
  if (r->int_no != 32) {
    /* kclear(); */
    /* kprints("IRQ: "); */
    /* kprintx(count++); */
    /* kprints(" and "); */
    /* kprintx(r->int_no); */
  }
  
  if (r->int_no >= 40) {
    outb(0xA0, 0x20);
  }
  outb(0x20, 0x20);

  if (r->int_no == 32) timer_callback();
  if (r->int_no == 33) keyboard_callback();
}
