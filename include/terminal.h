#ifndef KSCREEN_H_
#define KSCREEN_H_

#include <types.h>

#define VGA_START (char*)0xB8000;
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

/* void set_cursor(int pos); */

/* void kclear(); */

/* void kprintc(char c); */
/* void kprints(char *s); */
/* void kprintx(unsigned int x); */

/* // -- TESTING ONLY -- */
/* void backspace(); */

void terminal_init();

// TODO: Do we want this to be exposed as an API?
// or is it too unsafe?
// void put_char_at(char c, uint8_t color, int x, int y);
void kprintc(char c);
void kprints(char *s);
void kprintx(uint32_t x);
void kprintf(char *format, ...) __attribute__((format(printf, 1, 2)));

void kclear();

struct terminal {
  int x, y;
  uint8_t color;
  uint16_t *buf;

  int prompt_x, prompt_y;
};

#endif // KSCREEN_H_
