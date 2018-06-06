#include <stdio.h>
#include <stdlib.h>

#include "bst.h"

BST * new_node(char * key, void *data, size_t data_len){
    // we allocate space for the node data, the string key + char 0x00 + the data
    BST * node;
    size_t key_len;
    key_len = strlen(key);
    
    node = malloc(sizeof(BST));
    
    if (node == NULL)
        return NULL;
        
    node->left=NULL;
    node->right=NULL;
    
    node->key_len=key_len;
    node->data_len=data_len;
    
    node->key=(char*)malloc(key_len+1);
    node->data=malloc(data_len);
        
    strcpy(node->key,key);
    memcpy(node->data,data,data_len);
    return node;
}

void insert(BST ** root, char *key, void * data, size_t data_len)
{
    
    int cmp;
    
    if ((*root)==NULL){
        (*root) = new_node(key,data,data_len);
        return;     
    }
    
    cmp = strcmp(key, (*root)->key);
    
    if (cmp<0)  // key is smaller than root->key
    {
        insert(&((*root)->left), key, data, data_len);
    }
    else
    {
        insert(&((*root)->right), key, data, data_len);
    }
}

int replace(BST * root, char *key, void * data, size_t data_len)
{
    int cmp;
    
    if (root==NULL){
        return 0;
    }
    
    cmp = strcmp(key, root->key);
    
    if (cmp<0)  // key is smaller than root->key
    {
        return replace(root->left, key, data, data_len);
    }
    else
    {
        int key_len = strlen(key);
        
        free(root->key);
        free(root->data);
        
        root->key=(char*)malloc(key_len+1);
        root->data=malloc(data_len);
        
        strcpy(root->key,key);
        memcpy(root->data,data,data_len);
        
        root->key_len = key_len;
        root->data_len = data_len;
    }
    return 1;
}

BST* find(BST*root, char *key)
{
    int cmp;
    if (!root)
        return NULL;
    cmp = strcmp(key, root->key);
    if ( cmp == 0)
        return root;
    else if ( cmp < 0)
        return find(root->left, key);
    else 
        return find(root->right, key);
    
}

void in_order(BST*root, void (*fptr)(BST*))
{
    if (root == NULL)
        return;
    in_order(root->left, fptr);    
    (*fptr)(root);
    in_order(root->right, fptr);
}

// Helper function to get the mimimum value in the current tree
BST* minimum_key(BST*tree)
{
    BST*curr=tree;
    
    while(curr->left != NULL)
    {
        curr = curr->left;
    }   
    
    return curr;
}

// Helper function to get the parent of a given key, curr becomes NULL if not found
BST* search_key_parent(BST **curr, char *key){
    BST * parent = NULL;
    int cmp;
    while((*curr) != NULL && (cmp=strcmp(key,(*curr)->key)) != 0)
    {
        parent = (*curr);
        if (cmp <0)
            (*curr) = (*curr)->left;
        else
            (*curr) = (*curr)->right;
    }
    return parent;
}


// Helper functions that clones one node to another
void clone(BST* src, BST * tgt){
      
    tgt->key_len = src->key_len;
    tgt->data_len = src->data_len;
    tgt->key = malloc(src->key_len);
    strcpy(tgt->key, src->key);
    tgt->data = malloc(src->data_len);
    memcpy(tgt->data, src->data, src->data_len);
    tgt->left = src->left;
    tgt->right = src->right;
}

void delete_node(BST **root, char * key){
    BST * parent;
    BST * curr = (*root);
    
    parent = search_key_parent(&curr,key);
    
    if (curr == NULL)   // key not found, return
        return;
        
    if (curr->left == NULL && curr->right == NULL) // Case 1: node without descendants (leaf)
    {
        if (curr!=(*root)) // curr is not root, set its parent's left or right leaf to NULL
        {
            if (curr == parent->left)
                parent->left = NULL;
            else
                parent->right = NULL;
        }else{ // curr is root
            (*root) = NULL;
        }
        free(curr->key);
        free(curr->data);
        free(curr);
        
    }else if (curr->left && curr->right) // Case 2: node with two descendants
    {
        // We find the smaller key in the subtree of curr 
        BST * successor = minimum_key(curr);
        
        BST tmp;
        
        // first we clone the successor values, specially data and key into new memory
        clone(successor, &tmp);       
        
        //then we delete the successor, which has at most 1 descendant (the right one)
        delete_node(root, successor->key);
        
        //then we delete curr's obsolete values BUT we don't delete curr!
        
        free(curr->key);
        free(curr->data);
        
        // and finally store the successor cloned values into curr
        curr->key_len = tmp.key_len;
        curr->data_len = tmp.data_len;
        curr->key = tmp.key;
        curr->data = tmp.data;
        

    }else   // Case 3: node to be deleted only has one descendant
    {
        BST * child = curr->left? curr->left:curr->right;
        
        if (curr != (*root)) // current is not root node
        {
			if (curr == parent->left) // we set parent's left or right to NULL, depending of what curr is
			    parent->left = NULL;
			else
			    parent->right=NULL;
		}else  // node to be deleted is root, set it to child
		    (*root) = child;
		free(curr->key);
		free(curr->data);
		free(curr);
    }
    
}

void print_node(BST*node)
{
    printf("Node has key: '%s' and value '%s'\n", node->key, (char *)node->data);
}

