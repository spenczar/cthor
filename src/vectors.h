#ifndef vectors_h
#define vectors_h

struct Vec {
  /// A generic vector.
  size_t length;
  size_t capacity;
  size_t item_size;
  void *data;
};
int vector_new(struct Vec *vec, size_t capacity, size_t item_size);
void vector_free(struct Vec *vec);
void vector_push(struct Vec *vec, void *item);
int vector_get(struct Vec *vec, size_t index, void *item);

struct VecF64 {
  /// A vector of 64-bit floating point numbers.
  size_t length;
  size_t capacity;
  double *data;
};
int vec_f64_new(struct VecF64 *vec, size_t capacity);
void vec_f64_free(struct VecF64 *vec);
void vec_f64_push(struct VecF64 *vec, double item);
int vec_f64_get(struct VecF64 *vec, size_t index, double *item);


#endif
