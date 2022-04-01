#include <stdio.h>
#include <stdlib.h>
#define __DEFAULT_CAPACITY 4

typedef enum { INT, USER, CHANNEL, VECTOR } TYPES;

typedef struct __user {
    unsigned int id;
    const char *username;
    unsigned int discriminator;
} User;

typedef struct __channel {
    unsigned int id;
    float activity;
} Channel;

typedef struct __vector {
    void **items;
    size_t size;
    size_t capacity;
} Vector;

User *create_user(unsigned int id, const char *username, unsigned int dscm) {
    User *nuser = malloc(sizeof(User));
    if (nuser != NULL) {
        nuser->id = id;
        nuser->username = username;
        nuser->discriminator = dscm;
        return nuser;
    }
    return NULL;
}

Channel *create_channel(unsigned int id, float activity) {
    Channel *nchannel = malloc(sizeof(Channel));
    if (nchannel != NULL) {
        nchannel->id = id;
        nchannel->activity = activity;
        return nchannel;
    }
    return NULL;
}

Vector *vector_init() {
    Vector *nvec = malloc(sizeof(Vector));
    if (nvec != NULL) {
        nvec->size = 0;
        nvec->capacity = __DEFAULT_CAPACITY;
        nvec->items = malloc(sizeof(void *) * nvec->capacity);

        if (nvec->items != NULL) {
            for (int i = 0; i < nvec->capacity; i++) {
                nvec->items[i] = NULL;
            }
            return nvec;
        }
        free(nvec);
    }
    return NULL;
}

int vrealloc(Vector *vector, size_t ncapacity) {
    void **nitems = malloc(sizeof(void *) * ncapacity);
    if (nitems == NULL) {
        return 1;
    }
    for (int i = 0; i < ncapacity; i++) {
        if (vector->items[i] != NULL) {
            nitems[i] = vector->items[i];
        } else {
            nitems[i] = NULL;
        }
    }
    vector->items = nitems;
    vector->capacity = ncapacity;
    return 0;
}

int vpush(Vector *vector, void *value) {
    if (vector->size == vector->capacity) {
        if (vrealloc(vector, vector->capacity + (vector->capacity / 2)) != 0) {
            return 1;
        }
    }
    vector->items[vector->size++] = value;
    return 0;
}

void *vget(Vector *vector, unsigned int index, TYPES type) {
    if (index < 0 || index > vector->size) {
        return NULL;
    }

    void *res = vector->items[index];
    switch (type) {
    case INT:
        return (int *)(vector->items[index]);
    case USER:
        return (User *)(vector->items[index]);
    case CHANNEL:
        return (Channel *)(vector->items[index]);
    case VECTOR:
        return (Vector *)(vector->items[index]);
    default:
        return NULL;
    }
}

int main(int argc, char *argv[]) {
    Vector *vector = vector_init();

    User *carl = create_user(1, "Carl", 3030);
    User *john = create_user(2, "John", 9441);

    Channel *general = create_channel(1, 2.944);
    Channel *todo = create_channel(2, 0.451);

    int day = 25;
    int month = 3;
    int year = 2022;

    vpush(vector, carl);
    vpush(vector, general);
    vpush(vector, &day);
    vpush(vector, &month);
    vpush(vector, &year);

    User *result = vget(vector, 0, USER);
    printf("Fetching: %s#%i\n", result->username, result->discriminator);

    return 0;
}
