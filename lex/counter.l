%{
#include<stdio.h>
int xx = 0, lines=0, words=0,s_letters=0,c_letters=0, num=0, spl_char=0,total=0, decimal = 0, charec = 0;
%}

%%
 
\n { lines++; words++;}
[\t ' '] words++;
[A-Z] c_letters++;
[a-z] s_letters++;
[0-9]+ num++;
[0-9]*"."[0-9]+ decimal++;
[0-9]+"."[0-9]* decimal++;
. spl_char++;
%%


int yywrap(void) {}

int main()
{
yyin= fopen("myfile.txt","r");
yylex();
total=s_letters+c_letters+num+spl_char+decimal;
charec = c_letters + s_letters;
xx = num - decimal;
printf("This File contains the following:");
printf("\nlines: %d", lines);
printf("\nwords: %d",words);
printf("\nsmall letters: %d", s_letters);
printf("\ncapital letters: %d",c_letters);
printf("\nIntegers: %d", xx);
printf("\nspecial characters: %d",spl_char);
printf("\nFloating point: %d",decimal);
printf("\ncharacters: %d\n",charec);
//printf("\nTotal %d characters.\n",total);
return 0;
}
 


