%{
#include<stdio.h>
#include<stdlib.h>
%}

%token A B NL

%%
stmt: S NL {printf("VALID STRING\n");
             exit(0);}
;
S: A S B 
| A S
| S B
|
;
%%

int yyerror(char *msg)
{
printf("INVALID STRING\n");
exit(0);
}

main()
{
printf("PLEASE ENTER THE STRING:  ");
yyparse();
}
