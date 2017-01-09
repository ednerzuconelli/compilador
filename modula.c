#include <stdio.h>
#include "modula.h"
#include <string.h>

extern int yylex();
extern int yylineno;
extern char* yytext;
char* lookahead;
int ntoken,vtoken;

void erro(char* token){
	printf("\nErro: Era esperado o token %s porém foi encontrado o token %s.",token,lookahead);
}

void const_factor(){}
void const_term(){}
void simple_const_expr(){}


void priority(){
	simple_const_expr();		
	
	if(){
	
	}
}

void reconhecerToken(char* token){
	if(strcmp(lookahead,token)==0){
		lerArray();
	}else{
		erro(token);
	}
}

void definition_module(){}

void implementation_module(){}

void program_module(){
	reconhecerToken("MODULE");

	if(ntoken==2){
		lerArray();	
	}

	if(strcmp(lookahead,"[")==0){
		reconhecerToken("[");
		priority();
		reconhecerToken("]");		
	}

	reconhecerToken(";");

	

	

	printf("reconheceu module e o indetificador e o ; o proximo token é: %s",yytext);
}

void compilationUnit(){
	if(strcmp(lookahead,"DEFINITION")==0)
		definition_module();
	if(strcmp(lookahead,"IMPLEMENTATION")==0)	
		implementation_module();
	if(strcmp(lookahead,"MODULE")==0)	
		program_module();
	else
		erro("DEFINITION ou IMPLEMENTATION ou MODULE");
	

	
}



int lerArray(){
	

	ntoken = yylex();
	lookahead = yytext;
	compilationUnit();
	
 /*
	while(ntoken){


		switch(ntoken){
			case 1 : printf("Palavra Reservada: ");
				break;
			case 2 : printf("Identificador: ");
				break;
		}


		printf("%d: %s \n",ntoken, yytext);
		ntoken = yylex();
	}*/

	return 0;
}

int main(void){
	
	lerArray();
	return 0;

}

