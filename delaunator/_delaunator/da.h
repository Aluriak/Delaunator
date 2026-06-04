#pragma once

// Dynamic Arrays
#define DA_INIT(arr)         ((arr).data = NULL, (arr).len = 0, (arr).cap = 0)
#define DA_PUSH(arr, val)    do {                                          \
    if ((arr).len == (arr).cap) {                                          \
        (arr).cap = (arr).cap ? (arr).cap * 2 : 8;                         \
        (arr).data = realloc((arr).data, (arr).cap * sizeof(*(arr).data)); \
    }                                                                      \
    (arr).data[(arr).len++] = (val);                                       \
} while(0)
#define DA_FREE(arr)         free((arr).data)
