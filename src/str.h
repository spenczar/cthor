#ifndef STR_H
#define STR_H

#include <stddef.h>

struct String {
  size_t length;
  char *data;
};

int string_new(struct String *string, size_t length);
struct String string_create(const char *data);
void string_free(struct String *string);

#endif

