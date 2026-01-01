#include <idt.h>

struct idt_entry idt[256];
struct idt_ptr idtptr;

extern void idt_load();

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
  idt[num].base_lo = (base & 0xFFFF);
  idt[num].base_hi = (base >> 16) & 0xFFFF;
  idt[num].sel = sel;
  idt[num].reserved = 0;
  idt[num].flags = flags;
}

void load_idt() {
  // NOTE: It is `-1` as limit refers to the maximum
  // pointable address, not the actual end of the idt.
  // Therefore, we need to point at the start of the last
  // entry, which is sizeof(idt_entry) away from the end.
  idtptr.limit = (sizeof(struct idt_entry) * 256) - 1;
  idtptr.base = (uint32_t)&idt;

  uint32_t isr_addresses[] = {
    (uint32_t)isr0,  (uint32_t)isr1,  (uint32_t)isr2,  (uint32_t)isr3,
    (uint32_t)isr4,  (uint32_t)isr5,  (uint32_t)isr6,  (uint32_t)isr7,
    (uint32_t)isr8,  (uint32_t)isr9,  (uint32_t)isr10, (uint32_t)isr11,
    (uint32_t)isr12, (uint32_t)isr13, (uint32_t)isr14, (uint32_t)isr15,
    (uint32_t)isr16, (uint32_t)isr17, (uint32_t)isr18, (uint32_t)isr19,
    (uint32_t)isr20, (uint32_t)isr21, (uint32_t)isr22, (uint32_t)isr23,
    (uint32_t)isr24, (uint32_t)isr25, (uint32_t)isr26, (uint32_t)isr27,
    (uint32_t)isr28, (uint32_t)isr29, (uint32_t)isr30, (uint32_t)isr31,

    (uint32_t)isr32, (uint32_t)isr33, (uint32_t)isr34, (uint32_t)isr35,
    (uint32_t)isr36, (uint32_t)isr37, (uint32_t)isr38, (uint32_t)isr39,
    (uint32_t)isr40, (uint32_t)isr41, (uint32_t)isr42, (uint32_t)isr43,
    (uint32_t)isr44, (uint32_t)isr45, (uint32_t)isr46, (uint32_t)isr47
  };
  
  //idt_set_gate(0, (uint32_t)isr0, 0x08, 0x8E);
  for (int i = 0; i < 256; ++i) idt_set_gate(i, isr_addresses[i], 0x08, 0x8E);

  
  idt_load();
}
