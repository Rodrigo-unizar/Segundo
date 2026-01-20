int leerlinea(int fd, char* s){
    int i, j;
    char c;

    j = 0;
    i = read(fd, c, 1);
    while(i != 0 && c != '\n'){
        s[j] = c;
        j++;
        i = read(fd,c,1);
    }
    s[j] = '\0';
    return(j);
}
void trocearlineas(){
    char* lineas[10], *comando[10];
    comando[0] = strtok(lineas[1], " \t");
    for(int k = 1; (comando[k] = strtok(NULL, " \t")) != NULL; k++);
}