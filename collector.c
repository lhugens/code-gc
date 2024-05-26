/*
 * collector.c
 */

#include <stdio.h>
#include <string.h>

#include "list.h"

#include "globals.h"
#include "bistree.h"
#include "heap.h"

void mark_tree(BiTreeNode* t){
   if(t==NULL)
      return;
   // marcar nó corrente
   _block_header* p = ((_block_header*) t) - 1;
   p->marked = 1;
   // marcar os nós nas sub-arvores esquerda e direita
   mark_tree(t->left);
   mark_tree(t->right);
}

void mark(void){
   for(int i=0; i<roots->size; i++){
      BisTree* t = (BisTree*) list_get(roots,i);
      mark_tree(t->root);
   }
}

#ifdef MS
void sweep(void){
   char* p = heap->base;
   while(p<heap->top){
      _block_header* q = (_block_header*) p;
      if(q->marked == 1){
         q->marked = 0;
      }  
      else {
         list_addfirst(heap->freeb, (void*)(q+1));
      }
      p = p + (q->size) + sizeof(_block_header);
   }
}

void mark_sweep_gc(List* roots) {
   /*
    * mark phase:
    * go throught all roots,
    * traverse trees,
    * mark reachable
    */
   mark();
   /*
    * sweep phase:
    * go through entire heap,
    * add unmarked to free list
    */
   sweep();
}
#endif 
////////////////////////////////////////////////////////
#ifdef MC
void* computeLocations(void){
   char* _scan = heap->base;
   char* _free = heap->base;
   while(_scan < heap->top){
      _block_header* p = (_block_header*) _scan;
      if(p->marked){
         p->auxpointer =  _free + sizeof(_block_header); // forwardingAddress(scan) <- free
         _free = _free + p->size + sizeof(_block_header);
         // printf("p->size %d\n", p->size);
      }
      _scan = _scan + p->size + sizeof(_block_header);
      // printf("_scan %p\n", _scan);
   }
   return _free;
}

void updateReferences(void){
   printf("inside updateReferences\n");
   fflush(stdout);
   for(int i=0; i<roots->size; i++){
      BisTree* tree = (BisTree*) list_get(roots,i);
      if(tree!=NULL){
         _block_header* p = ((_block_header*) (tree->root)) - 1;
         tree->root = (BiTreeNode*) (p->auxpointer);
      }
      char* _scan = heap->base;
      while(_scan < heap->top){
         _block_header* q = (_block_header*) _scan;
         BiTreeNode* from = (BiTreeNode*) (q + 1);
         if(q->marked){
            if(from->left != NULL){
               BiTreeNode* to = (((_block_header*) (from->left)) - 1)->auxpointer;
               from->left = to;
            }
            if(from->right != NULL){
               BiTreeNode* to = (((_block_header*) (from->right)) - 1)->auxpointer;
               from->right = to;
            }
         }
         _scan = _scan + q->size + sizeof(_block_header);
      }
   }
}

void relocate(void* free){
   printf("inside relocate\n");
   fflush(stdout);
   char* _scan = heap->base;
   while(_scan < heap->top){
      _block_header* p = (_block_header*) _scan;
      if(p->marked){
         _block_header* q = ((_block_header*) (p->auxpointer)) - 1;
         memcpy(q, p, p->size+sizeof(_block_header));
         q->marked = 0;
      }
      _scan = _scan + p->size + sizeof(_block_header);
   }
   heap->top = free;
}

void compact(void){
   void* p = computeLocations();
   updateReferences();
   relocate(p);
}

void mark_compact_gc(List* roots) {
   /*
    * mark phase:
    * go throught all roots,
    * traverse trees,
    * mark reachable
    */
   mark();

   /*
    * compact phase:
    * go through entire heap,
    * compute new addresses
    * copy objects to new addresses
    */
   compact();
   return;
}
#endif
////////////////////////////////////////////
#ifdef CC
 void flip(char* fromSpace, char* toSpace){
   char* temp = fromSpace;
   fromSpace = toSpace;
   toSpace = temp;

   int extent = (heap->top - heap->base) / 2;
   char* top = toSpace + extent;
   char* free = toSpace;
 }

void copy_collection_gc(List* roots) {
   /*
    * go throught all roots,
    * traverse trees in from_space,
    * copy reachable to to_space
    */
   // create semispace
   char* toSpace = heap->base;
   int extent = (heap->top - heap->base) / 2;
   char* fromSpace = heap->base + extent;
   char* top = heap->base + extent;
   char* free = toSpace;

   flip(fromSpace, toSpace);
   return;
}
#endif

void generational_gc(List*roots, ...) {
    printf("gcing()...\n");
}