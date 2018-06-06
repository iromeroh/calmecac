%{
#include  <stdio.h>
#include  <ctype.h>
#include "bst.h"
#include "parser.h"

BST * symbols = NULL;

int  base;

%}

%union
{
    char charValue;
    long int longValue;
    long double ldoubleValue;
    char *stringValue;
    char *idValue;
    ExprType exprValue;
}

%token ID
%token CHAR_CONSTANT
%token INT_CONSTANT
%token FLOAT_CONSTANT
%token STRING_CONSTANT
%token DEF
%token IF ELSE
%token FOR WHILE

%type <exprValue> stat
%type <exprValue> expr
%type <exprValue> var
%type <idValue> ID
%type <longValue> INT_CONSTANT
%type <ldoubleValue> FLOAT_CONSTANT
%type <stringValue> STRING_CONSTANT
%type <charValue> CHAR_CONSTANT

%start  list

%token  DIGIT  LETTER

%left  '|'
%left  '&'
%left  '+'  '-'
%left  '*'  '/'  '%'
%left  UMINUS      /*  supplies  precedence  for  unary  minus  */

%%      /*  beginning  of  rules  section  */

list :    /*  empty  */
     |    list  stat  '\n' 
               {
                    debug ("  > list stat matched\n");
                    expr_preorder(&($2), 0);
               }
     
     |    list  error  '\n'
               {    yyerrok;  }
     ;

stat :    expr
               {
                   $$ = $1;
                   debug("  > Expression\n");
               }
     |    var  '='  expr
               {

                   BST * var = NULL;                   
                   LinkedList * assignment = list_create((void*) &($1), sizeof(YYSTYPE));
                   append(assignment, (void*) &($3), sizeof(union YYSTYPE) );
                   $$.op = ASSIGNMENT_SEM;
                   $$.items = assignment;                   

                   if ( (var = find (symbols, $1.val.idValue)) == NULL ){
                       insert(&symbols, $1.val.idValue, &($3), sizeof(union YYSTYPE *));
                   }else{
                       replace (var, $1.val.idValue , &($3), sizeof(union YYSTYPE *));
                   }
                   debug ("  > Assignment\n");

               }
     ;
     
var:       ID
           {
             $$.op = VAR_REF;
             $$.val.idValue = $1;
             $$.items = NULL;
           
           }
           ;


expr :    '('  expr  ')'
               {
                   $$  =  $2;
               }
     |    expr  '+'  expr
               {
                   LinkedList * sum = list_create((void*) &($1), sizeof(YYSTYPE));
                   append(sum, (void*) &($3), sizeof(union YYSTYPE) );
                   $$.op = ADD_SEM;
                   $$.items = sum;                   

               }
     |    expr  '-'  expr
               {
                   LinkedList * rest = list_create((void*) &($1), sizeof(YYSTYPE));
                   append(rest, (void*) &($3), sizeof(union YYSTYPE) );
                   $$.op = REST_SEM;
                   $$.items = rest;                   

               }
     |    expr  '*'  expr
               {
                   LinkedList * mult = list_create((void*) &($1), sizeof(YYSTYPE));
                   append(mult, (void*) &($3), sizeof(union YYSTYPE) );
                   $$.op = MULT_SEM;
                   $$.items = mult;                   

               }
     |    expr  '/'  expr
               {
                   LinkedList * div = list_create((void*) &($1), sizeof(YYSTYPE));
                   append(div, (void*) &($3), sizeof(union YYSTYPE) );
                   $$.op = DIV_SEM;
                   $$.items = div;                   

               }
     |    expr  '%'  expr
               {
                   LinkedList * modulo = list_create((void*) &($1), sizeof(YYSTYPE));
                   append(modulo, (void*) &($3), sizeof(union YYSTYPE) );
                   $$.op = MODULO_SEM;
                   $$.items = modulo;                   

               }
     |    expr  '&'  expr
               {
                   LinkedList * and = list_create((void*) &($1), sizeof(YYSTYPE));
                   append(and, (void*) &($3), sizeof(union YYSTYPE) );
                   $$.op = BITWISE_AND_SEM;
                   $$.items = and;                   
               }
     |    expr  '|'  expr
               {
                   LinkedList * or = list_create((void*) &($1), sizeof(YYSTYPE));
                   append(or, (void*) &($3), sizeof(union YYSTYPE) );
                   $$.op = BITWISE_OR_SEM;
                   $$.items = or;    
               }
     |    '-'  expr        %prec  UMINUS
               {
                   LinkedList * negative = list_create((void*) &($2), sizeof(YYSTYPE));

                   $$.op = NEG_SEM;
                   $$.items = negative;    
               }
     |    ID
               {
                   BST * var = NULL;
                   debug("  > id is '%s'\n",$1);
                   if ( (var = find (symbols,$1)) != NULL ){

                       $$.op = VAR_VAL;
                       $$.val.idValue = $1;
                       $$.items = NULL;
                   }else{
                       $$.op = ERROR_SEM;
                       $$.items = NULL;
                       debug ("  > Error: No such variable %s\n",$1);
                   }
               }
     |    INT_CONSTANT 
             {
                 $$.op = INT_LITERAL;
                 $$.val.longValue = $1;
                 $$.items = NULL;
             }
     |    FLOAT_CONSTANT 
             {
                 $$.op = FLOAT_LITERAL;
                 $$.val.ldoubleValue = $1;
                 $$.items = NULL;
             }

     ;

%%      /*  start  of  programs  */

extern char yytext[];
extern int column;

void yyerror(char const *s)
{
    fflush(stdout);
    printf("\n%*s\n%*s\n", column, "^", column, s);
}
