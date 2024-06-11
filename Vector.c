#include "Vector.h"

#include <stdio.h>
#include <stdlib.h>

static void resizeVector(Vector *vector) {
    if (vector->length >= vector->capacity) {
        vector->capacity *= 2;
        vector->data = (TYPE *)realloc(vector->data, sizeof(TYPE) * vector->capacity);

        if (vector->data == NULL) {
            printf("Failed to allocate memory; Exiting...\n");
            exit(1);
        }
    } else if (vector->length != 0 && vector->length <= vector->capacity / 2) {
        vector->capacity /= 2;
        vector->data = (TYPE *)realloc(vector->data, sizeof(TYPE) * vector->capacity);

        if (vector->data == NULL) {
            printf("Failed to allocate memory; Exiting...\n");
            exit(1);
        }
    }
}

void push(Vector *vector, TYPE value) {
    resizeVector(vector);

    vector->data[vector->length++] = value;
}

TYPE pop(Vector *vector) {
    if (vector->length == 0) {
        printf("Vector is empty; Exiting...\n");
        exit(1);
    }

    TYPE value = vector->data[--vector->length];

    resizeVector(vector);

    return value;
}

TYPE get(Vector *vector, unsigned int index) {
    if (index < 0 || index >= vector->length) {
        printf("Invalid index; Exiting...\n");
        exit(1);
    }

    return vector->data[index];
}

void freeVector(Vector *vector) {
    free(vector->data);
    free(vector);
}

Vector *initVector() {
    Vector *vector = (Vector *)malloc(sizeof(Vector));

    if (vector == NULL) {
        printf("Failed to allocate memory; Exiting...\n");
        exit(1);
    }

    vector->length = 0;
    vector->capacity = 1;
    vector->data = calloc(vector->capacity, sizeof(TYPE));

    if (vector->data == NULL) {
        printf("Failed to allocate memory; Exiting...\n");
        exit(1);
    }

    // attach functions
    vector->push = push;
    vector->pop = pop;
    vector->get = get;

    return vector;
}