#ifndef KIO_H_
#define KIO_H_

#include <types.h>

extern uint8_t inb(uint16_t port);
extern void outb(uint16_t port, uint8_t val);

#endif // KIO_H_
