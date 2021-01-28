#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), 0, DIRSIZ-strlen(p));
  return buf;
}

void find(char path[], char filename[]) {

    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;
    if((fd = open(path, 0)) < 0){
        
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    if (st.type == T_DIR) {
        if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
            printf("find: path too long\n");
            exit(0);
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
                printf("find: cannot stat %s\n", buf);
                continue;
            }
            if (st.type == T_FILE) {

                if (strcmp(fmtname(buf), filename) == 0) {
                    printf("%s\n", buf);
                   // printf("test %s/%s\n", path, filename);
                }
            } else if (st.type == T_DIR) {
                
                char* folderName = fmtname(buf);
                if (strcmp(".", folderName) != 0 && (strcmp("..", folderName) != 0)) {
                    char newFolder[256];
                    strcpy(newFolder, buf);
                    find(newFolder, filename);
                }
            }
        }
    }
}
int main(int argc, char* argv[]) {

    find(argv[1], argv[2]);
    exit(0);
}