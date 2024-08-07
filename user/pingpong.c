#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[])
{
    int pipeParent2Child[2];
    int pipeChild2Parent[2];
    pipe(pipeParent2Child);
    pipe(pipeChild2Parent);

    if(fork() != 0){  // parent process
        close(pipeParent2Child[0]);
        close(pipeChild2Parent[1]);
        write(pipeParent2Child[1], "1", 1);  // ping
        char buf;
        read(pipeChild2Parent[0], &buf, 1);
        printf("%d: received pong\n", getpid());
        close(pipeParent2Child[1]);
        close(pipeChild2Parent[0]);
    }
    else{  // child process
        close(pipeChild2Parent[0]);
        close(pipeParent2Child[1]);
        char buf;
        read(pipeParent2Child[0], &buf, 1);
        printf("%d: received ping\n", getpid());
        write(pipeChild2Parent[1], &buf, 1);  // pong
    }
    exit(0);
}