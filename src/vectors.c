#include <stdlib.h>
#include <string.h>
#include "vectors.h"

int vector_new(struct Vec *vec, size_t capacity, size_t item_size) {
  if (capacity < 1) {
    return -1;
  }
  vec->length = 0;
  vec->item_size = item_size;
  vec->capacity = capacity;
  vec->data = malloc(capacity * item_size);
  if (vec->data == NULL) {
    return -1;
  }
  return 0;
}

void vector_free(struct Vec *vec) {
  if (vec->data != NULL) {
    free(vec->data);
    vec->data = NULL;
  }
}

void vector_push(struct Vec *vec, void *item) {
  if (vec->length == vec->capacity) {
    vec->capacity *= 2;
    vec->data = realloc(vec->data, vec->capacity * vec->item_size);
  }
  memcpy(vec->data + vec->length * vec->item_size, item, vec->item_size);
  vec->length++;
}

int vector_get(struct Vec *vec, size_t index, void *item) {
  if (index >= vec->length) {
    return -1;
  }
  memcpy(item, vec->data + index * vec->item_size, vec->item_size);
  return 0;
}


void vec_f64_free(struct VecF64 *vec) {
  if (vec->data != NULL) {
    free(vec->data);
    vec->data = NULL;
  }
}

int vec_f64_new(struct VecF64 *vec, size_t capacity) {
  if (capacity < 1) {
    return -1;
  }
  vec->length = 0;
  vec->capacity = capacity;
  vec->data = malloc(capacity * sizeof(double));
  if (vec->data == NULL) {
    return -1;
  }
  return 0;
}

void vec_f64_push(struct VecF64 *vec, double item) {
  if (vec->length == vec->capacity) {
    vec->capacity *= 2;
    vec->data = realloc(vec->data, vec->capacity * sizeof(double));
  }
  vec->data[vec->length] = item;
  vec->length++;
}

int vec_f64_get(struct VecF64 *vec, size_t index, double *item) {
  if (index >= vec->length) {
    return -1;
  }
  *item = vec->data[index];
  return 0;
}
