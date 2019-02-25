%{
#include<stdio.h>
%}

//definition section


%token NUMBER ID                       // token from lex file
%left '+' '-'                                           // left associative 
%left '*' '/'
%%

expr: expr '+' expr                                        // grammer production rule 
     |expr '-' expr
     |expr '*' expr
     |expr '/' expr
     |'-'NUMBER
     |'-'ID
     |'('expr')'
     |NUMBER
     |ID
     ;
%%

//main function

main()
{
printf("PLEASE ENTER THE EXPRESSION: ");
yyparse();
printf("VALID EXPRESSION\n");
exit(0);
}

//if error occured 

int yyerror(char *s)
{
printf("INVALID EXPRESSION\n");
exit(0);
}
