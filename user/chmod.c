// chmod.c - user-space chmod utility for xv6
// Usage: chmod <octal_mode> <path> [path2 ...]
// Vi du:  chmod 755 hello
//         chmod 000 secret.txt
//         chmod 644 a.txt b.txt

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// Chuyen chuoi octal sang so nguyen
// "755" -> 493 (= 0755)
// "644" -> 420 (= 0644)
static int
parse_octal(const char *s)
{
  int val = 0;
  while(*s >= '0' && *s <= '7'){
    val = val * 8 + (*s - '0');
    s++;
  }
  return val;
}

int
main(int argc, char *argv[])
{
  if(argc < 3){
    fprintf(2, "Usage: chmod <mode> <file> [file...]\n");
    fprintf(2, "  mode: so octal, vi du: 755, 644, 000, 400\n");
    exit(1);
  }

  int mode = parse_octal(argv[1]);
  int i, ret = 0;

  for(i = 2; i < argc; i++){
    if(chmod(argv[i], mode) < 0){
      fprintf(2, "chmod: khong the doi quyen '%s'\n", argv[i]);
      ret = 1;
    }
  }
  exit(ret);
}
