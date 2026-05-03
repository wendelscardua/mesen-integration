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
void mesen_data_watch(const char *label, unsigned char value_size,
                      const void *value_address) {
  unsigned short short_label_address = (unsigned short)(uintptr_t)label;
  unsigned short short_value_address = (unsigned short)(uintptr_t)value_address;
  POKE(0x4022, (short_label_address >> 8) & 0xFF);
  POKE(0x4022, short_label_address & 0xFF);
  POKE(0x4023, value_size);
  POKE(0x4024, (short_value_address >> 8) & 0xFF);
  POKE(0x4024, short_value_address & 0xFF);
}