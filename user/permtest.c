// permtest.c - Test tu dong he thong permission xv6
// Chay: $ permtest
// Ket qua mong doi: 24/24 PASS

#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

static int pass_count = 0;
static int fail_count = 0;

void
check(const char *desc, int cond)
{
  if(cond){
    printf("  [PASS] %s\n", desc);
    pass_count++;
  } else {
    printf("  [FAIL] %s\n", desc);
    fail_count++;
  }
}

int
make_file(const char *path, const char *data)
{
  int fd = open(path, O_CREATE | O_RDWR);
  if(fd < 0) return -1;
  if(data)
    write(fd, data, strlen(data));
  close(fd);
  return 0;
}

int
main(void)
{
  int fd, n;
  struct stat st;

  printf("\n========================================\n");
  printf("   xv6 Permission System - Full Test\n");
  printf("========================================\n\n");

  // Don dep file cu
  unlink("t_file");
  unlink("t_newfile");

  // --- [1] Default permission ---
  printf("--- [1] Default permission on new file ---\n");
  fd = open("t_file", O_CREATE | O_RDWR);
  check("Create file succeeds", fd >= 0);
  if(fd >= 0){
    fstat(fd, &st);
    check("Default mode = 0644 (rw-r--r--)", st.mode == 0644);
    close(fd);
  }

  // --- [2] chmod thay doi mode ---
  printf("\n--- [2] chmod changes mode ---\n");
  check("chmod 755 returns 0", chmod("t_file", 0755) == 0);
  if(stat("t_file", &st) == 0)
    check("stat shows 0755 after chmod 755", st.mode == 0755);

  check("chmod 600 returns 0", chmod("t_file", 0600) == 0);
  if(stat("t_file", &st) == 0)
    check("stat shows 0600 after chmod 600", st.mode == 0600);

  check("chmod 000 returns 0", chmod("t_file", 0000) == 0);
  if(stat("t_file", &st) == 0)
    check("stat shows 0000 after chmod 000", st.mode == 0000);

  // --- [3] Read permission ---
  printf("\n--- [3] Read permission enforcement ---\n");
  chmod("t_file", 0644);
  fd = open("t_file", O_RDONLY);
  check("Open RDONLY with 0644 (owner has r): ALLOWED", fd >= 0);
  if(fd >= 0) close(fd);

  chmod("t_file", 0333);
  fd = open("t_file", O_RDONLY);
  check("Open RDONLY with 0333 (no r bit): DENIED", fd < 0);
  if(fd >= 0) close(fd);

  chmod("t_file", 0400);
  fd = open("t_file", O_RDONLY);
  check("Open RDONLY with 0400 (owner r only): ALLOWED", fd >= 0);
  if(fd >= 0) close(fd);

  // --- [4] Write permission ---
  printf("\n--- [4] Write permission enforcement ---\n");
  chmod("t_file", 0644);
  fd = open("t_file", O_WRONLY);
  check("Open WRONLY with 0644 (owner has w): ALLOWED", fd >= 0);
  if(fd >= 0){
    n = write(fd, "hello", 5);
    check("Write 5 bytes succeeds", n == 5);
    close(fd);
  }

  chmod("t_file", 0444);
  fd = open("t_file", O_WRONLY);
  check("Open WRONLY with 0444 (no w bit): DENIED", fd < 0);
  if(fd >= 0) close(fd);

  chmod("t_file", 0200);
  fd = open("t_file", O_WRONLY);
  check("Open WRONLY with 0200 (owner w): ALLOWED", fd >= 0);
  if(fd >= 0) close(fd);

  // --- [5] No access ---
  printf("\n--- [5] No access (chmod 000) ---\n");
  chmod("t_file", 0000);
  fd = open("t_file", O_RDONLY);
  check("Open RDONLY with 0000: DENIED", fd < 0);
  if(fd >= 0) close(fd);
  fd = open("t_file", O_WRONLY);
  check("Open WRONLY with 0000: DENIED", fd < 0);
  if(fd >= 0) close(fd);
  fd = open("t_file", O_RDWR);
  check("Open RDWR with 0000: DENIED", fd < 0);
  if(fd >= 0) close(fd);

  // --- [6] RDWR ---
  printf("\n--- [6] O_RDWR permission ---\n");
  chmod("t_file", 0644);
  fd = open("t_file", O_RDWR);
  check("Open RDWR with 0644: ALLOWED", fd >= 0);
  if(fd >= 0) close(fd);

  // --- [7] Directory default ---
  printf("\n--- [7] Directory default permission ---\n");
  mkdir("t_testdir");
  if(stat("t_testdir", &st) == 0){
    check("Directory default mode = 0755", st.mode == 0755);
    check("Directory type = T_DIR", st.type == T_DIR);
  }

  // --- [8] stat uid ---
  printf("\n--- [8] stat uid field ---\n");
  chmod("t_file", 0644);
  if(stat("t_file", &st) == 0){
    check("stat.uid = 1 (regular user)", st.uid == 1);
  }

  // --- [9] New file ---
  printf("\n--- [9] O_CREATE default permission ---\n");
  fd = open("t_newfile", O_CREATE | O_RDWR);
  check("Create new file succeeds", fd >= 0);
  if(fd >= 0){
    fstat(fd, &st);
    check("New file mode = 0644", st.mode == 0644);
    close(fd);
  }

  // Cleanup
  unlink("t_file");
  unlink("t_newfile");

  // Summary
  printf("\n========================================\n");
  printf("  Results: %d passed, %d failed\n", pass_count, fail_count);
  printf("========================================\n");
  if(fail_count == 0)
    printf("  ALL TESTS PASSED!\n");
  else
    printf("  SOME TESTS FAILED\n");
  printf("\n");

  exit(fail_count > 0 ? 1 : 0);
}
