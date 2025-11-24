#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

// Compare only the last component of a path: /a/b/c -> c
char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash
  for (p = path + strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return name padded to DIRSIZ
  if(strlen(p) >= DIRSIZ)
    return p;

  memmove(buf, p, strlen(p));
  buf[strlen(p)] = 0;
  return buf;
}

void
find(char *path, char *target)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  // Open the directory
  fd = open(path, 0);
  if(fd < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  // Stat the directory
  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  // If it's a file, just compare the name
  if(st.type == T_FILE){
    if(strcmp(fmtname(path), target) == 0){
      printf("%s\n", path);
    }
    close(fd);
    return;
  }

  // If directory, iterate entries
  if(st.type == T_DIR){
    // Build path prefix
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("find: path too long\n");
      close(fd);
      return;
    }

    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';

    // Read entries inside the directory
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;

      // Skip "." and ".."
      if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
        continue;

      // Append entry name
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;

      // Stat the entry
      if(stat(buf, &st) < 0){
        printf("find: cannot stat %s\n", buf);
        continue;
      }

      // If filename matches target, print it
      if(strcmp(de.name, target) == 0){
        printf("%s\n", buf);
      }

      // If directory -> recurse
      if(st.type == T_DIR){
        find(buf, target);
      }
    }
  }

  close(fd);
}

int
main(int argc, char *argv[])
{
  if(argc != 3){
    fprintf(2, "Usage: find <directory> <filename>\n");
    exit(1);
  }

  find(argv[1], argv[2]);
  exit(0);
}
