#Mooc Linux内核分析 第四周 实验作业

蔡景辉 原创作品转载请注明出处 《Linux内核分析》MOOC课程http://mooc.study.163.com/course/USTC-1000029000   

##实验要求
- 选择一个系统调用（13号系统调用time除外），
系统调用列表参见http://codelab.shiyanlou.com/xref/linux-3.18.6/arch/x86/syscalls/syscall_32.tbl
- 使用库函数API和C代码中嵌入汇编代码两种方式使用同一个系统调用,
- 内容围绕系统调用的工作机制进行
- 仔细分析汇编代码调用系统调用的工作过程，特别是参数的传递的方式等
- 总结部分需要阐明自己对“系统调用的工作机制”的理解。

##实验过程
选择了39号（0x27）系统调用，创建一个文件夹，搜索了一些资料，mkdir函数原型为
```c
#include <sys/stat.h> 

int mkdir(const char *path, mode_t mode); 
```
参数path是路径/目录名，mode是目录权限。在asm中eax存放系统调用号，abx存放path，acx存放mode。      
了解了以上信息，开始编程。首先用调用库函数的方法实现：

![img] (https://github.com/ZetaGo/Learning_Record/blob/master/Course_163_LinuxKernel/week4_img/20150329085105.png)

编译运行，结果如下：

![img](https://github.com/ZetaGo/Learning_Record/blob/master/Course_163_LinuxKernel/week4_img/20150329085106.png)

                   
利用嵌入汇编代码方式实现：

![img](https://github.com/ZetaGo/Learning_Record/blob/master/Course_163_LinuxKernel/week4_img/20150329092154.png)

![img](https://github.com/ZetaGo/Learning_Record/blob/master/Course_163_LinuxKernel/week4_img/20150329092210.png)


##实验分析与总结
系统调用提供了一些访问硬件的接口，如在mkdir中，ax存放了系统调用号，abx存放path，acx存放mode。
对这三个寄存器传参，可以很容易地调用系统功能。
而库函数API封装了一些调用方法，让我们能简单地调用系统功能，同时减少了用户随意访问系统误操作的可能。
