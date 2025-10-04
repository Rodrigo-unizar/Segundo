 //Autores: Razvan Ghita Calangiu   NIP: 927460 ; Rodrigo Herce Alonso   NIP:935413  Practica 1 Ejercicio 7
%%
B(R*UR*UR*UR*)*E  {printf("++%s++", yytext);}
B(R*UR*U)*R*UR*E    {printf("-%s-", yytext);}
B(R*UR*)*R*UR*E {printf(":%s:", yytext);}
%%

