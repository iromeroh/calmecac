#ifndef __BST_H
#define __BST_H

#include <string.h>

typedef struct TreeNode{
    struct TreeNode * left, * right;
    size_t key_len;
    size_t data_len;
    char * key;
    void * data;
} BST;

BST * new_node(char * key, void *data, size_t data_len);

void insert(BST ** root, char *key, void * data, size_t data_len);

int replace(BST * root, char *key, void * data, size_t data_len);

BST* find(BST*root, char *key);

void in_order(BST*root, void (*fptr)(BST*));

void delete_node(BST **root, char * key);

void clone(BST* src, BST * tgt);

#endif
