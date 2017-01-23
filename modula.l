%{
#include "modula.h"
%}


%%



"AND"|"ELSIF"|"LOOP"|"REPEAT"|"ARRAY"|"END"|"MOD"|"RETURN"|"BEGIN"|"EXIT"|"MODULE"|"SET"|"BY"|"EXPORT"|"NOT"|"THEN"|"CASE"|"FOR"|"OF"|"TO"|"CONST"|"FROM"|"OR"|"TYPE"|"DEFINITION"|"IF"|"POINTER"|"UNTIL"|"DIV"|"IMPLEMENTATION"|"VAR"|"DO"|"IMPORT"|"QUALIFIED"|"WHILE"|"ELSE"|"IN"|"RECORD"|"WITH" return PALAVRA_RESERVADA;        

"ABS" | "BITSET" | "BOOLEAN" | "CARDINAL" |"CAP" | "CHR" | "CHAR" | "COMPLEX" |"CMPLX" | "DEC" |"DISPOSE" | "EXCL" |"FALSE" | "FLOAT" | "HALT" | "HIGH" |"IM" | "INC" | "INCL" | "INT" |"INTERRUPTIBLE" | "INTEGER" | "LENGTH" | "LFLOAT" |"LONGCOMPLEX" | "LONGREAL" | "MAX" | "MIN" |"NEW"|"NIL" | "ODD" | "ORD" |"PROC" | "PROTECTION" | "RE" | "REAL" |"SIZE" | "TRUE" | "TRUNC" |"UNINTERRUPTIBLE" |"VAL" return IDENTIFICADOR_PADRAO;

[a-zA-Z][_a-zA-Z0-9]*	return 	IDENTIFICADOR;



\"(\\.|[^"])*\" 	return 	STRING;

\'(\\.|[^"])\'   	return 	ONECARACTER;

[ \n\t]			;

.			printf("caracter inexperado\n");

"=" | "#" | "<>" | "<" | "<=" | ">" | ">=" | "IN"  return OPERADOR_RELACIONAL; 

"+" | "-" return OPERADOR;

"*" | "/" | "DIV" | "MOD" | "AND" | "&" return OPERADOR_FACTOR;

"["|"(!"|"]"|"!)"|""|"(:"|""|":)"|"|"|"!" return SIMBOLOS_ALTERNATIVOS; 

[0-9]* return DECIMAL;
[0-7]*B |[0-7]*C return OCTAL;
[0-9A-F]*H return HEXADECIMAL;
                      
              	
%%


int yywrap(void){
	return 1;
}
