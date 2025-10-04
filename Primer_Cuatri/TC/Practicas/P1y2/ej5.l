%{
 //Autores: Razvan Ghita Calangiu   NIP: 927460 ; Rodrigo Herce Alonso   NIP: 935413  Practica 1 Ejercicio 5 
	int c=0, l=0, lv=0, v=0, m=0, d=0, n = 0;
%}
%%

\n {l++;}
[0-9]*[:][0-9]* {v++; c = c + yyleng; d++;
			if(yyleng > m){
				m = yyleng;}}

([a-zA-Z:][a-zA-Z: ]*[a-zA-Z:])|[a-zA-Z:] {v++; c= c + yyleng;
						n++;
						if(yyleng > m){
							m = yyleng;}}
([a-zA-Z:0-9][a-zA-Z:0-9 ]*[a-zA-Z:0-9])|[a-zA-Z:0-9] {v++; c = c + yyleng;
							if(yyleng > m){
								m = yyleng;}}
^[ ]*\n {lv++; l++; c = c + yyleng - 1;}
. {c = c + yyleng;}

%%
int main(){

	int error = yylex();
	if(error == 0){
		printf("C: %d\n", c);
		printf("L: %d\n", l);
		printf("LV: %d\n", lv);
		printf("V: %d\n", v);
		printf("M: %d\n", m);
		printf("D: %d\n", d);
		printf("N: %d\n", n);
	}

}
