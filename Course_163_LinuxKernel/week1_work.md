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
将实验代码保存到main.c，在终端输入gcc –S –o main.s main.c -m32
![image](https://github.com/ZetaGo/Learning_Record/Course/master/img/gdb_1.png)

编译成汇编语言


以"."开头的是一些用于链接的辅助信息，将其删除简化，得到易于观察的代码

