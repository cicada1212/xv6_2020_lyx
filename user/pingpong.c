//首先引入头文件
#include "kernel/types.h"
#include "user/user.h"
//下面是main函数的编写
int main(int argc, char *argv[])
{
    //定义两个数组，一个是父进程到子进程，一个是从子进程到父进程
    int ptoc_fd[2], ctop_fd[2];
    //创建两个管道，分别用于父进程到子进程和子进程到父进程的通信
    pipe(ptoc_fd);
    pipe(ctop_fd);
    //创建缓冲区字符数组，存放传递的信息
    char buf[8];
    int pid;
    pid=fork();//返回的进程PID
    //表明是新创建的子进程
    if (pid== 0) {
        // child process
        //表示从管道端读取字符
        read(ptoc_fd[0], buf, 4);
        printf("%d: received %s\n", getpid(), buf);
        //再向另一个管道写入字符
        write(ctop_fd[1], "pong", strlen("pong"));
    }
    //表明是父进程
    else {
        // parent process
        //表示向管道段写入字符
        write(ptoc_fd[1], "ping", strlen("ping"));
        wait(0);
        //再从另一个管道读取字符
        read(ctop_fd[0], buf, 4);
        printf("%d: received %s\n", getpid(), buf);
    }
    exit(0);
}

