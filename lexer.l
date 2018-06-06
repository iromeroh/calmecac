/* scanner for the Call Me Calc language */

%{
#include <stdio.h>
#include <stdarg.h>
#include "parser.h"
#include "grammar.tab.h"
/* need this for the call to atof() below */
#include <math.h>

int column = 0;
int line = 1;
int last_indent = 0;

void count(void);
void debug(const char *format, ...);

%}

DIGIT    [0-9]
ID       [a-z][a-z0-9]*
LETTER   	 [a-zA-Z_]

%%

{DIGIT}+    {
            count();
            //printf( "An integer: %s (%d)\n", yytext,
            //        atoi( yytext ) );
            yylval.longValue = atoi(yytext);
            return INT_CONSTANT;
            }

{DIGIT}+"."{DIGIT}*        {
            count();
            //printf( "A float: %s (%g)\n", yytext,
            //        atof( yytext ) );
            yylval.ldoubleValue = atof(yytext);
            return FLOAT_CONSTANT;
            }
LETTER?\"(\\.|[^\\"])*\"	{ 
                              count();
                              yylval.stringValue = strdup(yytext);
                              return(STRING_CONSTANT);
                            }

"\'"."\'"   {
             count();
             return CHAR_CONSTANT;
            }

def        {
             count();
             //printf( "function keyword: %s\n", yytext );
             return DEF;
            }

if         {
             count();
             //printf( "if keyword: %s\n", yytext );
             return IF;
            }

else        {
             count();
             //printf( "else keyword: %s\n", yytext );
             return ELSE;
            }

for        {
             count();
             //printf( "para keyword: %s\n", yytext );
             return FOR;
            }

while   {
             count();
             //printf( "if keyword: %s\n", yytext );
             return WHILE;
           }

{ID}        {
                count();
                //printf( "An identifier: %s\n", yytext );
                yylval.idValue = strdup(yytext);
                return ID;
            }

"("|")"|":"|"+"|"-"|"*"|"/"|"%"|"&"|"\|"|"="   {
                      count();
                      //printf( "An operator: %s\n", yytext );
                      return(yytext[0]);
                  }

"{"[^}\n]*"}"     { /* eat up one-line comments */
                      count();
                  }

[ \t]+          { /* eat up whitespace */
                      count();
                      last_indent = column;
                }
                
\n              {
                   count();
                   return '\n';
                }

.           {
              printf( "Unrecognized character '%s'on line %d, column %d: \n", yytext, line,column );
            }

%%

int yywrap(void)
{
	return 1;
}


void count(void)
{
	int i;

	for (i = 0; yytext[i] != '\0'; i++)
		if (yytext[i] == '\n'){
			column = 0;
			line++;
		}else if (yytext[i] == '\t')
			column += 8 - (column % 8);
		else
			column++;

	//ECHO;
	// printf("\nFound something: %s\n", yytext);
}

void debug(const char *format, ...)
{
  va_list args;
  char buffer[BUFSIZE];

  va_start(args, format);
  vsnprintf(buffer, sizeof buffer, format, args);
  va_end(args);
  fprintf(stderr, "%s\n",buffer);
}

