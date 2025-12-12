#include "user.h"
#include "datetime.h"   // THIS must be included

int main(int argc, char *argv[]) {
  struct datetime r; // This uses the definition from user/datetime.h

  if (datetime(&r)) {
    printf("datetime failed\n");
    exit(1);
  }

  // Print the date
  // Note: xv6 printf does not support %02d padding, so we use %d
  printf("%d-%d-%d %d:%d:%d\n", r.year, r.month, r.day, r.hour, r.minute, r.second);

  exit(0);
}
