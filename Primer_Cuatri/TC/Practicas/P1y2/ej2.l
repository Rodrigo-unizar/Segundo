%{
	int contador = 0;
%}
%%
hotmail {printf("%s", yytext); contador++;}
%%
int main(){

	int error = yylex();
	if (error == 0) { // si escaner finaliza correctamente
		printf("Total de usuarios: %d\n" , contador );
	}
	return error;
}
