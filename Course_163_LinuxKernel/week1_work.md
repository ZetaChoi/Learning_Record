#Mooc Linux内核分析 第一周 实验作业

蔡景辉 原创作品转载请注明出处 《Linux内核分析》MOOC课程http://mooc.study.163.com/course/USTC-1000029000   

##实验要求
使用
```
gcc –S –o main.s main.c -m32
```
命令编译成汇编代码
```c
int g(int x)
{
  return x + 8;
}
 
int f(int x)
{
  return g(x);
}
 
int main(void)
{
  return f(8) + 1;
}
```


- 需要使用实验截图 
- 需要仔细分析汇编代码的工作过程中堆栈的变化 
- 总结部分需要阐明自己对“计算机是如何工作的”理解   

##实验过程

###文件处理
将实验代码保存到main.c，在终端输入gcc -g -S -m32 -o main.s main.c
![image](https://github.com/ZetaGo/Learning_Record/blob/master/Course_163_LinuxKernel/week1_img/20150305170155.png)

编译成汇编语言

![image](https://github.com/ZetaGo/Learning_Record/blob/master/Course_163_LinuxKernel/week1_img/20150305170314.png)

以"."开头的是一些用于链接的辅助信息，将其删除简化，得到易于观察的代码

![image](https://github.com/ZetaGo/Learning_Record/blob/master/Course_163_LinuxKernel/week1_img/20150305170430.png)


###代码分析
接下来由main函数开始依据程序推进顺序对代码进行分析  
####main:  
```
pushl %ebp    
movl %esp, %ebp 
```
把ebp(栈底指针)的值push入栈,再将esp(栈顶指针)的值赋给esp。这两句的作用是保存当前的栈环境，并为当前函数开辟新的栈空间。
一般而言在每个函数块被调用时均会执行这两条语句，接下来的函数f、g亦同，不再赘述。    

```
subl $4, %esp
movl $8, (%esp)
```
esp减4，即开辟一个宽度为4的区域，然后将立即数“8”放入该区域中。我在第一次接触这段代码时不能理解这么做的意义，因为这两句与pushl $8, %esp没有本质的不同。于是在课程论坛中提出，得到了id为“五十梦幻”的朋友的帮助。

下面是他的解释：
  subl这条指令是为当前函数留出一段可用栈空间，有时可能是“subl $0x20, %esp”。这些空间为当前函数作用域内的变量所用，有有时为了对齐或其他考量，留出的空间可能大于所需的空间。
所以，为了编译器的生成代码的通用性，不能简单使用pushl替代。    

```
call f
```
执行该语句后，将当前eip的值（此时的eip指向call的下一条语句）保存到栈中，并将函数f的起始地址赋给eip，即跳转至函数f。  

上述步骤执行完成后，栈环境如下图，其中每个空间占用32位：

![image](https://github.com/ZetaGo/Learning_Record/blob/master/Course_163_LinuxKernel/week1_img/20150305183828.png)
  
####f:  
```
pushl %ebp    
movl %esp, %ebp 
subl $4, %esp
movl 8(%esp), %eax
movl %eax, (%esp)
call g
```
1、2条同上。esp减4开辟区域，将esp+8起始区域的值赋给eax（就是把形参10的值传入eax），之后后push eax入栈。
最后调用函数g。此时eax == 10。  

执行完成后栈环境如下图：

![image](https://github.com/ZetaGo/Learning_Record/blob/master/Course_163_LinuxKernel/week1_img/20150305190624.png)
  
####g:  
```
pushl %ebp    
movl %esp, %ebp 
movl 8(%esp), %eax
addl $3, %eax
```
1、2、3同上，开辟新栈空间后取出了形参值8赋给eax。接下来eax += 3，即eax == 11。     

```
popl %ebp
ret
```
popl将栈顶数据传入ebp，由上图可知就是把旧的esp值（指向上一个栈空间，即为函数f申请的空间）覆盖当前esp。
ret作用是从当前栈顶取值赋给eip，效果是程序跳转回函数f继续执行。  

完成后栈环境：

![image](https://github.com/ZetaGo/Learning_Record/blob/master/Course_163_LinuxKernel/week1_img/20150305193033.png)
  
####f:     
```
leave
ret
```
leave等价于<br/>
movl %ebp, %esp <br/>
pop %ebp<br/>
它与<br/>
pushl %ebp<br/>
movl %esp, %ebp <br/>
相对应，执行了回退的操作。ret作用如上所述，返回到main，执行完成后栈如图：

![image](https://github.com/ZetaGo/Learning_Record/blob/master/Course_163_LinuxKernel/week1_img/20150305194440.png)

####main:
```
addl $1, %eax
leave
ret
```
eax +=1，从上文中可知返回值保存在eax中，也就是对f(8)的返回值+1。leave、ret同上，结果如图，释放全部栈空间:

![image](https://github.com/ZetaGo/Learning_Record/blob/master/Course_163_LinuxKernel/week1_img/20150305195152.png)
<br/><br/>
至此程序执行完成

##总结
现代计算机采用冯诺依曼体系结构，由上面的小程序也可清楚发现，计算机工作时利用各个寄存器将数据从主存中取出，再交给运算器进行逻辑\数字运算，并重复这个循环。
