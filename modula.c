#include <stdio.h>
#include "modula.h"


extern int yylex();
extern int yylineno;
extern char* yytext;




int main(void){
	int ntoken,vtoken;

	ntoken = yylex();

	while(ntoken){


		switch(ntoken){
			case 1 : printf("Palavra Reservada: ");
				break;
			case 2 : printf("Identificador: ");
				break;
		}


		printf(" %s\n", yytext);
		ntoken = yylex();
	}

	return 0;

}

