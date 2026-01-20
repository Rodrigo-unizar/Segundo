 //Autores: Razvan Ghita Calangiu 927460 ; Rodrigo Herce Alonso 935413
%{
#include<string.h>
#include "y.tab.h"
%}



%%

                                        //ejemplo de uso de yylval para tokens a los que se
                                        //les ha cambiado su valor semantico en th.y
 /* ALGUNA_EXP_REG                              {yylval.nombre=strdup(yytext);return(ALGUN_TOKEN);} */
"->"            {return(FLECHA);}
[0-3][0-3][0-3]         {yylval.nombre=strdup(yytext); return(NUM);}
"("             {return(PAR_IZQ);}
")"             {return(PAR_DER);}
","             {return(COMA);}
";"             {return(PC);}
"graph"[a-zA-Z0-9_ \n]+         {return(GRAPH);}
"{"             {return(A_LL);}
"}"             {return(C_LL);}  
[ \t]           {/* ignorar espacios y tabuladores */}
\n              {return(EOL);}
%%
