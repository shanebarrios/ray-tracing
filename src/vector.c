// #include "vector.h"

// #define VECTOR_INIT_IMPL(T) \
// void vector_##T##_init(VECTOR_TYPE(T)* self) \
// { \
//     self->size = 0; \
//     self->capacity = 1; \
//     self->data = (T*) malloc(self->capacity * sizeof(T)); \
// }

// #define VECTOR_SIZED_INIT_IMPL(T) \
// void vector_##T##_sized_init(VECTOR_TYPE(T)* self, size_t size) \
// { \
//     self->size = size; \
//     self->capacity = size; \
//     self->data = (T*) malloc(self->capacity * sizeof(T)); \
// }

// #define VECTOR_REALLOC_IMPL(T) \
// static bool vector_##T##_realloc(VECTOR_TYPE(T)* self, size_t new_capacity) \
// { \
//     T* new_data = (T*) realloc(self->data, sizeof(T) * new_capacity); \
//     if (!new_data) return false; \
//     self->data = new_data; \
//     self->capacity = new_capacity; \
//     return true; \
// } 

// #define VECTOR_PUSH_IMPL(T) \
// void vector_##T##_push(VECTOR_TYPE(T)* self, T val) \
// { \
//     if (self->size >= self->capacity) \
//     { \
//         if (!vector_##T##_realloc(self, self->capacity * 2)) return; \
//     } \
//     self->data[self->size++] = val; \
// }

// #define VECTOR_CPYPUSH_IMPL(T) \
// void vector_##T##_cpypush(VECTOR_TYPE(T)* self, const T* val) \
// { \
//     if (self->size >= self->capacity) \
//     { \
//         if (!vector_##T##_realloc(self, self->capacity * 2)) return; \
//     } \
//     ##T##_copy(val, &self->data[self->size++]); \
// }

// #define VECTOR_DESTROY_IMPL(T) \
// void vector_##T##_destroy(VECTOR_TYPE(T)* self) \
// { \
//     free(self->data); \
// } 

// #define VECTOR_RESERVE_IMPL(T) \
// void vector_##T##_reserve(VECTOR_TYPE(T)* self, size_t capacity) \
// { \
//     if (capacity < self->capacity) return; \
//     vector_##T##_realloc(self, capacity); \
// } 

// #define VECTOR_RESIZE_IMPL(T) \
// void vector_##T##_resize(VECTOR_TYPE(T)* self, size_t size) \
// { \
//     if (size > self->capacity) \
//     { \
//         if (!vector_##T##_realloc(self, size)) return; \
//     } \
//     self->size = size; \
// }

// #define VECTOR_IMPL(T) \ 
//     VECTOR_INIT_IMPL(T) \
//     VECTOR_SIZED_INIT_IMPL(T) \
//     VECTOR_REALLOC_IMPL(T) \
//     VECTOR_PUSH_IMPL(T) \
//     VECTOR_DESTROY_IMPL(T) \
//     VECTOR_RESERVE_IMPL(T) \
//     VECTOR_RESIZE_IMPL(T)

// #define VECTOR_IMPL_WITH_CPY(T) \
//     VECTOR_IMPL(T) \
//     VECTOR_CPYPUSH_IMPL(T)

// VECTOR_IMPL(int)
// VECTOR_IMPL_WITH_CPY(vec3_t);