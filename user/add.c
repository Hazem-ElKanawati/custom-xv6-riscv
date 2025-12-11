#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// Safe atoi with validation
// Returns 1 on success, 0 on failure
int atoi_safe(char *s, int *out) {
    int n = 0;
    int neg = 0;
    char *p = s;

    if (*p == '-') {
        neg = 1;
        p++;
    }

    if (*p == '\0') return 0; // string was just "-" -> invalid

    while (*p) {
        if (*p < '0' || *p > '9') return 0; // invalid character
        n = n * 10 + (*p - '0');
        p++;
    }

    if (neg) n = -n;
    *out = n;
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc == 2 && strcmp(argv[1], "?") == 0) {
        printf("Usage: add <number1> <number2>\n");
        exit(0);
    }

    if (argc != 3) {
        printf("Error: add requires exactly 2 numbers\n");
        exit(1);
    }

    int num1, num2;

    if (!atoi_safe(argv[1], &num1) || !atoi_safe(argv[2], &num2)) {
        printf("Error: arguments must be valid integers\n");
        exit(1);
    }

    int total = num1 + num2;
    printf("%d + %d = %d\n", num1, num2, total);

    exit(0);
}
