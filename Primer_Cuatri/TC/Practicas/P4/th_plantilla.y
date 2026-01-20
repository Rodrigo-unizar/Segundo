 //Autores: Razvan Ghita Calangiu 927460 ; Rodrigo Herce Alonso 935413
%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern int yylex();
extern int yyparse();
int yyerror(char* s);

#define PALOS 3
#define DIM 27
#define BUFF 4000

char* origen = "000";

struct ListaTransiciones {
    char* nodoOrig;
    char* nodosFin[DIM];
    char* etiquetas[DIM]; 
    int total;
} listaTr;

char* tablaTr[DIM][DIM];

void iniTabla(char* tabla[DIM][DIM]) {
    for (int i = 0; i < DIM; i++) {
        for (int j = 0; j < DIM; j++) {
            tabla[i][j] = (char*) calloc(BUFF, sizeof(char));  // <-- asignar memoria
        }
    }
}

/*
 * Calcula la multiplicacion simbolica de matrices 
 * cuadradas DIM x DIM: res = t1*t2
 *
 * CUIDADO: res DEBE SER UNA TABLA DISTINTA A t1 y t2
 * Por ejemplo, NO SE DEBE USAR en la forma:
 *           multiplicar(pot, t, pot); //mal
 */
void multiplicar(char* t1[DIM][DIM], char* t2[DIM][DIM], char* res[DIM][DIM]) {
	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++) {
			res[i][j] = (char*) calloc(BUFF, sizeof(char));
			for (int k = 0; k < DIM; k++) {
				if (strcmp(t1[i][k],"")!=0 && strcmp(t2[k][j],"") != 0) {
					strcat(strcat(res[i][j],t1[i][k]),"-");
					strcat(res[i][j],t2[k][j]);
				}
			}
		}
	}
}


/* 
 *Copia la tabla orig en la tabla copia
*/
void copiar(char* orig[DIM][DIM], char* copia[DIM][DIM]) {
	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++) {
			copia[i][j] = strdup(orig[i][j]);
		}
	}
}

int nodoFila, nodoColumna;
char* transicion;

int transformar(char* nodo){

        if(strlen(nodo) == 3){
                return (((nodo[0] - '0') * 9) + ((nodo[1] - '0')*3) + (nodo[2] -'0'));
        } else {
                return -1;
        }
}

%}

%union{
    char* nombre;
}

%token <nombre> NUM
%token EOL FLECHA PAR_IZQ PAR_DER COMA PC GRAPH C_LL A_LL
%start g

%%
g: GRAPH A_LL EOL s C_LL EOL
    ;

s:  /* nada */
    | NUM FLECHA {nodoFila = transformar($1);} exp PC EOL s  // {nodoFila = transformar($1);}   //pasamos el número de la fila a un índice válido para la matriz   sigue estando dentro de la misma expresion
    ;

exp: NUM PAR_IZQ NUM PAR_DER    {nodoColumna = transformar($1);
                                 transicion = $3;   //guardamos la tras

                                        if(nodoFila >= 0 && nodoColumna >= 0){   //comprobamos que sea un índice válido
                                                tablaTr[nodoFila][nodoColumna] = strdup(transicion);    //copiamos la transición
                                        }
                                }
        | exp COMA exp
        ;

%%


int yyerror(char* s) {
    fprintf(stderr, "Error: %s\n", s);  // <-- pasar argumento correctamente
    return 0;
}

int main() {
    //inicializar lista transiciones
	listaTr.total = 0;

	//inicializar tabla de adyacencia
	iniTabla(tablaTr);
	char* resultado[DIM][DIM];
	iniTabla(resultado);

	//nodo inicial
	char* estadoIni = "000";

	//nodo final
	char* estadoFin = "222";
	
	int error = yyparse();

	int encontrado = 0;
	
	
	if (error == 0) {
		//matriz para guardar la potencia
		char* pot[DIM][DIM];
		copiar(tablaTr,pot);
		//calcular movimientos de estadoIni a estadoFin
		//calculando las potencias sucesivas de tablaTr
		
		for(int i = 1; i < DIM && !encontrado; i++){
			multiplicar(pot, tablaTr, resultado);
			copiar(resultado, pot);
			if(strcmp(resultado[transformar(estadoIni)][transformar(estadoFin)], "") != 0){
				encontrado = 1;
			}
		}


		printf("Nodo inicial  : %s\n", estadoIni);
		//rellenar los ... con los indices adecuados a vuestro codigo
		printf("Movimientos   : %s\n", resultado[transformar(estadoIni)][transformar(estadoFin)]);
		printf("Nodo final    : %s\n", estadoFin);
		
	}

	return error;
}
