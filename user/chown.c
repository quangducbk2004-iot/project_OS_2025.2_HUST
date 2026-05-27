#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
parse_int(char *s)
{
  int n = 0;
  while(*s){
    if(*s < '0' || *s > '9'){
      printf("chown: invalid number\n");
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
  int uid, gid = -1;
  int i;

  if(argc < 3){
    printf("usage: chown UID[:GID] FILE...\n");
    exit(1);
  }

  char *spec = argv[1];
  char *colon = 0;

  for(char *p = spec; *p; p++){
    if(*p == ':'){
      colon = p;
      *p = '\0';
      break;
    }
  }

  uid = parse_int(spec);

  if(colon){
    gid = parse_int(colon + 1);
  }

  for(i = 2; i < argc; i++){
    if(chown(argv[i], uid, gid) < 0){
      printf("chown: cannot change owner of %s\n", argv[i]);
    }
  }
  exit(0);
}
