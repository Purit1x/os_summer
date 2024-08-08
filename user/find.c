#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char*, char*);

int main(int argc, char* argv[])
{
    if (argc != 3) {
        printf("error: The number of arguments must be 2. ");
        exit(1);
    }
    char* targetPath = argv[1];
    char* targetFile = argv[2];
    find(targetPath, targetFile);
    exit(0);
}

void find(char* path, char* targetFile)
{
    char buf[512], * p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch (st.type) {
    case T_FILE:
        printf("Usage: find specified file(s) in a directory tree");
        break;

    case T_DIR:
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
            printf("find: path too long\n");
            break;
        }
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        while (read(fd, &de, sizeof(de)) == sizeof(de)) {
            // 不能进入.和..
            if (de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
                continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            if (stat(buf, &st) < 0) {
                printf("find: cannot stat %s\n", buf);
                continue;
            }
            if (st.type == T_DIR) {
                find(buf, targetFile);
            }
            else if (st.type == T_FILE) {
                if (strcmp(de.name, targetFile) == 0)
                    printf("%s\n", buf);
            }
        }
        break;
    }
    close(fd);
}
