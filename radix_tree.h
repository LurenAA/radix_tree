#ifndef __RADIX_TREE_XX_
#define __RADIX_TREE_XX_

#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include "simpleQueue.h"

typedef void (*handle)(void*);

// 用位域处理，将四个域总体的大小限制到四个字节

typedef struct radix_node {
  uint32_t is_null: 1;
  uint32_t is_key: 1;
  uint32_t is_compressed: 1;
  uint32_t size: 29;
  char data[];
} radix_node;

typedef struct
{
  radix_node* root;
  int node_numbers;
  int key_numbers;
} radix_tree;

radix_tree* radixNewTree();
radix_node* radixNewNode(int children, bool is_key, bool is_compressed);
uint8_t radixInsert(radix_tree* tree, char* key, int len, void* val);
int linkLowWalk(radix_tree*, const char* ,radix_node**,radix_node***, int*);
void radixSetData(radix_node* node, void* val);
void* radixGetData(radix_node* node);
void traversalDebug(radix_tree* tree, handle had);
#endif