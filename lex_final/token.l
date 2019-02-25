%{
#include<stdio.h>
%}

%%

auto|double|int|struct|break|else|long|switch|case|enum|register|typedef|char|extern|return|union|continue|for|signed|void|do|if|static|while|default|goto|sizeof|volatile|const|float|short {ECHO; printf("\tKEYWORD\n");}
[{}()] {ECHO; printf("\tBRACKETS\n");}
[<>] {ECHO; printf("\tRELATIONAL OPERATOR\n");}
[=] {ECHO; printf("\tASSIGNMENT OPERATOR\n");}
[+|-|/|*] {ECHO; printf("\tARTHMETIC OPERATOR\n");}
[{};,<>()+-/*] {ECHO; printf("\tSPECIAL SYBMBOL\n");}
[0-9]+ {ECHO; printf("\tINTEGER\n");}
[0-9]*"."[0-9]+ {ECHO; printf("\tFLOATING NUMBER\n");}
[0-9]+"."[0-9]* {ECHO; printf("\tFLOATING NUMBER\n");}
([a-zA-Z][0-9])+|[a-zA-Z]* {ECHO; printf("\tIDENTIFIER\n");}
.|\n ; 															
%%


int yywrap(void) {}

int main()
{
yyin= fopen("myfile.txt","r");
yylex();
return 0;
}
