#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
parse_int(char *s)
{
  int n = 0;

  while(*s){
    if(*s < '0' || *s > '9'){
      printf("chgrp: invalid number\n");
      exit(1);
    }
    n = n * 10 + (*s - '0');
    s++;
  }

  return n;
}

int
main(int argc, char *argv[])
{
  if(argc < 3){
    printf("usage: chgrp GID FILE...\n");
    exit(1);
  }

  int gid = parse_int(argv[1]);

  for(int i = 2; i < argc; i++){
    if(chgrp(argv[i], gid) < 0){
      printf("chgrp: cannot change group of %s\n", argv[i]);
    }
  }

  exit(0);
}
