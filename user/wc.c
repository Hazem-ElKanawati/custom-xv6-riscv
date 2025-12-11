// user/wc.c
// Enhanced wc for xv6: supports -l, -w, -c, -m and multiple files.
// Defaults to printing lines, words, bytes (like typical wc).

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

#define BUF 512

struct counts {
  uint64 lines;
  uint64 words;
  uint64 bytes;
  uint64 chars; // same as bytes for ASCII
};

static int is_space(char c) {
  return c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == '\v' || c == '\f';
}

static void count_buffer(char *buf, int n, struct counts *ct, int *in_word) {
  int i;
  ct->bytes += (uint64)n;
  ct->chars += (uint64)n; // ASCII == bytes
  for (i = 0; i < n; i++) {
    char c = buf[i];
    if (c == '\n') ct->lines++;
    if (is_space(c)) {
      if (*in_word) *in_word = 0;
    } else {
      if (!*in_word) {
        ct->words++;
        *in_word = 1;
      }
    }
  }
}

static int count_fd(int fd, struct counts *ct) {
  char buf[BUF];
  int n;
  int in_word = 0;
  ct->lines = ct->words = ct->bytes = ct->chars = 0;
  while ((n = read(fd, buf, sizeof(buf))) > 0) {
    count_buffer(buf, n, ct, &in_word);
  }
  if (n < 0) return -1;
  return 0;
}

static int count_file(const char *path, struct counts *ct) {
  if (path[0] == '-' && path[1] == '\0') {
    return count_fd(0, ct); // stdin
  }
  int fd = open((char*)path, 0);
  if (fd < 0) return -1;
  int rc = count_fd(fd, ct);
  close(fd);
  return rc;
}

static void print_counts(struct counts *ct, int show_l, int show_w, int show_c, int show_m, const char *name) {
  int any = show_l || show_w || show_c || show_m;
  if (!any) { // default: l, w, c
    show_l = show_w = show_c = 1;
  }

  if (show_l) printf("%d ", (int)ct->lines);
  if (show_w) printf("%d ", (int)ct->words);
  if (show_c) printf("%d ", (int)ct->bytes);
  if (show_m) printf("%d ", (int)ct->chars);

  if (name)
    printf("%s", name);

  printf("\n");
}

int
main(int argc, char *argv[])
{
  int show_l = 0, show_w = 0, show_c = 0, show_m = 0;
  int i, file_start = 1;

  // Parse flags (combined like -lw). Stop at first non-flag argument or lone "-" (filename).
  for (i = 1; i < argc; i++) {
    char *s = argv[i];
    if (s[0] != '-' || s[1] == '\0') { // not a flag (or single "-" filename)
      break;
    }
    // If argument is exactly "--", treat as end of flags
    if (s[1] == '-' && s[2] == '\0') {
      i++;
      break;
    }
    // parse combined flags
    int j;
    for (j = 1; s[j]; j++) {
      switch (s[j]) {
        case 'l': show_l = 1; break;
        case 'w': show_w = 1; break;
        case 'c': show_c = 1; break;
        case 'm': show_m = 1; break;
        default:
          printf("Usage: wc [-lwc m] [file...]\n");
          exit(1);
      }
    }
    file_start = i + 1;
  }

  int files = argc - file_start;
  if (files <= 0) {
    // no files: read stdin
    struct counts ct;
    if (count_fd(0, &ct) < 0) {
      printf("wc: read error\n");
      exit(1);
    }
  print_counts(&ct, show_l, show_w, show_c, show_m, 0);
    exit(0);
  }

  struct counts total;
  total.lines = total.words = total.bytes = total.chars = 0;
  int multiple = (files > 1);

  for (i = file_start; i < argc; i++) {
    const char *name = argv[i];
    struct counts ct;
    if (count_file(name, &ct) < 0) {
      printf("wc: cannot open %s\n", name);
      continue;
    }

    print_counts(&ct, show_l, show_w, show_c, show_m, name);

    total.lines += ct.lines;
    total.words += ct.words;
    total.bytes += ct.bytes;
    total.chars += ct.chars;
  }

  if (multiple) {
    print_counts(&total, show_l, show_w, show_c, show_m, "total");
  }

  exit(0);
}
