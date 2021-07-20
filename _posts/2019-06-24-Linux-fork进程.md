---
layout:     post
title:      Linux fork进程
subtitle:   介绍进程fork的简单模板
date:       2019-06-24
author:     Jerry Chen
header-img: img/post-bg-coffee.jpeg
catalog: true
tags:
    - 系统编程
    - 进程
---

> 首先知道`execl`函数可以开辟一段内存空间，传入程序的二进制文件和参数，跳转执行，但是pid还是相同的。而执行fork后，会fork进程的所有资源，包括变量。同时有两个进程在函数中运行，有两次返回。

### 例子

```
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc,char *argv[]){
	pid_t pid;
	
	pid = fork();
	switch(pid){
	case -1:
		perror("fork");
		return 1;
	case 0:
		printf("子进程%d正在执行！\n",getpid());
		printf("当前父进程是%d！\n",getppid());
		sleep(2);	//休眠2s
		exit(1);	//杀掉进程
	default:
		printf("父进程%d正在执行!\n",getpid());
		printf("当前子进程是%d！\n",pid);
		sleep(2);	//休眠2s
		exit(1);	//杀掉进程
	}
	return 0;
}
```

最后的执行结果是：

```
root@ubuntu:~# ./a.out
父进程12823正在执行!
当前子进程是12824！
子进程12824正在执行！
当前父进程是12823！
root@ubuntu:~#
```

可以看到fork进程是连续的，fork前进程是12823，fork后的父进程是12823，子进程是12824。

fork会把返回值0传给子进程，把子进程pid传给父进程。sleep(2)是为了让进程有序进行，不在控制台显示错乱。

可以使用`top`、`jobs`、`ps`查看当前进程。`time`查看运行时间，比如上述程序运行了2.002s。

```
root@ubuntu:~# time ./a.out
父进程12830正在执行!
当前子进程是12831！
子进程12831正在执行！
当前父进程是12830！

real    0m2.002s
user    0m0.000s
sys     0m0.000s
root@ubuntu:~#
```

