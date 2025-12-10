#include <fcntl.h>
#include <unistd.h>

int main (int argc, char* argv[]){
    close(1);
    open(argv[1],1 | O_CREAT | O_TRUNC);
    close(2);
    open(argv[1],1);
    execvp(argv[2], &argv[2]);
}