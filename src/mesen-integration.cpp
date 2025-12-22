#include "mesen-integration.hpp"
#include <cstdio>
#include <peekpoke.h>

void put_hexdigit(unsigned char h) {
  if (h < 10)
    putchar('0' + h);
  else
    putchar('a' + h - 10);
}
void put_hex(unsigned char h) {
  put_hexdigit(h >> 4);
  put_hexdigit(h & 0x0f);
}
void put_hex(unsigned short h) {
  put_hex((unsigned char)(h >> 8));
  put_hex((unsigned char)h);
}
void start_mesen_watch(const char *addr) {
  unsigned short address = (unsigned short)(uintptr_t)addr;
  POKE(0x4020, (address >> 8) & 0xFF);
  POKE(0x4020, address & 0xFF);
}
void stop_mesen_watch(const char *addr) {
  unsigned short address = (unsigned short)(uintptr_t)addr;
  POKE(0x4021, (address >> 8) & 0xFF);
  POKE(0x4021, address & 0xFF);
}
void break_mesen(unsigned char label) { POKE(0x4019, label); }