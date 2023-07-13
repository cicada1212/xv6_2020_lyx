

#include "kernel/types.h"
#include "user/user.h"

// 映射函数
void mapping(int n, int p2c[])
{
 //关闭当前进程的某个文件描述符，把管道的读端或者写端的描述符通过 dup 函数复制给刚刚关闭的文件描述符，再把管道描述符关闭
  close(n);
  dup(p2c[n]);
  close(p2c[0]);
  close(p2c[1]);
}

// 求素数
void primes()
{
  // 获取管道中的数
  int before, after;
  // 定义管道描述符数组
  int p2c[2];
  // 从管道读取数据
  if (read(0, &before, sizeof(int)))
  {
    // 由于第一个数是素数，直接打印
    printf("prime %d\n", before);
    // 创建管道
    pipe(p2c);
    int pid=fork();//返回pid
    if (pid == 0)
    {
      mapping(1, p2c);
      // 接着我们需要不断读取管道中的数据
      while (read(0, &after, sizeof(int)))
      {
        // 如果该数不是倍数
        if (after % before != 0)
        {
          // 写入管道
          write(1, &after, sizeof(int));
        }
      }
    }
    else
    {
      // 等待子进程把数据全部写入管道
      wait(0);
      // 映射到描述符 0 上
      mapping(0, p2c);
      // 递归调用
      primes();
    }  
  }  
}

int main(int argc, char *argv[])
{
  // 定义一个数组
  int p2c[2];
  // 创建对应的管道
  pipe(p2c);
  int pid=fork();//返回pid
  if (pid == 0)
  {
    // 表明此时在子进程
    mapping(1, p2c);
    for (int i = 2; i < 36; i++)
    {
      // 将其写入管道
      write(1, &i, sizeof(i));
    }
  }
  else
  {
    // 等待子进程
    wait(0);
    // 父进程将管道的读端口映射到描述符 0 上
    mapping(0, p2c);
    // 求素数
    primes();
  }
  // 父进程结束，正常退出
  exit(0);
}

