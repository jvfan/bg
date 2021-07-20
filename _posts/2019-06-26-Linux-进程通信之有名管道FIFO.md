---
layout:     post
title:      Linux 进程通信之有名管道FIFO
subtitle:   介绍进程之间通过有名管道FIFO通讯
date:       2019-06-26
author:     Jerry Chen
header-img: img/post-bg-coffee.jpeg
catalog: true
tags:
    - 系统编程
    - 有名管道

---



FIFO，也称为命名管道，它是一种文件类型。

### 特点
FIFO可以在无关的进程之间交换数据，即不要求交换数据的双方是父子进程，与无名管道不同。
FIFO有路径名与之相关联，它会生成一种特殊设备文件存在于文件系统中。

### 原型
`int mkfifo(const char *pathname, mode_t mode);`

其中的 mode 参数与open函数中的 mode 相同。一旦创建了一个 FIFO，就可以用一般的文件I/O函数操作它。

当 open 一个FIFO时，是否设置非阻塞标志（O_NONBLOCK）的区别：

若没有指定O_NONBLOCK（默认），只读 open 要阻塞到某个其他进程为写而打开此 FIFO。类似的，只写 open 要阻塞到某个其他进程为读而打开它。

若指定了O_NONBLOCK，则只读 open 立即返回。而只写 open 将出错返回 -1 如果没有进程已经为读而打开该 FIFO，其errno置ENXIO。

### 例子
FIFO的通信方式类似于在进程中使用文件来传输数据，只不过FIFO类型文件同时具有管道的特性。在数据读出时，FIFO管道中同时清除数据，并且“先进先出”。下面的例子演示了使用 FIFO 进行 IPC 的过程：

```
//write_fifo.c

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

int main(int argc,char *argv[]){
	int fd;
	int n,i;
	char buf[1024];
	time_t tp;
	
	printf("I am %d process.\n",getpid());  // 说明进程id
	
	if((fd = open("fifo1",O_WRONLY))<0)  // 只写方式打开FIFO
	{
		perror("Open FIFO failed!\n");
		exit(1);
	}
	
	for(i = 0;i < 10; ++i){
		time(&tp);
		n = sprintf(buf,"Process %d's time is %s",getpid(),ctime(&tp));
		printf("Send message:%s",buf);
		if(write(fd,buf,n+1)<0){
			perror("write FIFO");
			close(fd);
			exit(1);
		}
		sleep(1);
	}
	
	close(fd);
	return 0;
}
```

```
//read_fifo.c

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

int main(int argc,char *argv[]){
	int fd;
	int len;
	char buf[1024];
	
	if((mkfifo("fifo1",0666)<0)&&(errno!=EEXIST))	//创建失败且错误存在
		perror("mkfifo");
	
	if((fd = open("fifo1",O_RDONLY))<0)  // 只读方式打开FIFO
	{
		perror("Open FIFO");
		exit(1);
	}
	
	while((len = read(fd,buf,1024)) > 0)
		printf("Read message:%s",buf);
	
	close(fd);
	
	return 0;
}
```

在两个终端里用 gcc 编译上面两个文件，先后台运行`./fifo_read &`，然后运行`./fifo_write`可以看到输出结果如下：

```
I am 209 process.
Send message:Process 209's time is Wed Jul 10 02:35:10 2019
Read message:Process 209's time is Wed Jul 10 02:35:10 2019
Send message:Process 209's time is Wed Jul 10 02:35:11 2019
Read message:Process 209's time is Wed Jul 10 02:35:11 2019
Send message:Process 209's time is Wed Jul 10 02:35:12 2019
Read message:Process 209's time is Wed Jul 10 02:35:12 2019
Send message:Process 209's time is Wed Jul 10 02:35:13 2019
Read message:Process 209's time is Wed Jul 10 02:35:13 2019
Send message:Process 209's time is Wed Jul 10 02:35:14 2019
Read message:Process 209's time is Wed Jul 10 02:35:14 2019
Send message:Process 209's time is Wed Jul 10 02:35:15 2019
Read message:Process 209's time is Wed Jul 10 02:35:15 2019
Send message:Process 209's time is Wed Jul 10 02:35:16 2019
Read message:Process 209's time is Wed Jul 10 02:35:16 2019
Send message:Process 209's time is Wed Jul 10 02:35:17 2019
Read message:Process 209's time is Wed Jul 10 02:35:17 2019
Send message:Process 209's time is Wed Jul 10 02:35:18 2019
Read message:Process 209's time is Wed Jul 10 02:35:18 2019
Send message:Process 209's time is Wed Jul 10 02:35:19 2019
Read message:Process 209's time is Wed Jul 10 02:35:19 2019
```

上述例子可以扩展成 客户进程—服务器进程 通信的实例，write_fifo的作用类似于客户端，可以打开多个客户端向一个服务器发送请求信息，read_fifo类似于服务器，它适时监控着FIFO的读端，当有数据时，读出并进行处理，但是有一个关键的问题是，每一个客户端必须预先知道服务器提供的FIFO接口，下图显示了这种安排：

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20190710023821.png)



### 探究

我发现fifo的执行结果非常神奇，忍不住进行进一步的探究。

在这里我做了几个小实验。

1. 在fifo_read.c的open函数中加O_NONBLOCK修饰，会发现fifo_read进程会很快执行结束，这就是阻塞的开关。
2. 在fifo_read.c中open属性未加O_NONBLOCK修饰的前提下，保持fifo_read在后台运行，在fifo_write中加入死循环，也让其在后台运行，会发现两个进程均不会结束，接下来kill掉fifo_write进程会发现两个进程均会结束。



结论是：

- 进程结束前及时不关闭文件句柄，进程结束后也会被回收。
- fifo_read进程中open（只读）函数在fifo_write进程中open（只写）函数执行后取消阻塞。
- fifo_read进程中read函数在fifo_write进程中**write函数执行后**或者**文件句柄关闭后**取消阻塞。

