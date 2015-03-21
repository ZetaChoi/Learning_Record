#Mooc Linux内核分析 第三周 实验作业

蔡景辉 原创作品转载请注明出处 《Linux内核分析》MOOC课程http://mooc.study.163.com/course/USTC-1000029000   

##实验要求
- 完成一个简单的时间片轮转多道程序内核代码，代码见视频中或从mykernel找。
- 详细分析该精简内核的源代码并给出实验截图。

##实验内容
打开shell
```
cd LinuxKernel/
qemu -kernel linux-3.18.6/arch/x86/boot/bzImage -initrd rootfs.img
```
使用gdb跟踪调试内核
```
qemu -kernel linux-3.18.6/arch/x86/boot/bzImage -initrd rootfs.img -s -S # 关于-s和-S选项的说明：
# -S freeze CPU at startup (use ’c’ to start execution)
# -s shorthand for -gdb tcp::1234 若不想使用1234端口，则可以使用-gdb tcp:xxxx来取代-s选项
```
另开一个shell窗口
```
gdb
（gdb）file linux-3.18.6/vmlinux # 在gdb界面中targe remote之前加载符号表
（gdb）target remote:1234 # 建立gdb和gdbserver之间的连接,按c 让qemu上的Linux继续运行
（gdb）break start_kernel # 断点的设置可以在target remote之前，也可以在之后
```                        
##实验要求
- 使用gdb跟踪调试内核从start_kernel到init进程启动
- 需要使用实验截图
- 需要仔细分析start_kernel函数的执行过程
- 总结部分需要阐明自己对“Linux系统启动过程”的理解，尤其是idle进程、1号进程是怎么来的。                        

##实验过程
依据内容要求运行内核

![img](https://github.com/ZetaGo/Learning_Record/blob/master/Course_163_LinuxKernel/week3_img/20150321212450.png)

使用break指令设置断点，追踪内核运行过程。                     
分析得知start_kernel运行过程：                              
start_kernel    -->    lockdep_init();    -->    set_task_stack_end_magic(&init_task);    -->    
在这里看内核源码。http://codelab.shiyanlou.com/xref/linux-3.18.6/init/main.c#500
```c
void set_task_stack_end_magic(struct task_struct *tsk)
{
	unsigned long *stackend;
	stackend = end_of_stack(tsk);
	*stackend = STACK_END_MAGIC;	/* for overflow detection */
}
```
可以看到这里 set_task_stack_end_magic把&init_task（0号进程）的尾地址保存成一个 STACK_END_MAGIC，作用是检测stackoverflow                  
再看看0号进程
```c
static struct signal_struct init_signals = INIT_SIGNALS(init_signals);
15static struct sighand_struct init_sighand = INIT_SIGHAND(init_sighand);

/* Initial task structure */
struct task_struct init_task = INIT_TASK(init_task);
EXPORT_SYMBOL(init_task);

/*
 * Initial thread structure. Alignment of this is handled by a special
 * linker map entry.
 */
union thread_union init_thread_union __init_task_data =
	{ INIT_THREAD_INFO(init_task) };
```
