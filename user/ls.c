#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  buf[sizeof(buf)-1] = '\0';
  return buf;
}

// In permission string dang "rwxr-xr-x"
void
print_perm(uint mode)
{
  char p[10];
  p[0] = (mode & 0400) ? 'r' : '-';
  p[1] = (mode & 0200) ? 'w' : '-';
  p[2] = (mode & 0100) ? 'x' : '-';
  p[3] = (mode & 0040) ? 'r' : '-';
  p[4] = (mode & 0020) ? 'w' : '-';
  p[5] = (mode & 0010) ? 'x' : '-';
  p[6] = (mode & 0004) ? 'r' : '-';
  p[7] = (mode & 0002) ? 'w' : '-';
  p[8] = (mode & 0001) ? 'x' : '-';
  p[9] = '\0';
  printf("%s", p);
}

void
ls(char *path)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, O_RDONLY)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }
  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_DEVICE:
  case T_FILE:
    printf("%s ", fmtname(path));
    print_perm(st.mode);
    printf(" uid:%d %d\n", st.uid, (int)st.size);
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("ls: cannot stat %s\n", buf);
        continue;
      }
      printf("%s ", fmtname(buf));
      print_perm(st.mode);
      printf(" uid:%d %d\n", st.uid, (int)st.size);
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  int i;
  if(argc < 2){
    ls(".");
    exit(0);
  }
  for(i=1; i<argc; i++)
    ls(argv[i]);
  exit(0);
}
