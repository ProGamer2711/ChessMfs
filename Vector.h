#pragma once

typedef void *TYPE;

typedef struct Vector Vector;

struct Vector {
    TYPE *data;
    int length;
    int capacity;

    // functions
    void (*push)(Vector *vector, TYPE value);
    TYPE (*pop) (Vector *vector);
    TYPE (*get) (Vector *vector, unsigned int index);
    TYPE (* popIndex) (struct Vector* self, unsigned int index);
};

Vector *initVector();

void freeVector(Vector *vector);