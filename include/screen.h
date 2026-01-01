#ifndef KSCREEN_H_
#define KSCREEN_H_

#define VIDEO_MEM_START (char*)0xB8000;

void set_cursor(int pos);

void kclear();

void kprintc(char c);
void kprints(char *s);
void kprintx(unsigned int x);

// -- TESTING ONLY --
void backspace();

#endif // KSCREEN_H_
