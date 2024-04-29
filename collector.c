/*
 * collector.c
 */

#include <stdio.h>

#include "list.h"

#include "globals.h"
#include "bistree.h"
#include "heap.h"

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
   printf("gcing()...\n");
   return;
 }

void mark(void){
   for(int i=0; i<roots->size; i++){
      BisTree* t = (BisTree*) list_get(roots,i);
      mark_tree(t->root);
   }
}

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

void sweep(void){
   char* p = heap->top;
   while(p<heap->limit){
      _block_header* q = (_block_header*) p;
      if(q->marked == 1){
         q->marked = 0;
      }  
      else {
         list_addlast(heap->freeb, (char*)(q+1));
         p = p + (q->size) + sizeof(_block_header);
      }
   }
}


void mark_compact_gc(List* roots) {
   /*
    * mark phase:
    * go throught all roots,
    * traverse trees,
    * mark reachable
    */

   /*
    * compact phase:
    * go through entire heap,
    * compute new addresses
    * copy objects to new addresses
    */
   printf("gcing()...\n");
   return;
 }

void copy_collection_gc(List* roots) {
   /*
    * go throught all roots,
    * traverse trees in from_space,
    * copy reachable to to_space
    */
   printf("gcing()...\n");
   return;
}


void generational_gc(List*roots, ...) {
    printf("gcing()...\n");
}