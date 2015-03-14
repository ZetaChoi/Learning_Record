#Mooc Linux内核分析 第一周 实验作业

蔡景辉 原创作品转载请注明出处 《Linux内核分析》MOOC课程http://mooc.study.163.com/course/USTC-1000029000   

##实验要求
- 完成一个简单的时间片轮转多道程序内核代码，代码见视频中或从mykernel找。
- 详细分析该精简内核的源代码并给出实验截图。

##实验过程
使用实验楼的虚拟机打开shell，输入
```
cd LinuxKernel/linux-3.9.4
qemu -kernel arch/x86/boot/bzImage
```

![img](https://github.com/ZetaGo/Learning_Record/blob/master/Course_163_LinuxKernel/week2_img/20150314084718.png)

修改mymain.c / myinterrupt.c，创建mypcb.h 三个文件。源码地址：https://github.com/mengning/mykernel。执行
···
make
qemu -kernel arch/x86/boot/bzImage
···

![img](https://github.com/ZetaGo/Learning_Record/blob/master/Course_163_LinuxKernel/week2_img/20150314093605.png)

1、2、3、4四个进程相互切换

##实验分析
对代码进行分析
####mypcb.h:
```c
#define MAX_TASK_NUM        4
#define KERNEL_STACK_SIZE   1024*8

/* CPU-specific state of this task */
struct Thread {
    unsigned long		ip;
    unsigned long		sp;
};

typedef struct PCB{
    int pid;
    volatile long state;	/* -1 unrunnable, 0 runnable, >0 stopped */
    char stack[KERNEL_STACK_SIZE];
    /* CPU-specific state of this task */
    struct Thread thread;
    unsigned long	task_entry;
    struct PCB *next;
}tPCB;

void my_schedule(void);
```
mypcb.h中定义了线程结构和pcb结构。               
Thread定义了ip和sp，分别指代当前位置和栈顶指针。                 
state表示当前进程是否运行过，是则为0，否则为-1。            
stack[]用数组分配出栈块。              
task_entry标志程序执行的任务。                
*next指向下一个进程。      

####mymain.h:
```c
    int pid = 0;
    int i;
    /* Initialize process 0*/
    task[pid].pid = pid;
    task[pid].state = 0;/* -1 unrunnable, 0 runnable, >0 stopped */
    task[pid].task_entry = task[pid].thread.ip = (unsigned long)my_process;
        task[pid].thread.sp = (unsigned long)&task[pid].stack[KERNEL_STACK_SIZE-1];
    task[pid].next = &task[pid];
    /*fork more process */
    for(i=1;i<MAX_TASK_NUM;i++)
    {
        memcpy(&task[i],&task[0],sizeof(tPCB));
        task[i].pid = i;
        task[i].state = -1;
        task[i].thread.sp = (unsigned long)&task[i].stack[KERNEL_STACK_SIZE-1];
        task[i].next = task[i-1].next;
        task[i-1].next = &task[i];
    }
```
对task[0]进行初始化，执行的任务为my_process。             
用相同的方法对1、2、3号进程初始化，并将四个进程连成链。
```c
pid = 0;
    my_current_task = &task[pid];
	asm volatile(
    	"movl %1,%%esp\n\t" 	/* set task[pid].thread.sp to esp */
    	"pushl %1\n\t" 	        /* push ebp */
    	"pushl %0\n\t" 	        /* push task[pid].thread.ip */
    	"ret\n\t" 	            /* pop task[pid].thread.ip to eip */
    	"popl %%ebp\n\t"
    	: 
    	: "c" (task[pid].thread.ip),"d" (task[pid].thread.sp)	/* input c or d mean %ecx/%edx*/
	);
```
定0号进程为当前任务，并将其装入cpu。          
原理与week1相同，利用栈对各个寄存器赋值，进程顺序为0 -> 1 -> 2 -> 3 -> 0。（注意这里每个进程都会保存自己的ip和sp副本，
作用是在下次调用该进程时能从上一次执行结束时的状态开始继续执行，下文中会详细说明）
```c
void my_process(void)
{
    int i = 0;
    while(1)
    {
        i++;
        if(i%10000000 == 0)
        {
            printk(KERN_NOTICE "this is process %d -\n",my_current_task->pid);
            if(my_need_sched == 1)
            {
                my_need_sched = 0;
        	    my_schedule();
        	}
        	printk(KERN_NOTICE "this is process %d +\n",my_current_task->pid);
        }     
    }
}
```
my_process是进程执行的任务，每10000000次循环检测一次my_need_sched的状态，为1则调用my_schedule()以切换进程。            
####myinterrupt.c:
```c
void my_timer_handler(void)
{
#if 1
    if(time_count%1000 == 0 && my_need_sched != 1)
    {
        printk(KERN_NOTICE ">>>my_timer_handler here<<<\n");
        my_need_sched = 1;
    } 
    time_count ++ ;  
#endif
    return;  	
}
```
计时器，每1000个周期检测一次my_need_sched的状态，为0则置1，表示当前进程时间片已经用完，资源将分配给下一个进程。           
#####my_schedule(void):
```c
    next = my_current_task->next;
    prev = my_current_task;
```
将当前进程next指针指向的内容（下一个进程）赋给next，并讲当前进程赋给prev。表示接下来将运行next进程，
而prev进程的内容将会被保存。               
```c
 if(next->state == 0)/* -1 unrunnable, 0 runnable, >0 stopped */
    {
    	/* switch to next process */
    	asm volatile(	
        	"pushl %%ebp\n\t" 	    /* save ebp */
        	"movl %%esp,%0\n\t" 	/* save esp */
        	"movl %2,%%esp\n\t"     /* restore  esp */
        	"movl $1f,%1\n\t"       /* save eip */	
        	"pushl %3\n\t" 
        	"ret\n\t" 	            /* restore  eip */
        	"1:\t"                  /* next process start here */
        	"popl %%ebp\n\t"
        	: "=m" (prev->thread.sp),"=m" (prev->thread.ip)
        	: "m" (next->thread.sp),"m" (next->thread.ip)
    	); 
    	my_current_task = next; 
    	printk(KERN_NOTICE ">>>switch %d to %d<<<\n",prev->pid,next->pid);   	
    }
    else
    {
        next->state = 0;
        my_current_task = next;
        printk(KERN_NOTICE ">>>switch %d to %d<<<\n",prev->pid,next->pid);
    	/* switch to new process */
    	asm volatile(	
        	"pushl %%ebp\n\t" 	    /* save ebp */
        	"movl %%esp,%0\n\t" 	/* save esp */
        	"movl %2,%%esp\n\t"     /* restore  esp */
        	"movl %2,%%ebp\n\t"     /* restore  ebp */
        	"movl $1f,%1\n\t"       /* save eip */	
        	"pushl %3\n\t" 
        	"ret\n\t" 	            /* restore  eip */
        	: "=m" (prev->thread.sp),"=m" (prev->thread.ip)
        	: "m" (next->thread.sp),"m" (next->thread.ip)
    	);          
    }   
```
检测state状态，若为0（执行过）则直接执行汇编语句，否则（未执行过）将其置0并执行汇编语句。            
在汇编语句中，将当前栈状态保存到prev进程中，并将next所保存的状态（上一次该进程结束时所保存的栈环境）装载，
**这里分析一下，进程执行的任务是my_process，而状态由task[pid].thread.ip和task[pid].thread.sp保存，调用my_schedule()
会将这个状态保存，在下一次轮到该进程执行后又还原，也就是说程序会在断点处继续执行！（若要证明这点，只需将my_process的i值打印，
观察第二次轮到该进程时的值是否连续）**            
至此程序全部分析完成。            
##总结
计算机执行多任务时会有一个调度的过程，时间片轮转法即定下每个进程运行的时间，时间片用完则将资源分配给下一个进程，
由此循环直至所有进程运行结束。其他的进程调度原理大同小异，调度的关键在于进程状态的装载和保存。
