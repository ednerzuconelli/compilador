%{
#include "modula.h"
%}

%%

"AND"|"ELSIF"|"LOOP"|"REPEAT"|"ARRAY"|"END"|"MOD"|"RETURN"|"BEGIN"|"EXIT"|"MODULE"|"SET"|"BY"|"EXPORT"|"NOT"|"THEN"|"CASE"|"FOR"|"OF"|"TO"|"CONST"|"FROM"|"OR"|"TYPE"|"DEFINITION"|"IF"|"POINTER"|"UNTIL"|"DIV"|"IMPLEMENTATION"|"VAR"|"DO"|"IMPORT"|"QUALIFIED"|"WHILE"|"ELSE"|"IN"|"RECORD"|"WITH" return PALAVRA_RESERVADA;        

[a-zA-Z][a-zA-Z0-9]*	return 	IDENTIFICADOR;
[ \n\t]			;
.			printf("caracter inexperado\n");


                      
              	
%%


int yywrap(void){
	return 1;
}
