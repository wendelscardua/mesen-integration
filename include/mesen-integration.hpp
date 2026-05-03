#pragma once

void put_hexdigit(unsigned char h);
void put_hex(unsigned char h);
void put_hex(unsigned short h);

// Starts a watch with the given label
// - addr is the address of the label, a null-terminated string
// - watches can be nested
void start_mesen_watch(const char *addr);

// Stops a watch with the given label
// - addr is the address of the label, a null-terminated string
// - will cause Mesen to track cycles from the respective
//   label given to start_mesen_watch()
// - the lua script will display these times in a hierarchical view
void stop_mesen_watch(const char *addr);

// Breaks the execution at the given label
// - label is just an identifier byte so it's possible to know
//   which breakpoint was hit
void break_mesen(unsigned char label);

// Sets up a "data watch"
// - label is the label of the data watch, a null-terminated string
// - value_size is the size of the value to watch, in bytes
// - value_address is the address of the value to watch
// - the lua script will display these values in a table (label = value)
void mesen_data_watch(const char *label, unsigned char value_size,
                      const void *value_address);

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
#define MESEN_DATA_WATCH(label, value_size, value_address)                     \
  do {                                                                         \
  } while (0)
#else
#define START_MESEN_WATCH(addr) start_mesen_watch(addr)
#define STOP_MESEN_WATCH(addr) stop_mesen_watch(addr)
#define BREAK_MESEN(label) break_mesen(label)
#define MESEN_DATA_WATCH(label, value_size, value_address)                     \
  mesen_data_watch(label, value_size, value_address)
#endif