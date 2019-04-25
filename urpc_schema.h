#include <stdlib.h>

typedef int (*URPCSchemaParserFn_t)(void *src, size_t size);

typedef union {
    // Receive types
    // Send types
    URPCSchemaParserFn_t parse;
    uint8_t *u8;
} URPCSchema_t;