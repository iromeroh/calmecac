#ifndef __PARSER_H
#define __PARSER_H

#define BUFSIZE 500

#define ERROR_SEM          97
#define FLOAT_LITERAL      98
#define INT_LITERAL        99
#define ASSIGNMENT_SEM     100
#define ADD_SEM            101
#define REST_SEM           102
#define MULT_SEM           103
#define DIV_SEM            104
#define MODULO_SEM         105
#define BITWISE_AND_SEM    106
#define BITWISE_OR_SEM     107
#define NEG_SEM            108
#define VAR_VAL            109
#define VAR_REF            110



typedef struct NodeType{
    void * element;
    size_t n;
    struct NodeType * next;
}LinkedList;

typedef struct {
    char charValue;
    long int longValue;
    long double ldoubleValue;
    char *stringValue;
    char *idValue;
} Value;

typedef struct{
    int op;
    Value val;
    LinkedList * items;
} ExprType;

void error( const char* format, ... );
void yyerror(char const *s);

LinkedList * list_create(void *element, size_t n);
int len(LinkedList * head);
LinkedList * append(LinkedList * head, void *element, size_t n);
LinkedList * element_at(LinkedList * head, int i);
LinkedList * insert_after(LinkedList * node, void *element, size_t n);
LinkedList * insert_before(LinkedList * node, void *element, size_t n);

ExprType* expr_create(unsigned int op, LinkedList *items);
void expr_preorder(ExprType *root, int depth);
void expr_inorder(ExprType *root, int depth);

#endif
