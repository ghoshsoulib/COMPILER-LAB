%{
#include<stdio.h>
int lines=0, words=0,s_letters=0,c_letters=0, num=0, spl_char=0,total=0, decimal = 0, charec = 0;;
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
printf("This File contains the following:");
printf("\n%d lines: ", lines);
printf("\n%d words: ",words);
printf("\n%d small letters: ", s_letters);
printf("\n%d capital letters: ",c_letters);
printf("\n%d Integers: ", num);
printf("\n%d special characters: ",spl_char);
printf("\n%d Floating point: ",decimal);
printf("\n%d characters: \n",charec);
//printf("\nTotal %d characters.\n",total);
return 0;
}
 


