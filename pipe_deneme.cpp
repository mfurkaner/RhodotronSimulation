#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <spawn.h>
#include <iostream>

int main(int argc, char** argv) {
    
    int _fd;
    if ( argc > 0  ) {

        //std::cout << "args: " << argv[1] << " " << argv[2] << std::endl;
        if (strcmp(argv[1],"-fd") == 0 ){
            //mkfifo(argv[1], 0666);
            _fd = open(argv[2], O_WRONLY);
            char hello[] = "hello\n";
            write(_fd, hello, strlen(hello));

            close(_fd);
        }

        sleep(10);
    }


}