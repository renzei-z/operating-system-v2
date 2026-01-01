#include <gdt.h>

struct gdt_entry gdt[3];
struct gdt_ptr gdtptr;

extern void gdt_flush(uint32_t);

void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt[num].base_lo = (base & 0xFFFF);
    gdt[num].base_mid = (base >> 16) & 0xFF;
    gdt[num].base_hi = (base >> 24) & 0xFF;
    gdt[num].limit_lo = (limit & 0xFFFF);
    gdt[num].granularity = (limit >> 16) & 0x0F;
    gdt[num].granularity |= gran & 0xF0;
    gdt[num].access = access;
}

void load_gdt() {
  gdtptr.limit = (sizeof(struct gdt_entry) * 3) - 1;
  gdtptr.base = (uint32_t)&gdt;

  gdt_set_gate(0, 0, 0, 0, 0);
  gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code
  gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data

  gdt_flush((uint32_t)&gdtptr);
}
