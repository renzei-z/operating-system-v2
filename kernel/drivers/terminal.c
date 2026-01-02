#include <io.h>
#include <terminal.h>

// TODO: rewrite our own version - boo libc!!
#include <stdarg.h>

static struct terminal terminal = {0};

#define POS_TO_IDX(x, y) ((y * VGA_WIDTH + x))

void terminal_init() {
  terminal.x = 0;
  terminal.y = 0;
  terminal.color = 0x0F; // TODO
  terminal.buf = (uint16_t*)VGA_START;
  terminal.prompt_x = 0;
  terminal.prompt_y = 0;
}

void set_cursor(int idx) {
  outb(0x3D4, 14);
  outb(0x3D5, (unsigned char)(idx >> 8));

  outb(0x3D4, 15);
  outb(0x3D5, (unsigned char)(idx & 0xFF));
}

void put_char_at(char c, uint8_t color, int x, int y) {
  const int idx = POS_TO_IDX(x, y);
  terminal.buf[idx] = (uint16_t)c | (uint16_t)color << 8;
}

void kprintc(char c) {
  if (c == '\n') {
    terminal.x = 0;
    terminal.y ++;
    return;
  }
  
  put_char_at(c, terminal.color, terminal.x, terminal.y);
  terminal.x++;
  set_cursor(POS_TO_IDX(terminal.x, terminal.y));
}

void kprints(char *s) {
  for (; *s != '\0'; s++) {
    kprintc(*s);
  }
}

void kprintx(uint32_t x) {
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
      kprintc(hex_chars[(int)c]);
      leading_zeroes_finished = 1;
    }
  }

  return;
}

void kclear() {
  for (int x = 0; x < VGA_WIDTH; x++) {
    for (int y = 0; y < VGA_HEIGHT; y++) {
      put_char_at(' ', terminal.color, x, y);
    }
  }
}

void kprintf(char *format, ...) {
  va_list args;
  va_start(args, format);

  for (int i = 0; format[i] != '\0'; ++i) {
    if (format[i] == '%') {
      i++;
      switch (format[i]) {
      case '\0':
	continue;
      case '%':
	kprintc('%');
	break;
      case 's':
	char *s = va_arg(args, char*);
	kprints(s);
	break;
      case 'X':
	uint32_t x = va_arg(args, uint32_t);
	kprintx(x);
	break;
      default:
	kprintc(format[i]);
	break;
      }
    } else {
      kprintc(format[i]);
    }
  }
}

/* static int pos = 0; */
/* static char *video_mem = (char*)VIDEO_MEM_START; */

/* void update_cursor() { */
/*   set_cursor(pos); */
/* } */

/* // TODO: VERY stupid implementation. */
/* // This will very easily buffer overflow */
/* // or delete stuff not inputted by the user. */
/* // */
/* // This is for TESTING PURPOSES ONLY. */
/* void backspace() { */
/*   pos -= 1; */
/*   video_mem[pos * 2] = ' '; */
/*   update_cursor(); */
/* } */

/* void kprintc(char c) { */
/*   if (c == '\n') { */
/*     pos = (pos / 80 + 1) * 80; */
/*     update_cursor(); */
/*     return; */
/*   } */
  
/*   // Print character at position */
/*   video_mem[pos * 2] = c; */
/*   video_mem[pos * 2 + 1] = 0x0F; */
/*   // Update global position */
/*   pos++; */
/*   update_cursor(); */
/* } */

/* void kprints(char *s) { */
/*   while (*s != '\0') { */
/*     kprintc(*s); */
/*     s++; */
/*   } */
/* } */

/* // Clears the terminal. */
/* void kclear() { */
/*   int size = 80 * 25; */

/*   for (int i = 0; i < size; ++i) { */
/*     video_mem[i*2] = ' '; */
/*     video_mem[i*2+1] = 0x0F; */
/*   } */

/*   pos = 0; */
/*   update_cursor(); */
/* } */

