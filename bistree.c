/*
 * bistree.c
 */

#include <stdio.h>
#include <stdlib.h>

#include "bool.h"
#include "bistree.h"
#include "heap.h"

#include "globals.h"

void bistree_init(BisTree* tree) {
    tree->root = NULL;
    tree->size = 0;
    return;
}

bool bitreenode_lookup(BiTreeNode* node, int data) {
   if (node == NULL)
      return false;
   if (data < node->data)
      return bitreenode_lookup(node->left, data);
   if (data > node->data)
      return bitreenode_lookup(node->right, data);
   return true;
}

bool bistree_lookup(BisTree* tree, int data) {
    return bitreenode_lookup(tree->root, data);
}

BiTreeNode* bitreenode_insert(BiTreeNode* node, int data, BiTreeNode* node2) {
   if (node == NULL) {
      node2->data = data;
      node2->left = NULL;
      node2->right= NULL;
      return node2;
    }
    else if(data < node->data)
      node->left = bitreenode_insert(node->left, data, node2);
    else
      node->right= bitreenode_insert(node->right, data, node2);
    return node;
}

bool bistree_insert(BisTree* tree, int data) {
   if (bistree_lookup(tree, data))
      return false;
   BiTreeNode* node = (BiTreeNode*)my_malloc(sizeof(BiTreeNode)); // passar a ser o my_malloc
   if(node == NULL){
      printf("my_malloc returned NULL. exiting.");
      exit(1);
   }
   // printf("new node %x %x %x\n", node, heap->base, heap->limit);
   tree->root = bitreenode_insert(tree->root, data, node);
   tree->size = tree->size + 1;
   return true;
}

BiTreeNode* bitreenode_remove(BiTreeNode* node, int data) {
   if(data < node->data)
      node->left = bitreenode_remove(node->left, data);
   else if(data > node->data)
      node->right = bitreenode_remove(node->right, data);
   else if(node->left == NULL)
      node = node->right;
   else if(node->right == NULL)
      node = node->left;
   else {
     BiTreeNode* lnode = node->left;
     while(lnode->right != NULL)
        lnode = lnode->right;
     node->data = lnode->data;
     node->left = bitreenode_remove(node->left, lnode->data);
     //free(lnode); // para desparecer, ser feito automiticamente
   }
   return node;
}

bool bistree_remove(BisTree* tree, int data) {
   if (!bistree_lookup(tree, data))
      return false;
   tree->root = bitreenode_remove(tree->root, data);
   tree->size = tree->size - 1;
   return true;
}

void bitreenode_inorder(BiTreeNode* node) {
   if(node == NULL)
      return;
   bitreenode_inorder(node->left);
   printf(" %d ", node->data);
   bitreenode_inorder(node->right);
}

void bistree_inorder(BisTree* tree) {
   printf("root: %p\n", tree);
   bitreenode_inorder(tree->root);
   printf("\n");
}
