/*
int yylex(void) ->chama o lexer para o inicio da analise
char *yytext -> ponteiro para a string alvo
int yyleng -> tamanho da string-alvo
int yywrap -> chamada ao final do processamento(retorna 1 se terminou, 0 se continuar)
FILE *yyin -> Entrada para o lexer, padrao stdin
*/

%{
 int count=0;
// int yylineno=0
%}

digit	[0-9]
letter	[A-Za-z]

%%
\n	{ yylineno++; }
{letter}({letter}|{digit})* {printf("Identificador %s na linha %d\n",yytext,yylineno); }
{digit}+{letter}({letter}|{digit})*  {printf("identificador invalido na linha %d\n",yylineno); }
	count++;
.	{ ; }
%%
int yywrap(void){
	return 1;
}

int main(int argc, char *argv[]){
  yyin = fopen(argv[1],"r");
  yylex();
  fclose(yyin);

  printf("Numero de identificadores = %d\n",count);
  return 0;
}
