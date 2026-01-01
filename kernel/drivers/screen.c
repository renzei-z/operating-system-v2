#include <screen.h>

extern void outb(unsigned short port, unsigned short val);

static int pos = 0;
static char *video_mem = (char*)VIDEO_MEM_START;

void set_cursor(int pos) {
  outb(0x3D4, 14);
  outb(0x3D5, (unsigned char)(pos >> 8));

  outb(0x3D4, 15);
  outb(0x3D5, (unsigned char)(pos & 0xFF));
}

void update_cursor() {
  set_cursor(pos);
}

void kprintc(char c) {
  if (c == '\n') {
    pos = (pos / 80 + 1) * 80;
    update_cursor();
    return;
  }
  
  // Print character at position
  video_mem[pos * 2] = c;
  video_mem[pos * 2 + 1] = 0x0F;
  // Update global position
  pos++;
  update_cursor();
}

void kprints(char *s) {
  while (*s != '\0') {
    kprintc(*s);
    s++;
  }
}

void kprintx(unsigned int x) {
  char *hex_chars = "0123456789ABCDEF";
  kprints("0x");

  if (x == 0) {
    kprintc('0');
    return;
  }

  int leading_zeroes_finished = 0;
  for (int i = 28; i >= 0; i -= 4) {
    char c = (x >> i) & 0xF;
    if (c > 0 || leading_zeroes_finished) {
      kprintc(hex_chars[c]);
      leading_zeroes_finished = 1;
    }
  }

  return;
}

// Clears the terminal.
void kclear() {
  int size = 80 * 25;

  for (int i = 0; i < size; ++i) {
    video_mem[i*2] = ' ';
    video_mem[i*2+1] = 0x0F;
  }

  pos = 0;
  update_cursor();
}

