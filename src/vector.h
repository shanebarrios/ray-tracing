// #ifndef VECTOR_H
// #define VECTOR_H

// #include "common.h"


// #define VECTOR_TYPE(T) vector_##T##_t

// #define VECTOR_STRUCT_IMPL(T) \
// typedef struct vector_##T \
// { \
//     T* data; \
//     size_t size; \
//     size_t capacity; \
// } vector_##T##_t; 

// #define VECTOR_HEADER(T) \
//     VECTOR_STRUCT_IMPL(T) \
//     void vector_##T##_init(VECTOR_TYPE(T)* self); \
//     void vector_##T##_sized_init(VECTOR_TYPE(T)* self, size_t size); \
//     void vector_##T##_push(VECTOR_TYPE(T)* self, T val); \
//     void vector_##T##_destroy(VECTOR_TYPE(T)* self); \
//     void vector_##T##_reserve(VECTOR_TYPE(T)* self, size_t capacity); \
//     void vector_##T##_resize(VECTOR_TYPE(T)* self, size_t size); \
//     void vector_##T##_clear(VECTOR_TYPE(T)* self);

// #define VECTOR_HEADER_WITH_CPY(T) \
//     VECTOR_HEADER(T) \
//     void vector_##T##_cpypush(VECTOR_TYPE(T)* self, const T* val);

// struct scene_object;
// typedef struct scene_object scene_object_t;

// VECTOR_HEADER(int)
// VECTOR_HEADER(scene_object_t)

// #undef VECTOR_HEADER_WITH_CPY
// #undef VECTOR_HEADER
// #undef VECTOR_STRUCT_IMPL

// #endif