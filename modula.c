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

//cabeçalhos de procedimentos
void program_module();
void reconhecerToken();
void reconhecerNToken();
void simple_const_expr();
void qualident();
void const_expression();

void qualident(){
	reconhecerNToken(IDENTIFICADOR);

	if(strcmp(lookahead,".")==0){
		reconhecerToken(".");
		reconhecerNToken(IDENTIFICADOR);
	}
}

void number(){

}

void string(){

}

void set(){

}


void const_factor(){
	if(ntoken==IDENTIFICADOR){
		qualident();
		if(strcmp(lookahead,"{")==0)
			set();
	}

	if(ntoken==DECIMAL||ntoken==OCTAL||ntoken==HEXADECIMAL){
		number();
	}	

	if(ntoken==STRING||ntoken==ONECARACTER){
		number();
	}

	if(strcmp(lookahead,"{")==0)
		set();
	
	if(strcmp(lookahead,"(")==0){
		reconhecerToken("(");
		const_expression();
		reconhecerToken(")");	
	}

	if(strcmp(lookahead,"NOT")==0)
		const_factor();
}



void const_term(){
	const_factor();

	while(ntoken==OPERADOR_FACTOR){
		reconhecerNToken(OPERADOR_FACTOR);
		const_factor();
	}
}



void priority(){
	simple_const_expr();		
}

void simple_const_expr(){
	if(ntoken==OPERADOR){
		reconhecerNToken(OPERADOR);
	}

	const_term();

	if(ntoken==OPERADOR || strcmp(lookahead,"OR")==0){
		reconhecerNToken(OPERADOR);
		const_term();
	}
};

void const_expression(){
	simple_const_expr();

	if(ntoken==OPERADOR_RELACIONAL){
		reconhecerNToken(OPERADOR_RELACIONAL);
		simple_const_expr();
	}
}

void constant_declaration(){
	reconhecerNToken(IDENTIFICADOR);
	reconhecerToken("=");
	const_expression();
}



void definition(){
	while(strcmp(lookahead,"CONST")==0 || strcmp(lookahead,"TYPE")==0 || strcmp(lookahead,"VAR")==0 || strcmp(lookahead,"PROCEDURE")==0){

	if(strcmp(lookahead,"CONST")==0){
		reconhecerToken("CONST");
		while(ntoken==IDENTIFICADOR){
		//const declaration
		reconhecerToken(";");
		}
	}


	if(strcmp(lookahead,"TYPE")==0){
		reconhecerToken("TYPE");

		while(ntoken==IDENTIFICADOR){
			reconhecerNToken(IDENTIFICADOR);
	
			if(strcmp(lookahead,"=")==0){
				reconhecerToken("=");
				//type
			}
		
			reconhecerToken(";");
		}
	}

	if(strcmp(lookahead,"VAR")==0){
		reconhecerToken("VAR");

		/* reconhecer VAR
		while(ntoken==IDENTIFICADOR){
			reconhecerNToken(IDENTIFICADOR);
	
			if(strcmp(lookahead,"=")==0){
				reconhecerToken("=");
				//type
			}
		
			reconhecerToken(";");
		}*/
	}

	if(strcmp(lookahead,"PROCEDURE")==0){
		reconhecerToken("PROCEDURE");

		while(strcmp(lookahead,"PROCEDURE")==0){
			//procedure _heading
			reconhecerToken(";");
		}
	}

	}

}

void import(){
	if(strcmp(lookahead,"FROM")==0){
		reconhecerToken("FROM");
		reconhecerNToken(IDENTIFICADOR);
	}

	reconhecerToken("IMPORT");
	reconhecerNToken(IDENTIFICADOR);

	while(strcmp(lookahead,",")==0){
		reconhecerToken(",");
		reconhecerNToken(IDENTIFICADOR);
	}

	reconhecerToken(";");
}

void export(){
	reconhecerToken("EXPORT");

	if(strcmp(lookahead,"QUALIFIED")==0){
		reconhecerToken("QUALIFIED");
	}

	reconhecerNToken(IDENTIFICADOR);
	
	while(strcmp(lookahead,",")==0){
		reconhecerToken(",");
		reconhecerNToken(IDENTIFICADOR);
	}
	
	reconhecerToken(";");
}

void reconhecerToken(char* token){
	if(strcmp(lookahead,token)==0){
		lerArray();
	}else{
		erro(token);
	}
}

void reconhecerNToken(int nToken){
	if(ntoken == nToken){
		lerArray();
	}
}

void definition_module(){
	reconhecerToken("DEFINITION");
	reconhecerToken("MODULE");

	reconhecerNToken(IDENTIFICADOR);

	reconhecerToken(";");

	while(strcmp(lookahead,"FROM")==0 || strcmp(lookahead,"IMPORT")==0){
		import();
	}

	if(strcmp(lookahead,"EXPORT")==0){
		export();
	}

	definition();
	
	reconhecerToken("END");

	reconhecerNToken(IDENTIFICADOR);

	reconhecerToken(".");
	
}

void implementation_module(){
	reconhecerToken("IMPLEMENTATION");
	program_module();
}

void program_module(){
	reconhecerToken("MODULE");

	reconhecerNToken(IDENTIFICADOR);

	if(strcmp(lookahead,"[")==0){
		reconhecerToken("[");
		priority();
		reconhecerToken("]");		
	}

	reconhecerToken(";");
	
	while(strcmp(lookahead,"FROM")==0 || strcmp(lookahead,"IMPORT")==0){
		import();
	}


	//REGRA BLOCK
	
	reconhecerNToken(IDENTIFICADOR);

	reconhecerToken(".");
	

	

	printf("reconheceu module e o indetificador e o ; o proximo token é: %s",yytext);
}

int compilationUnit(){
	if(strcmp(lookahead,"DEFINITION")==0){
		definition_module();
		return 0;
	}

	if(strcmp(lookahead,"IMPLEMENTATION")==0){	
		implementation_module();
		return 0;
	}

	if(strcmp(lookahead,"MODULE")==0){	
		program_module();
		return 0;
	}
	

	
}



int lerArray(){
	

	ntoken = yylex();
	lookahead = yytext;
	compilationUnit();

	return 0;
}

int main(void){
	
	lerArray();
	return 0;

}

