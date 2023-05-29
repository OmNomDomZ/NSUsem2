#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHUNK_SIZE 10
#define NUM_OF_CHUNKS 10
#define ARRAY_SIZE 10

typedef struct
{
    size_t chunk_size;
    void *chunk_ptr;
} Allocator_t;

typedef struct
{
    void *next;
} Chunk_t;

Allocator_t *create_allocator(size_t chunk_size)
{
    // otherwise the pointer to the next chunk will not fit
    if (chunk_size < sizeof(void *))
    {
        return NULL;
    }

    Allocator_t *allocator = malloc(sizeof(Allocator_t));
    if (allocator == NULL)
    {
        return NULL;
    }
    allocator->chunk_size = chunk_size;
    allocator->chunk_ptr = NULL;
    return allocator;
}

void *connect_chunks(Allocator_t *allocator, void *block)
{
    Chunk_t *current_chunk = block;
    for (int i = 0; i < NUM_OF_CHUNKS - 1; ++i)
    {
        current_chunk->next = (void*)current_chunk + allocator->chunk_size;
        current_chunk = current_chunk->next;
    }
    current_chunk->next = NULL;
    return block;
}

void *allocate_block(Allocator_t *allocator)
{
    // allocate memory for chunks in one block
    size_t full_size = NUM_OF_CHUNKS * allocator->chunk_size;
    void *block = malloc(full_size);
    if (block == NULL)
    {
        return NULL;
    }

    block = connect_chunks(allocator, block);
    allocator->chunk_ptr = block;

    return block;
}

void *alloc_chunk(Allocator_t *allocator)
{
    if (allocator->chunk_ptr == NULL)
    {
        void *block = allocate_block(allocator);
    }

    void *current_chunk = allocator->chunk_ptr;
    allocator->chunk_ptr = ((Chunk_t *)current_chunk)->next;

    return current_chunk;
}

void free_chunk(Allocator_t *allocator, void *ptr)
{
    if (ptr == NULL)
    {
        return;
    }

    ((Chunk_t *)ptr)->next = allocator->chunk_ptr;
    allocator->chunk_ptr = ptr;
}

void *calloc_chunk(Allocator_t *allocator)
{
    void *calloc_chunk = alloc_chunk(allocator);
    memset(calloc_chunk, 0, allocator->chunk_size);
    return calloc_chunk;
}

int main()
{
    Allocator_t *allocator = create_allocator(CHUNK_SIZE);
    if (allocator != NULL)
    {
        int *values[ARRAY_SIZE];

        printf("allocated:\n");
        for (int i = 0; i < 7; ++i)
        {
            values[i] = alloc_chunk(allocator);
            printf("%p\n", values[i]);
        }

        *values[5] = 5;
        printf("values[5] = %i\n", *values[5]);

        printf("free:\n");
        free_chunk(allocator, values[5]);
        printf("%p\n", values[5]);

        printf("reuse:\n");
        values[5] = alloc_chunk(allocator);
        *values[5] = 10;
        printf("%i\n%p\n", *values[5], values[5]);

        printf("calloc:\n");
        values[9] = calloc_chunk(allocator);
        printf("%p %i\n", values[9], *values[9]);
    }

    return EXIT_SUCCESS;
}
