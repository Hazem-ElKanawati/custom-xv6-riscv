#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAX 3000  // Max digits supported for large factorials

// Store digits in reverse order for multiplication
static int res[MAX];

int main(int argc, char *argv[]) {
    // Help feature
    if (argc == 2 && strcmp(argv[1], "?") == 0) {
        printf("Usage: fact <non-negative integer>\n");
        exit(0);
    }

    // Check argument count
    if (argc != 2) {
        printf("Error: You must provide exactly one number\n");
        exit(1);
    }

    // Detect negative numbers manually (xv6 atoi does not handle '-')
    for (int i = 0; argv[1][i] != '\0'; i++) {
        if (argv[1][i] == '-') {
            printf("Error: factorial is not defined for negative numbers.\n");
            exit(1);
        }
        // Optional: ensure all characters are digits
        if (argv[1][i] < '0' || argv[1][i] > '9') {
            printf("Error: Input must be a non-negative integer\n");
            exit(1);
        }
    }

    int n = atoi(argv[1]);

    // Optional: limit maximum factorial to avoid huge memory usage
    if (n > 1000) {
        printf("Error: Maximum supported input is 1000\n");
        exit(1);
    }

    // Initialize result = 1
    res[0] = 1;
    int res_size = 1;

    // Compute factorial using big integer array
    for (int x = 2; x <= n; x++) {
        int carry = 0;
        for (int i = 0; i < res_size; i++) {
            int val = res[i] * x + carry;
            res[i] = val % 10;
            carry = val / 10;
        }
        while (carry) {
            res[res_size] = carry % 10;
            carry /= 10;
            res_size++;
        }
    }

    // Print factorial result
    printf("%d! = ", n);
    for (int i = res_size - 1; i >= 0; i--) {
        printf("%d", res[i]);
    }
    printf("\n");

    exit(0);
}
