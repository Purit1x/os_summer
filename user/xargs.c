#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

#define MAX_SIZE 100

int main(int argc, char* argv[])
{
  char* cmd = argv[1];
  char params[MAXARG][MAX_SIZE] = { 0 };
  char* argLine[MAXARG];
  char buf;

  while (1) {
    int count = argc - 1;
    memset(params, 0, MAXARG * MAX_SIZE);  // 获取一条指令的参数前先清空
    for (int i = 1; i < argc; i++)
      strcpy(params[i - 1], argv[i]);
    int currentPos = 0;
    int flag = 0;
    int readReturn;

    while ((readReturn = read(0, &buf, 1)) > 0 && buf != '\n') {
      if (buf == ' ' && flag == 1) {  // 新的一个参数
        count++;
        currentPos = 0;
        flag = 0;  // 接下来为新参数, 不重复进入该代码块
      }
      else if (buf != ' ') {
        params[count][currentPos++] = buf;
        flag = 1;
      }
    }
    if (readReturn <= 0)  // 读取结束
      break;
    for (int i = 0; i < MAXARG - 1;i++)
      argLine[i] = params[i];
    argLine[MAXARG - 1] = 0;
    if (fork() == 0) {
      exec(cmd, argLine);
      exit(0);
    }
    else
      wait((int*)0);
  }
  exit(0);
}

