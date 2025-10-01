CC = g++			#Compilador
CPPFLAGS = -std=c++11		#Opciones de compilacion
RM = rm -f			#Borrar archivos


all: main
#-----------------Compilacion 1 a 1---------------------------------

contacto.o : contacto.cpp contacto.hpp
	${CC} -c contacto.cpp ${CPPFLAGS}
	
agenda.o: agenda.cpp agenda.hpp
	${CC} -c agenda.cpp ${CPPFLAGS}

main.o: main.cpp 
	${CC} -c main.cpp ${CPPFLAGS}

#-----------------Compilacion total---------------------------------

main: contacto.o agenda.o main.o
	${CC}  main.o agenda.o contacto.o -o main
	
	
#---------------------Limpieza--------------------------------------

clean:
	${RM} contacto.o agenda.o main.o main
