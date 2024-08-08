#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void createChild(int* pipeLeft) {
    int pipeRight[2];
    int firstNum;
    close(pipeLeft[1]);

    int isReadEnd = read(pipeLeft[0], &firstNum, sizeof(int));
    if (0 == isReadEnd)  // 无数可读, 到达终点
    {
        exit(0);
    }
    else {
        printf("prime %d \n", firstNum);
        pipe(pipeRight);
        if (0 == fork()) {  // child process
            createChild(pipeRight);
        }
        else {  // parent process
            while (1) {
                int num;
                close(pipeRight[0]);
                if (read(pipeLeft[0], &num, sizeof(int)) == 0)
                    break;
                else
                    if (num % firstNum != 0)
                        write(pipeRight[1], &num, sizeof(int));
            }
            close(pipeRight[1]);
            close(pipeLeft[0]);
            wait((int*)0);
        }
    }
    exit(0);
}

int main(int argc, char* argv[])
{
    int p[2];
    pipe(p);

    if (fork() == 0) {
        createChild(p);
    }
    else {
        close(p[0]);
        for (int i = 2;i <= 35;i++)
            write(p[1], &i, sizeof(int));
        close(p[1]);
        wait((int*)0);
    }
    exit(0);
}
