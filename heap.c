/*
 * the heap
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

#include "heap.h"
#include "globals.h"
#include "collector.h"
#include "bistree.h"


void heap_init(Heap* heap, unsigned int size, void (*collector)(List*)){
    heap->base  = mmap ( NULL, size, PROT_READ | PROT_WRITE,
                         MAP_PRIVATE | MAP_ANONYMOUS, 0, 0 );
    heap->size  = size;
    heap->limit = heap->base + size;
    heap->top   = heap->base;
    heap->freeb = (List*)malloc(sizeof(List));
    list_init(heap->freeb);
    heap->collector = collector;
    return;
}

void heap_destroy(Heap* heap) {
    munmap(heap->base, heap->size);
    return;
}

#ifdef MS
void *my_malloc(unsigned int nbytes)
{
    if (heap->top + sizeof(_block_header) + nbytes < heap->limit)
    {
        _block_header *q = (_block_header *)(heap->top);
        q->marked = 0;
        q->size = nbytes;
        char *p = heap->top + sizeof(_block_header);
        heap->top = heap->top + sizeof(_block_header) + nbytes;
        return p;
    }
    else
    {
        if (list_isempty(heap->freeb))
        {
            printf("my_malloc: not enough space, performing GC...\n");
            heap->collector(roots);
            // printf("freed %d blocks\n", heap->freeb->size);
            // list_print(heap->freeb); 
            float f1 = ((heap->freeb->size)*(sizeof(_block_header))+sizeof(BiTreeNode));
            float f2 = (heap->base - heap->top);
            printf("Available heap: %2.1f\n", (100*(f1+f2))/(heap->base - heap->limit));
        }
        if (list_isempty(heap->freeb))
            return NULL;
        void *p = list_getfirst(heap->freeb);
        list_removefirst(heap->freeb);
        return p;
    }
}
#endif 

#ifdef MC
void *my_malloc(unsigned int nbytes)
{
    if (heap->top + sizeof(_block_header) + nbytes < heap->limit)
    {
        _block_header *q = (_block_header *)(heap->top);
        q->marked = 0;
        q->size = nbytes;
        char *p = heap->top + sizeof(_block_header);
        heap->top = heap->top + sizeof(_block_header) + nbytes;
        return p;
    }
    else
    {
        printf("my_malloc: not enough space, performing GC...\n");
        heap->collector(roots);
        printf("Available heap: %2.1f\n", 100*((float)(heap->limit - heap->top))/(heap->limit - heap->base));
        if (heap->top + sizeof(_block_header) + nbytes < heap->limit)
        {
            _block_header *q = (_block_header *)(heap->top);
            q->marked = 0;
            q->size = nbytes;
            char *p = heap->top + sizeof(_block_header);
            heap->top = heap->top + sizeof(_block_header) + nbytes;
            return p;
        }
        else {
            return NULL;
        }
    
    }
}
#endif
