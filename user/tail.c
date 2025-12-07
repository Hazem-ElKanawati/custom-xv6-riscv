#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

#define MAXLINE 128

int main(int argc, char *argv[]) {
    int n = 10;           // default: last 10 lines
    char *filename = 0;

    // Parse arguments
    if (argc > 1 && strcmp(argv[1], "-n") == 0) {
        if (argc < 4) {
            printf("Usage: tail [-n number] <file>\n");
            exit(1);
        }
        n = atoi(argv[2]);
        if (n <= 0) {
            printf("tail: invalid number of lines: %s\n", argv[2]);
            exit(1);
        }
        filename = argv[3];
    } else if (argc > 1) {
        filename = argv[1];
    }

    int fd = 0; // default stdin
    if (filename) {
        fd = open(filename, 0);
        if (fd < 0) {
            printf("tail: cannot open %s\n", filename);
            exit(1);
        }
    }

    // First pass: count total lines
    char c;
    int total_lines = 0;
    while (read(fd, &c, 1) == 1) {
        if (c == '\n') total_lines++;
    }

    int skip = total_lines - n;
    if (skip < 0) skip = 0;

    // Second pass: rewind file
    if (filename) {
        close(fd);
        fd = open(filename, 0);
    }

    int current_line = 0;
    char buf[MAXLINE];
    int idx = 0;

    while (read(fd, &c, 1) == 1) {
        if (c != '\n' && idx < MAXLINE-1) {
            buf[idx++] = c;
        } else {
            buf[idx] = '\0';
            if (current_line >= skip) {
                printf("%s\n", buf);
            }
            idx = 0;
            current_line++;
        }
    }
    // Print last line if file does not end with newline
    if (idx > 0 && current_line >= skip) {
        buf[idx] = '\0';
        printf("%s\n", buf);
    }

    if (filename) close(fd);
    exit(0);
}
