
#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"
#include "kernel/fs.h"

// find 函数
void find(char *dir, char *file)
{   //buf 是用来记录文件前缀的，这样才会打印出之前的目录
    char buf[512], *p;
    int fd;
    // 声明与文件相关的结构体
    struct dirent de;
    struct stat st;

    // open() 函数打开路径，返回一个文件描述符，如果错误返回 -1
    fd = open(dir, 0);
    if (fd < 0)
    {
        fprintf(2, "find: cannot open %s\n", dir);
        return;
    }

    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "find: cannot stat %s\n", dir);
        close(fd);
        return;
    }

    // 如果不是目录类型
    if (st.type != T_DIR)
    {
        fprintf(2, "find: %s is not a directory\n", dir);
        close(fd);
        return;
    }

    // 如果路径过长放不入缓冲区
    if(strlen(dir) + 1 + DIRSIZ + 1 > sizeof buf)
    {
        fprintf(2, "find: directory too long\n");
        close(fd);
        return;
    }
    // 绝对路径复制到 buf
    strcpy(buf, dir);
    //p 为一个指针，指向 buf(path) 的末尾
    p = buf + strlen(buf);
    //在末尾添加 / 比如 path 为 a/b/c 经过这步后变为 a/b/c/< p
    *p++ = '/';
    // 读取 fd ，如果 read 返回字节数与 de 长度相等则循环
    while (read(fd, &de, sizeof(de)) == sizeof(de))
    {
        if(de.inum == 0 || !strcmp(de.name, ".") || !strcmp(de.name, ".."))
            continue;
        memmove(p, de.name, DIRSIZ);
        // 设置文件名结束符
        p[DIRSIZ] = 0;
        if(stat(buf, &st) < 0)
        {
            // 报错
            fprintf(2, "find: cannot stat %s\n", buf);
            continue;
        }
       
        if (st.type == T_DIR)
        { // 如果是目录类型，递归查找
            find(buf, file);
        }
        // 如果是文件类型 并且 名称与要查找的文件名相同
        else if (st.type == T_FILE && !strcmp(de.name, file))
        {
            // 直接打印出该文件的位置
            printf("%s\n", buf);
        } 
    }
}

int main(int argc, char *argv[])
{
    // 检查参数个数是否为3，否则退出
    if (argc != 3)
    {
        fprintf(2, "usage: find dirName fileName\n");
        exit(1);
    }
    // 寻找指定目录下的文件
    find(argv[1], argv[2]);
    exit(0);
}

