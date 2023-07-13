
// 引入声明类型的头文件
#include "kernel/types.h"
// 引入声明系统调用和 user/ulib.c 中其他函数的头文件
#include "user/user.h"
int main(int argc, char *argv[])
{
    // 如果命令行参数不等于2个，则打印错误信息
    if (argc != 2)
    {
        // 使用 user/printf.c 中的 fprintf(int fd, const char *fmt, ...) 函数进行格式化输出
        fprintf(2, "Usage: sleep time\n");
        // 退出程序
        exit(1);
    }
    int time = atoi(argv[1]);
    sleep(time);
    // 正常退出程序
    exit(0);
}

