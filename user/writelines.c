#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int main() {
    int fd = open("testfile.txt", O_CREATE | O_WRONLY);
    if(fd < 0){
        printf("Cannot create file\n");
        exit(1);
    }

    char *lines[] = {
        "line1\n", "line2\n", "line3\n", "line4\n", "line5\n",
        "line6\n", "line7\n", "line8\n", "line9\n", "line10\n", "line11\n"
    };

    for(int i = 0; i < 11; i++){
        write(fd, lines[i], strlen(lines[i]));
    }

    close(fd);
    exit(0);
}
