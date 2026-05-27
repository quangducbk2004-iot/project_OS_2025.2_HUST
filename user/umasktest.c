#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int
main(int argc, char *argv[])
{
  struct stat st;
  int fd;

  printf("=== UMASK TEST ===\n\n");

  printf("Test 1: Default umask 022\n");
  fd = open("u1.txt", O_CREATE | O_WRONLY);
  if(fd < 0){
    printf("FAIL: cannot create u1.txt\n");
    exit(1);
  }
  close(fd);

  if(stat("u1.txt", &st) == 0){
    printf("  Mode: 0%d%d%d, expect 0644\n",
      (st.mode >> 6) & 7,
      (st.mode >> 3) & 7,
      st.mode & 7);

    if(st.mode == 0644)
      printf("  [PASS]\n");
    else
      printf("  [FAIL]\n");
  }

  printf("\nTest 2: umask 077\n");
  umask(077);

  fd = open("u2.txt", O_CREATE | O_WRONLY);
  if(fd < 0){
    printf("FAIL: cannot create u2.txt\n");
    exit(1);
  }
  close(fd);

  if(stat("u2.txt", &st) == 0){
    printf("  Mode: 0%d%d%d, expect 0600\n",
      (st.mode >> 6) & 7,
      (st.mode >> 3) & 7,
      st.mode & 7);

    if(st.mode == 0600)
      printf("  [PASS]\n");
    else
      printf("  [FAIL]\n");
  }

  printf("\nTest 3: umask 000\n");
  umask(000);

  fd = open("u3.txt", O_CREATE | O_WRONLY);
  if(fd < 0){
    printf("FAIL: cannot create u3.txt\n");
    exit(1);
  }
  close(fd);

  if(stat("u3.txt", &st) == 0){
    printf("  Mode: 0%d%d%d, expect 0666\n",
      (st.mode >> 6) & 7,
      (st.mode >> 3) & 7,
      st.mode & 7);

    if(st.mode == 0666)
      printf("  [PASS]\n");
    else
      printf("  [FAIL]\n");
  }

  umask(022);

  printf("\n=== UMASK TEST DONE ===\n");

  exit(0);
}
