#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
parse_octal(char *s)
{
  int n = 0;

  while(*s){
    if(*s < '0' || *s > '7'){
      printf("umask: invalid octal\n");
      exit(1);
    }
    n = n * 8 + (*s - '0');
    s++;
  }

  return n;
}

int
main(int argc, char *argv[])
{
  if(argc == 1){
    int old = umask(0);
    umask(old);

    printf("%d%d%d\n",
      (old >> 6) & 7,
      (old >> 3) & 7,
      old & 7);
  } else {
    int mask = parse_octal(argv[1]);
    umask(mask);

    printf("umask: set to 0%d%d%d\n",
      (mask >> 6) & 7,
      (mask >> 3) & 7,
      mask & 7);
  }

  exit(0);
}
