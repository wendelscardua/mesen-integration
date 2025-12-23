#pragma once

void put_hexdigit(unsigned char h);
void put_hex(unsigned char h);
void put_hex(unsigned short h);

void start_mesen_watch(const char *addr);
void stop_mesen_watch(const char *addr);
void break_mesen(unsigned char label);

#ifdef NDEBUG
#define START_MESEN_WATCH(addr)                                                \
  do {                                                                         \
  } while (0)
#define STOP_MESEN_WATCH(addr)                                                 \
  do {                                                                         \
  } while (0)
#define BREAK_MESEN(label)                                                     \
  do {                                                                         \
  } while (0)
#else
#define START_MESEN_WATCH(addr) start_mesen_watch(addr)
#define STOP_MESEN_WATCH(addr) stop_mesen_watch(addr)
#define BREAK_MESEN(label) break_mesen(label)
#endif