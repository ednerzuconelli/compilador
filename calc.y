/*yacc -d <nome.y> -v --locations

void yyerror(char*) -> funçao de erro
yyparse(void) -> inicia o parser
yyloc -> localizaçao no codigo fonte(PARÂMETRO = --locations)
	first_line
	first_column
	last_line
	last_column
*/
%{
#include<stdio.h>
#include<stdlib.h>
 int yylex(void);
 void yyerror(char*);
 int SIMBOLOS[50];		
%}
%token  INTEGER
%token  VARIABLE
%%
PROGRAM:  PROGRAM STMT '\n' {printf("%d", $2);}
	| 
	;
STMT:	EXP
	| VARIABLE '='EXP	{SIMBOLOS[$1] = $3;}
	;

EXP:	
	EXP1 		
	|EXP'+'EXP1 	{$$ = $1+$3;}
	|EXP'-'EXP1 	{$$ = $1-$3;}
	;
EXP1:	
	EXP2
	|EXP1'*'EXP2 	{$$ = $1*$3;}
	|EXP1'/'EXP2 	{$$ = $1/$3;}
	;
EXP2:	
	INTEGER 	{$$ = $1;}
	|VARIABLE	{SIMBOLOS[$1] = $1;}
	|'('EXP ')'	{$$ = $2;}
	;
%%
	
void yyerror(char* s) {fprintf(stderr," %s\n",s);} 
int main(){
   yyparse();
  return 0;
}	
