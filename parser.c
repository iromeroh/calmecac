#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <stdarg.h>

#include "parser.h"
#include "grammar.tab.h"

char * semantics[] = {
"ERROR_SEM",
"FLOAT_LITERAL",
"INT_LITERAL",
"ASSIGNMENT_SEM",
"ADD_SEM",
"REST_SEM",
"MULT_SEM",
"DIV_SEM",
"MODULO_SEM",
"BITWISE_AND_SEM",
"BITWISE_OR_SEM",
"NEG_SEM",
"VAR_VAL",
"VAR_REF"

};


void error( const char* format, ... )
{
  va_list arglist;

  printf( "Error: " );
  va_start( arglist, format );
  vfprintf( stderr, format, arglist );
  va_end( arglist );
}

LinkedList * list_create(void *element, size_t n)
{
  // We allocate memory for the list node + the data
  LinkedList * new_list = (LinkedList *) malloc(sizeof(LinkedList)+n);
  if (new_list){
      //the element is always sizeof(LinkedList) bytes after the node
      new_list->element = new_list+sizeof(LinkedList);
      memcpy(new_list->element, element, n);
      new_list->n = n;
      new_list->next = NULL;
  }
  return new_list;
}

int len(LinkedList * head)
{
    int n=1;
    LinkedList * current = head;

    if (!head)
        return 0;

    while(current->next!=NULL){
        n++;
        current = current->next;
    }
    return n;
}

LinkedList * append(LinkedList * head, void *element, size_t n)
{
    LinkedList * current = head;

    if (!head)
        return NULL;

    while (current->next != NULL)
        current=current->next;

    current->next = list_create(element,n);
    return current->next;
}

LinkedList * element_at(LinkedList * head, int i)
{
    int j=0;
    LinkedList * current = head;

    if (!head)
        return NULL;
    while (j < i && current->next != NULL){
        current = current->next;
        j++;
    }
    return current;
}

LinkedList * insert_after(LinkedList * node, void *element, size_t n)
{
    LinkedList * new_node;
    if (!node)
        return NULL;
    new_node = list_create(element,n);
    if (!new_node)
        return NULL;
    new_node->next = node->next;
    node->next = new_node;
    return new_node;
}

LinkedList * insert_before(LinkedList * node, void *element,size_t n)
{
    void * curr_element;
    LinkedList * new_node;
    if (!node)
        return NULL;
    curr_element = node->element;
    new_node = list_create(curr_element,n);
    if (!new_node)
        return NULL;
    new_node->next = node->next;
    node->next = new_node;
    node->element = element;
    return new_node;
}

ExprType* expr_create(unsigned int op, LinkedList *items)
{
  ExprType * new_node = (ExprType *) malloc(sizeof(ExprType));
  if (new_node){
      new_node->items = items;
      new_node->op = op;
  }
  return new_node;
}

void expr_preorder(ExprType *root, int depth){

    if (root==NULL)
        return;
    
    // printf ("root is not null\n");
    
    LinkedList * current = root->items;
    
    while(current!=NULL){
        //printf ("doing element\n");
        expr_preorder((ExprType*) current->element, depth+1);
        current = current->next;
    }
    
    for(int i=0;i <depth; i++){
        printf("-");
    }
    
    int op = root->op - 97;
    
    printf("semantic is %d, %s\n", root->op, semantics[op]);

}

void expr_inorder(ExprType *root, int depth){

    if (root==NULL)
        return;
    
    // printf ("root is not null\n");

    for(int i=0;i <depth; i++){
        printf("-");
    }
    
    int op = root->op - 97;
    
    printf("semantic is %d, %s\n", root->op, semantics[op]);

    
    LinkedList * current = root->items;
    
    while(current!=NULL){
        //printf ("doing element\n");
        expr_preorder((ExprType*) current->element, depth+1);
        current = current->next;
    }

}


int main(int argc, char*argv[])
{
    yyparse();
}
