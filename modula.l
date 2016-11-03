%{
#include "modula.h"
%}

oct [0-7]

digito oct|8|9

%%


"AND"|"ELSIF"|"LOOP"|"REPEAT"|"ARRAY"|"END"|"MOD"|"RETURN"|"BEGIN"|"EXIT"|"MODULE"|"SET"|"BY"|"EXPORT"|"NOT"|"THEN"|"CASE"|"FOR"|"OF"|"TO"|"CONST"|"FROM"|"OR"|"TYPE"|"DEFINITION"|"IF"|"POINTER"|"UNTIL"|"DIV"|"IMPLEMENTATION"|"VAR"|"DO"|"IMPORT"|"QUALIFIED"|"WHILE"|"ELSE"|"IN"|"RECORD"|"WITH" return PALAVRA_RESERVADA;        

"ABS" | "BITSET" | "BOOLEAN" | "CARDINAL" |"CAP" | "CHR" | "CHAR" | "COMPLEX" |"CMPLX" | "DEC" |"DISPOSE" | "EXCL" |"FALSE" | "FLOAT" | "HALT" | "HIGH" |"IM" | "INC" | "INCL" | "INT" |"INTERRUPTIBLE" | "INTEGER" | "LENGTH" | "LFLOAT" |"LONGCOMPLEX" | "LONGREAL" | "MAX" | "MIN" |"NEW"|"NIL" | "ODD" | "ORD" |"PROC" | "PROTECTION" | "RE" | "REAL" |"SIZE" | "TRUE" | "TRUNC" |"UNINTERRUPTIBLE" |"VAL" return IDENTIFICADOR_PADRAO;

":"|","|".."|"="|"."|";"|"("|")" return SIMBOLOS_OBRIGATORIOS; 

[a-zA-Z][_a-zA-Z0-9]*	return 	IDENTIFICADOR;

[ \n\t]			;
.			printf("caracter inexperado\n");


                      
              	
%%


int yywrap(void){
	return 1;
}
