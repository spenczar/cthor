#include <stdlib.h>
#include <string.h>

#include "str.h"

int string_new(struct String *string, size_t length) {
  if (length < 1) {
    return -1;
  }
  string->length = length;
  string->data = malloc(length);
  if (string->data == NULL) {
    return -1;
  }
  return 0;
}

struct String string_create(const char *data) {
  /// Create a new String object, copied from the given data.
  struct String string;
  string.length = strlen(data);
  string.data = malloc(string.length);
  memcpy(string.data, data, string.length);
  return string;
}

void string_free(struct String *string) {
  if (string->data != NULL) {
    free(string->data);
    string->data = NULL;
  }
}
