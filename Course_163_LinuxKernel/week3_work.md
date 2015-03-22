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
再看看0号进程：
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
用定义了一个tesk_struct类型的init_tesk
```c
struct task_struct {
	u64 curr_chain_key;
	int lockdep_depth;
	unsigned int lockdep_recursion;
	struct held_lock held_locks[MAX_LOCK_DEPTH];
	gfp_t lockdep_reclaim_gfp;
	int pid;
	char comm[17];
};
```
跟第二周的作业的PCB结构类似，可以看出是用来保存pid、栈地址等等信息的。              
有了零号进程、经过一定的进程调度，就可以执行到我们需要的1号进程了，找到调度模块rest_init()        http://codelab.shiyanlou.com/xref/linux-3.18.6/init/main.c#680                                 
```c
static noinline void __init_refok rest_init(void)
{
	int pid;

	rcu_scheduler_starting();
	/*
	 * We need to spawn init first so that it obtains pid 1, however
	 * the init task will end up wanting to create kthreads, which, if
	 * we schedule it before we create kthreadd, will OOPS.
	 */
	kernel_thread(kernel_init, NULL, CLONE_FS);
	numa_default_policy();
	pid = kernel_thread(kthreadd, NULL, CLONE_FS | CLONE_FILES);
	rcu_read_lock();
	kthreadd_task = find_task_by_pid_ns(pid, &init_pid_ns);
	rcu_read_unlock();
	complete(&kthreadd_done);

	/*
	 * The boot idle thread must execute schedule()
	 * at least once to get things moving:
	 */
	init_idle_bootup_task(current);
	schedule_preempt_disabled();
	/* Call into cpu_idle with preempt disabled */
	cpu_startup_entry(CPUHP_ONLINE);
}
```
依据注释的提示，可以了解到进程创建跟kernel_thread有关，而且函数形参就是kernel_init。直接看到kernel_init。    http://codelab.shiyanlou.com/xref/linux-3.18.6/init/main.c#930               
```c
static int __ref kernel_init(void *unused)
{
	int ret;

	kernel_init_freeable();
	/* need to finish all async __init code before freeing the memory */
	async_synchronize_full();
	free_initmem();
	mark_rodata_ro();
	system_state = SYSTEM_RUNNING;
	numa_default_policy();

	flush_delayed_fput();

	if (ramdisk_execute_command) {
		ret = run_init_process(ramdisk_execute_command);
		if (!ret)
			return 0;
		pr_err("Failed to execute %s (error %d)\n",
		       ramdisk_execute_command, ret);
	}

	/*
	 * We try each of these until one succeeds.
	 *
	 * The Bourne shell can be used instead of init if we are
	 * trying to recover a really broken machine.
	 */
	if (execute_command) {
		ret = run_init_process(execute_command);
		if (!ret)
			return 0;
		pr_err("Failed to execute %s (error %d).  Attempting defaults...\n",
			execute_command, ret);
	}
	if (!try_to_run_init_process("/sbin/init") ||
	    !try_to_run_init_process("/etc/init") ||
	    !try_to_run_init_process("/bin/init") ||
	    !try_to_run_init_process("/bin/sh"))
		return 0;

	panic("No working init found.  Try passing init= option to kernel. "
	      "See Linux Documentation/init.txt for guidance.");
}
```
注意这里
```c
	if (!try_to_run_init_process("/sbin/init") ||
	    !try_to_run_init_process("/etc/init") ||
	    !try_to_run_init_process("/bin/init") ||
	    !try_to_run_init_process("/bin/sh"))
		return 0;
```
可以知道，ini文件就是放在这四个文件夹下的。                  

##总结
这周的作业做得真是苦不堪言啊，内核代码看的一头雾水无从下手，最终在看了论坛上几个大大的帖子之后稍微有了点思路，kernel虽然很庞大，但是本质上很多机制跟跟我们所知的确是大同小异，在进程调用这块，内核首先调用了0号进程，然后frok出1号，进入用户的进程。kernel中能学的东西还有很多很多。
