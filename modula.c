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
void element();
void type();
void simple_type();
void field_list();
void block();
void expression();
void statement();
void designator();
void case_labels2();

void qualident(){


	reconhecerNToken(IDENTIFICADOR);

	if(strcmp(lookahead,".")==0){
		reconhecerToken(".");
		reconhecerNToken(IDENTIFICADOR);
	}
}



void number(){
	char* lah;

	if(ntoken==DECIMAL){
		
		 reconhecerNToken(DECIMAL);

		if(strcmp(lookahead,".")==0){
			reconhecerToken(".");
			reconhecerNToken(DECIMAL);
		}
	}

	if(ntoken==OCTAL){
		reconhecerNToken(OCTAL);
	}

	if(ntoken==HEXADECIMAL){
		reconhecerNToken(HEXADECIMAL);
	}
}

void string(){
	if(ntoken==STRING){
		reconhecerNToken(STRING);
	}

	if(ntoken==ONECARACTER){
		reconhecerNToken(ONECARACTER);
	}
}

void element(){
	
	const_expression();
	if(strcmp(lookahead,".")==0){
		reconhecerToken(".");
		reconhecerToken(".");
		const_expression();
	}

}

void set(){
	reconhecerToken("{");
	element();
	reconhecerToken("}");
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
		string();
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

	while(ntoken==OPERADOR || strcmp(lookahead,"OR")==0){
		if(ntoken==OPERADOR)
			reconhecerNToken(OPERADOR);

		if(strcmp(lookahead,"OR")==0)
			reconhecerToken("OR");
	
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

void case_labels(){
	const_expression();
	
	if(strcmp(lookahead,"..")==0){
		reconhecerToken("..");
		const_expression();
	}
	
}

void variant(){
	case_labels();

	while(strcmp(lookahead,",")==0){
			reconhecerToken(",");
			case_labels();
	}

	reconhecerToken(":");

	field_list();

	while(strcmp(lookahead,";")==0){
			reconhecerToken(";");
			field_list();
	}
}

void field_list(){
	if(strcmp(lookahead,"CASE")==0){
		reconhecerToken("CASE");
		if(ntoken==IDENTIFICADOR){
			reconhecerNToken(IDENTIFICADOR);
			reconhecerToken("=");
		}

		qualident();

		reconhecerToken("OF");

		variant();

		while(strcmp(lookahead,"|")==0){
			reconhecerToken("|");
			variant();
		}

		if(strcmp(lookahead,"ELSE")==0){

			reconhecerToken("ELSE");
			field_list();

			while(strcmp(lookahead,";")==0){
				field_list();
			}
		}

		reconhecerToken("END");
			
	}


	if(ntoken==IDENTIFICADOR){
		reconhecerNToken(IDENTIFICADOR);

		while(strcmp(lookahead,",")==0){
				reconhecerToken(",");
				reconhecerNToken(IDENTIFICADOR);
		}

		reconhecerToken(":");

		type();
	}
}

void formal_type(){
	if(strcmp(lookahead,"ARRAY")==0){
		reconhecerToken("ARRAY");
		reconhecerToken("OF");
	}

	qualident();
}

void array_type(){
	reconhecerToken("ARRAY");
	simple_type();

	while(strcmp(lookahead,",")==0){
	
		reconhecerToken(",");
		simple_type();
	}

	reconhecerToken("OF");

	type();
}

void record_type(){
	reconhecerToken("RECORD");

	field_list();

	while(strcmp(lookahead,";")==0){
		reconhecerToken(";");
		field_list();
	}

	reconhecerToken("END");
	
}

void set_type(){
	reconhecerToken("SET");
	reconhecerToken("OF");
	simple_type();
}

void pointer_type(){
	reconhecerToken("POINTER");
	reconhecerToken("TO");
	type();
}

void procedure_type(){
	reconhecerToken("PROCEDURE");

	if(strcmp(lookahead,"(")==0){
		reconhecerToken("(");
		
		if(strcmp(lookahead,"ARRAY")==0||ntoken==IDENTIFICADOR||strcmp(lookahead,"VAR")==0){
			if(strcmp(lookahead,"VAR")==0){
				reconhecerToken("VAR");
			}
			
			formal_type();

			while(strcmp(lookahead,",")==0){
				reconhecerToken(",");
				
				if(strcmp(lookahead,"VAR")==0){	
					reconhecerToken("VAR");
				}

				formal_type();
				
			}
			
			
		}

		reconhecerToken(")");

		if(strcmp(lookahead,":")==0){
			reconhecerToken(":");
			qualident();
		}
	}

	

}

void enumeration(){
	reconhecerToken("(");
	reconhecerNToken(IDENTIFICADOR);

	while(strcmp(lookahead,",")==0){
	
		reconhecerToken(",");
		reconhecerNToken(IDENTIFICADOR);	
	}

	reconhecerToken(")");
	
}

void subrange_type(){
	reconhecerToken("[");
	const_expression();
	reconhecerToken("..");
	const_expression();
	reconhecerToken("]");
}



void simple_type(){
	if(strcmp(lookahead,"(")==0){
		enumeration();	
	}

	if(strcmp(lookahead,"[")==0){
		subrange_type();	
	}

	if(ntoken==IDENTIFICADOR){
		qualident();
	}

	
}

void type(){
	if(strcmp(lookahead,"ARRAY")==0){
		array_type();	
	}

	if(strcmp(lookahead,"RECORD")==0){
		record_type();	
	}

	if(strcmp(lookahead,"SET")==0){
		set_type();	
	}

	if(strcmp(lookahead,"POINTER")==0){
		pointer_type();	
	}

	if(strcmp(lookahead,"PROCEDURE")==0){
		procedure_type();	
	}

	if(ntoken==IDENTIFICADOR||strcmp(lookahead,"(")==0||strcmp(lookahead,"[")==0)
		simple_type();
	


	
}


void variable_declaration(){
	reconhecerNToken(IDENTIFICADOR);

	while(strcmp(lookahead,",")==0){
	
		reconhecerToken(",");
		reconhecerNToken(IDENTIFICADOR);	
	}

	reconhecerToken(":");

	type();

}

void fp_section(){
	if(strcmp(lookahead,"VAR")==0){
		reconhecerToken("VAR");
		
	}

	reconhecerNToken(IDENTIFICADOR);

	while(strcmp(lookahead,",")==0){
		reconhecerToken(",");
		reconhecerNToken(IDENTIFICADOR);
	}	

	reconhecerToken(":");

	if(strcmp(lookahead,"ARRAY")==0){
		reconhecerToken("ARRAY");
		reconhecerToken("OF");
	}

	qualident();
}

void formal_parameters(){
	
	reconhecerToken("(");
	fp_section();

	while(strcmp(lookahead,";")==0){
	
		reconhecerToken(";");
		fp_section();
	}

	reconhecerToken(")");

	if(strcmp(lookahead,":")==0){
		reconhecerToken(":");
		qualident();	
	}
}

void procedure_heading(){
	reconhecerToken("PROCEDURE");
	reconhecerNToken(IDENTIFICADOR);

	if(strcmp(lookahead,"(")==0){
		formal_parameters();
	}
}


void definition(){
	while(strcmp(lookahead,"CONST")==0 || strcmp(lookahead,"TYPE")==0 || strcmp(lookahead,"VAR")==0 || strcmp(lookahead,"PROCEDURE")==0){

	if(strcmp(lookahead,"CONST")==0){
		reconhecerToken("CONST");
		while(ntoken==IDENTIFICADOR){
			constant_declaration();
			reconhecerToken(";");
		}
	}


	if(strcmp(lookahead,"TYPE")==0){
		reconhecerToken("TYPE");

		while(ntoken==IDENTIFICADOR){
			reconhecerNToken(IDENTIFICADOR);
	
			if(strcmp(lookahead,"=")==0){
				reconhecerToken("=");
				type();
			}
		
			reconhecerToken(";");
		}
	}

	if(strcmp(lookahead,"VAR")==0){
		reconhecerToken("VAR");
		variable_declaration();
		reconhecerToken(";");
		
	}

	if(strcmp(lookahead,"PROCEDURE")==0){
		
			procedure_heading();
			reconhecerToken(";");
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

void type_declaration(){
	reconhecerNToken(IDENTIFICADOR);
	reconhecerToken("=");
	type();
}

void procedure_declaration(){
	procedure_heading();
	reconhecerToken(";");
	block();	
	reconhecerNToken(IDENTIFICADOR);
}

void module_declaration(){
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

	if(strcmp(lookahead,"EXPORT")==0){
		export();
	}

	block();
	
	reconhecerNToken(IDENTIFICADOR);

}

void declaration(){

	if(strcmp(lookahead,"CONST")==0){
		reconhecerToken("CONST");
		while(ntoken==IDENTIFICADOR){
			constant_declaration();
			reconhecerToken(";");
		}
	}


	if(strcmp(lookahead,"TYPE")==0){
		reconhecerToken("TYPE");

		while(ntoken==IDENTIFICADOR){
			type_declaration();
			reconhecerToken(";");
		}
	}

	if(strcmp(lookahead,"VAR")==0){
		reconhecerToken("VAR");

		while(ntoken==IDENTIFICADOR){
			variable_declaration();
			reconhecerToken(";");
		}
		
	}

	if(strcmp(lookahead,"PROCEDURE")==0){
		procedure_declaration();
		reconhecerToken(";");
	}

	if(strcmp(lookahead,"MODULE")==0){
		module_declaration();
		reconhecerToken(";");
	}
}

void if_statement(){
	
	reconhecerToken("IF");
	expression();
	reconhecerToken("THEN");
	statement();
	
	while(strcmp(lookahead,"FROM")==0){
		reconhecerToken(";");
		statement();
	}

	if(strcmp(lookahead,"ELSIF")==0){
		reconhecerToken("ELSIF");

		expression();
		reconhecerToken("THEN");
		statement();
	
		while(strcmp(lookahead,"FROM")==0){
			reconhecerToken(";");
			statement();
		}
	}

	if(strcmp(lookahead,"ELSE")==0){
		statement();
	
		while(strcmp(lookahead,"FROM")==0){
			reconhecerToken(";");
			statement();
		}
	}

	reconhecerToken("END");
}

void case_labels2(){
	const_expression();

	while(strcmp(lookahead,"..")==0){
		reconhecerToken("..");
		const_expression();
	}
}

void case2(){
	case_labels2();

	while(strcmp(lookahead,",")==0){
		reconhecerToken(",");
		case_labels2();
	}

	reconhecerToken(":");
	statement();
	
	while(strcmp(lookahead,";")==0){
		reconhecerToken(";");
		statement();
	}
}

void case_statement(){
	reconhecerToken("CASE");
	expression();
	reconhecerToken("OF");
	case2();

	while(strcmp(lookahead,"|")==0){
		reconhecerToken("|");
		case2();
	}

	if(strcmp(lookahead,"ELSE")==0){
		reconhecerToken("ELSE");
		statement();

		while(strcmp(lookahead,";")==0){
			reconhecerToken(";");
			statement();
		}
	}

	reconhecerToken("END");
}

void while_statement(){
	reconhecerToken("WHILE");
	expression();
	reconhecerToken("DO");
	statement();

	while(strcmp(lookahead,";")==0){
			reconhecerToken(";");
			statement();
	}

	reconhecerToken("END");
	
}

void repeat_statement(){
	reconhecerToken("REPEAT");
	statement();

	while(strcmp(lookahead,";")==0){
			reconhecerToken(";");
			statement();
	}

	reconhecerToken("UNTIL");
	expression();
}

void loop_statement(){
	reconhecerToken("LOOP");
	statement();

	while(strcmp(lookahead,";")==0){
			reconhecerToken(";");
			statement();
	}

	reconhecerToken("END");

}

void for_statement(){
	reconhecerToken("FOR");
	reconhecerNToken(IDENTIFICADOR);
	reconhecerToken(":=");

	expression();
	reconhecerToken("TO");
	expression();

	if(strcmp(lookahead,"BY")==0){
		reconhecerToken("BY");
		const_expression();
	}

	reconhecerToken("DO");
	statement();

	while(strcmp(lookahead,";")==0){
			reconhecerToken(";");
			statement();
	}

	reconhecerToken("END");
}

void with_statement(){
	reconhecerToken("WITH");
	designator();
	reconhecerToken("DO");

	statement();

	while(strcmp(lookahead,";")==0){
			reconhecerToken(";");
			statement();
	}

	reconhecerToken("END");
}

void designator(){
	qualident();
	
	while(strcmp(lookahead,".")==0||strcmp(lookahead,"[")==0||strcmp(lookahead,"^")==0){

		
		if(strcmp(lookahead,".")==0){
			reconhecerToken(".");
			reconhecerNToken(IDENTIFICADOR);
		}

		if(strcmp(lookahead,"[")==0){
			reconhecerToken("[");
			expression();
	
			while(strcmp(lookahead,",")==0){
				reconhecerToken(",");
				expression();
			}

			reconhecerToken("]");
		}

		if(strcmp(lookahead,"^")==0){
			reconhecerToken("^");
		}
	}
}


void factor(){
	if(ntoken==IDENTIFICADOR){
		qualident();

		if(strcmp(lookahead,"{")==0)
			set();

		if(strcmp(lookahead,".")==0 || strcmp(lookahead,"[")==0 || strcmp(lookahead,"^")==0){
			if(strcmp(lookahead,".")==0){
				reconhecerToken(".");
				reconhecerNToken(IDENTIFICADOR);
			}

			if(strcmp(lookahead,"[")==0){
				reconhecerToken("[");
				expression();	
				while(strcmp(lookahead,",")==0){
					reconhecerToken(",");
					expression();
				}

				reconhecerToken("]");
			}

			if(strcmp(lookahead,"^")==0){
				reconhecerToken("^");
			}
	
			if(strcmp(lookahead,"(")==0){
				reconhecerToken("(");

				expression();
				while(strcmp(lookahead,",")==0){
					reconhecerToken(",");
					expression();
				}

				reconhecerToken(")");
				
			}
			

		}

	}

	if(ntoken==DECIMAL||ntoken==OCTAL||ntoken==HEXADECIMAL){
		number();
	}	

	if(ntoken==STRING||ntoken==ONECARACTER){
		string();
	}

	if(strcmp(lookahead,"{")==0)
		set();
	
	if(strcmp(lookahead,"(")==0){
		reconhecerToken("(");
		expression();
		reconhecerToken(")");	
	}

	if(strcmp(lookahead,"NOT")==0)
		factor();
}

void term(){
	factor();

	while(ntoken==OPERADOR_FACTOR){
		reconhecerNToken(OPERADOR_FACTOR);
		factor();
	}
}

void simple_expression(){

	
	if(ntoken==OPERADOR){
		reconhecerNToken(OPERADOR);
	}



	term();



	while(strcmp(lookahead,"+")==0 || strcmp(lookahead,"OR")==0||strcmp(lookahead,"-")==0){
		if(strcmp(lookahead,"+")==0)
			reconhecerToken("+");

		if(strcmp(lookahead,"-")==0)
			reconhecerToken("-");

		if(strcmp(lookahead,"OR")==0)
			reconhecerToken("OR");

	

		term();
	}

	
}

void expression(){
	simple_expression();

	if(ntoken==OPERADOR_RELACIONAL){
		reconhecerNToken(OPERADOR_RELACIONAL);	
		simple_expression();
	}

}

void statement(){
	if(strcmp(lookahead,"IF")==0){
		if_statement();
	}

	if(strcmp(lookahead,"CASE")==0){
		case_statement();
	}

	if(strcmp(lookahead,"WHILE")==0){
		while_statement();
	}

	if(strcmp(lookahead,"REPEAT")==0){
		repeat_statement();
	}

	if(strcmp(lookahead,"LOOP")==0){
		loop_statement();
	}

	if(strcmp(lookahead,"FOR")==0){
		for_statement();
	}

	if(strcmp(lookahead,"WITH")==0){
		with_statement();
	}

	if(ntoken==IDENTIFICADOR){
		designator();

		if(strcmp(lookahead,"(")==0){
			reconhecerToken("(");
			if(!strcmp(lookahead,")")==0){
				expression();
				
				while(strcmp(lookahead,",")==0){
					reconhecerToken(",");
					expression();
				}
			}
			reconhecerToken(")");
		}else if(strcmp(lookahead,":=")==0){
			reconhecerToken(":=");
			expression();
		}
	}
	
	if(strcmp(lookahead,"EXIT")==0){
		reconhecerToken("EXIT");
	}

	if(strcmp(lookahead,"RETURN")==0){
		reconhecerToken("RETURN");

		if(!strcmp(lookahead,")")==0){
			expression();
		}
	}
}

void block(){
	while(strcmp(lookahead,"CONST")==0||strcmp(lookahead,"TYPE")==0||strcmp(lookahead,"VAR")==0||strcmp(lookahead,"PROCEDURE")==0||strcmp(lookahead,"MODULE")==0){
		declaration();
	}

	if(strcmp(lookahead,"BEGIN")==0){
		reconhecerToken("BEGIN");
		statement();

		while(strcmp(lookahead,";")==0){
			reconhecerToken(";");
			statement();
		}	
		
		
	}

	reconhecerToken("END");
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


	block();
	
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
	printf("\no proximo token é: %s",yytext);
		
	

	

	return 0;
}

int main(/*int argc, char **argv*/){
	
/*	if(strcmp(argv[1],"?")==0||strcmp(argv[1],"-?")==0||strcmp(argv[1],"/?")==0){
		printf("\nComo executar: ./executável < arquivo de entrada.txt\n");
	}else{*/
		lerArray();
		compilationUnit();
	//}
	return 0;

}

