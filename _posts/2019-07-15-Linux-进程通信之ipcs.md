---
layout:     post
title:      Linux 进程通信之ipcs
subtitle:   介绍进程间通信（ipcs）包括：消息队列、信号量、共享内存
date:       2019-7-15
author:     Jerry Chen
header-img: img/post-bg-coffee.jpeg
catalog: true
tags:
    - 系统编程
    - 进程间通信

---


进程间通信（IPC,Inter-Process Communication），包括消息队列（查看方式：`ipcs -q`）、信号量（查看方式：`ipcs -s`）、共享内存（查看方式：`ipcs -m`）。

### 消息队列

实验过程中发现WSL（Windows子系统）下的msgget函数总是报错：Function not implemented。

因此该实验在虚拟机Ubuntu12.04下进行。

#### 介绍

消息队列和管道相同的是信息取出会删除，不同的是消息队列的传输内容可以是**自定义结构体**，但结构体的首个成员需要是long类型，用作类别标志。

操作方法可以分类为 创建消息队列/获取队列id、发送消息到队列中、从队列中取出消息、控制消息队列（比如删除消息队列，注意消息从消息队列中取出会删除，但此时消息队列不会删除）。

#### 操作

##### 1. 创建消息队列

创建消息队列的命令：`int msgget(key_t key, int msgflg);`

> 如果已经创建，用另一进程调用传入相同的key就是获取消息队列id

其中输入的key可自定义或者使用ftok函数的结果，msgflg一般写`0666|IPC_CREAT`。返回值-1表示失败，正常返回值为创建的消息队列的msgid。



最简创建消息队列：
```
#include <stdio.h>  // printf,perror
#include <stdlib.h> // exit
#include <sys/msg.h>

int main(int argc,char *argv[]){
	if((msgget((key_t)123,0666|IPC_CREAT)) == -1){
		perror("msgget");
		exit(1);
	}
	
	return 0;
}
```

查看创建的消息队列：

可以看到key为0x0000007b正是10进制的123。但是注意，删掉消息再次使用msgget创建消息队列的msqid可能不同。

```
root@ubuntu:~# ipcs -q

------ Message Queues --------
key        msqid      owner      perms      used-bytes   messages
0x0000007b 196608     root       666        0            0

```
删除并查看消息队列：
```
root@ubuntu:~# ipcrm -q 196608
root@ubuntu:~# ipcs -q

------ Message Queues --------
key        msqid      owner      perms      used-bytes   messages

```

再看生成key的函数ftok（实质是参数node和proj_id的某种组合生成密钥），原型`key_t ftok(const char *pathname, int proj_id);`，一般使用`ftok(".",1);`

##### 2. 发送消息队列

发送消息队列的命令：`int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);`

一般来说这些参数按照如下填写：

**msqid**：消息队列标识符，也就是msgget函数的返回值。

**msgp**：&msg，消息结构体地址。

**msgsz**：sizeof(msg)-sizeof(long)，消息的正文大小，其中msg是消息结构体，long是消息结构体中消息类型标志的大小。

**msgflg**：0，习惯用0，表示调用阻塞直到条件满足为止。还可用IPC_NOWAIT表示不阻塞。



基于上面生成了id为196608的消息队列，这里往该队列发送简单消息：

```
#include <stdio.h>  // printf,perror
#include <stdlib.h> // exit
#include <sys/msg.h>
#include <string.h>

struct _msg {
	long mType;		// 消息类型
	char mText[100];// 消息正文成员，可以为其他类型
	// 消息正文可以有多个成员
};

int main(int argc,char *argv[]){
	struct _msg msg;
	// 给消息队列结构体赋值
	msg.mType = 10;
	strcpy(msg.mText,"hello jerry!");

	msgsnd(196608, &msg, sizeof(msg)-sizeof(long), 0);
	return 0;
}
```

发送后使用`ipcs -q`查看消息队列发现，队列中信息数量由0变为1，其中104字节刚好是我创建的消息结构体的大小（4+100）。

```
root@ubuntu:~# ipcs -q

------ Message Queues --------
key        msqid      owner      perms      used-bytes   messages
0x0000007b 196608     root       666        104          1

```

##### 3. 接收消息队列

接收消息队列的命令：`ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp,int msgflg);`

一般来说这些参数按照如下填写：

**msqid**：消息队列标识符，也就是msgget函数的返回值。

**msgp**：&msg，消息结构体地址。

**msgsz**：sizeof(msg)-sizeof(long)，消息的正文大小，其中msg是消息结构体，long是消息结构体中消息类型标志的大小。

**msgtyp**：消息类型比如10，填写msg消息结构体的第一个long类型成员。

**msgflg**：0，习惯用0，表示调用阻塞直到条件满足为止。还可用IPC_NOWAIT表示不阻塞，MSG_NOERROR表示长度过长截取返回不报错。



基于上面id为196608的消息队列有类型为10的消息，这里往该队列取出该消息：

```
#include <stdio.h>  // printf,perror
#include <stdlib.h> // exit
#include <sys/msg.h>

struct _msg {
	long mType;		// 消息类型
	char mText[100];// 消息正文成员，可以为其他类型
	// 消息正文可以有多个成员
};

int main(int argc,char *argv[]){
	struct _msg msg;

	// 取出消息队列的值赋给声明的消息结构体并打印
	msgrcv(196608, &msg, sizeof(msg)-sizeof(long), (long)10, 0);
	printf("msg.mText = %s\n", msg.mText); 
	return 0;
}
```

接收后可以看到正常打印消息，然后使用`ipcs -q`查看消息队列发现，队列中消息数量由1变为0，但是消息队列未被删除。

```
root@ubuntu:~# ./a.out
msg.mText = hello jerry!
root@ubuntu:~# ipcs -q

------ Message Queues --------
key        msqid      owner      perms      used-bytes   messages
0x0000007b 196608     root       666        0            0

```

##### 4. 控制消息队列

控制消息队列的命令：`int msgctl(int msqid, int cmd, struct msqid_ds *buf);`

该命令可以改变消息队列属性，删除消息队列等。

一般来说这些参数按照如下填写：

**msqid**：消息队列标识符，也就是msgget函数的返回值。

**cmd**：函数功能的控制。其取值可以为：`IPC_RMID`删除由 msqid 指示的消息队列；`IPC_STAT`把消息队列的属性备份到 buf 里。`IPC_SET`把消息队列原来的属性值清空，再由 buf 来替换。

**buf**：msqid_ds 数据类型的地址，用来存放或更改消息队列的属性。



基于上面存在id为196608的消息队列，这里删除该消息队列：

```
#include <stdio.h>  // printf,perror
#include <stdlib.h> // exit
#include <sys/msg.h>

int main(int argc,char *argv[]){
	// 删除消息队列
	msgctl(196608, IPC_RMID, NULL);
	return 0;
}
```

执行后发现消息队列被正常删除。

```
root@ubuntu:~# ipcs -q

------ Message Queues --------
key        msqid      owner      perms      used-bytes   messages

```

#### 例程

编写简单的msgsend.c和msgreceive.c文件实现消息队列。

文件msgsend.c如下：

```
#include <unistd.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <string.h>  
#include <sys/msg.h>  
#include <errno.h>  
  
#define MAX_TEXT 512  
struct msg_st  
{  
    long int msg_type;  
    char text[MAX_TEXT];  
};  
  
int main()
{  
    int running = 1;  
    struct msg_st data;  
    char buffer[BUFSIZ];  // BUFSIZ 8192
    int msgid = -1;  
  
    //建立消息队列  
    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);  
    if(msgid == -1)  
    {  
        fprintf(stderr, "msgget failed with error: %d\n", errno);  
        exit(EXIT_FAILURE);  
    }  
  
    //向消息队列中写消息，直到写入end  
    while(running)  
    {  
        //输入数据  
        printf("Enter some text: ");  
        fgets(buffer, BUFSIZ, stdin);  //stdin 表示标准输入流
        data.msg_type = 1;
        strcpy(data.text, buffer);  
        //向队列发送数据  
        if(msgsnd(msgid, (void*)&data, MAX_TEXT, 0) == -1)  
        {  
            fprintf(stderr, "msgsnd failed\n");  
            exit(EXIT_FAILURE);  
        }  
        //输入end结束输入  
        if(strncmp(buffer, "end", 3) == 0)  
            running = 0;  
        sleep(1);  
    }  
    exit(EXIT_SUCCESS);  
}
```

文件msgreceive.c如下：

```
#include <unistd.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <string.h>  
#include <errno.h>  
#include <sys/msg.h>  
  
struct msg_st
{  
    long int msg_type;  
    char text[BUFSIZ];  
};  
  
int main()  
{  
    int running = 1;  
    int msgid = -1;  
    struct msg_st data;  
    long int msgtype = 0;//0表示最新一个
  
	
  
    //建立消息队列  
    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);  
    if(msgid == -1)  
    {  
        fprintf(stderr, "msgget failed with error: %d\n", errno);  
        exit(EXIT_FAILURE);  
    }  
    //从队列中获取消息，直到遇到end消息为止  
    while(running)  
    {  
        if(msgrcv(msgid, (void*)&data, BUFSIZ, msgtype, 0) == -1)  
        {  
            fprintf(stderr, "msgrcv failed with errno: %d\n", errno);  
            exit(EXIT_FAILURE);  
        }  
        printf("You wrote: %s\n",data.text);  
        //遇到end结束  
        if(strncmp(data.text, "end", 3) == 0) //strcmp是全部比较，这里只比较3个字符
            running = 0;  
    }  
    //删除消息队列  
    if(msgctl(msgid, IPC_RMID, 0) == -1)  
    {  
        fprintf(stderr, "msgctl(IPC_RMID) failed\n");  
        exit(EXIT_FAILURE);  
    }  
    exit(EXIT_SUCCESS);  
} 
```

执行结果：

可以看到标准输入会转换成消息发送到消息队列，然后消息被取出打印出来。当输入end后会删除消息队列，最后结束进程。如果中间强制kill进程，创建的消息队列不会回收。

```
root@ubuntu:~# ./ipcs_q_read &
[1] 4754
root@ubuntu:~# ./ipcs_q_write
Enter some text: 123
You wrote: 123

Enter some text: I love you 3000 times
You wrote: I love you 3000 times

Enter some text: end
You wrote: end

[1]+  Done                    ./ipcs_q_read
root@ubuntu:~# ipcs -q

------ Message Queues --------
key        msqid      owner      perms      used-bytes   messages

```

### 信号量

信号量是一个阻塞和放行进程的工具，和一般的变量在fork后不同，它的值是共享的，也就是在一个进程中改变了信号量的值，其他进程读取到信号量的值也会改变。补充知识：对于一般变量n，父进程n=10，然后fork两个进程，在子进程1改变n=6，不会影响父进程和子进程2中的n=10，因为fork时候已经把进程中所有的资源全部复制了。

#### 介绍

**1、信号量--semaphore**

信号量是一个表示资源的实体，是一个与队列有关的整型变量，信号量只支持P操作（申请）和V操作（释放）。其具体的意义如下述代码所示(为什么是联合体，首先指针占用空间均是4字节，估计该联合体某个时刻只做一种用途)：

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20190712190128.png)

对于val取值的说明：

- val>0 时：信号量可用的资源个数；

- val=0 时：无空闲资源，无空闲进程，正在执行一个进程；

- val< 0时：value的绝对值代表使用该资源的阻塞进程的个数；

**2、P操作和V操作**

P(proberen)操作是申请资源的操作，其伪代码如下：

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20190712165915.png)



V(verhogen)操作是释放资源的操作，其伪代码如下：

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20190712170031.png)



流程图如下：

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20190712170202.png)

**3、打印机作业详解P V操作**

在并发程序中，有进程A B C 执行打印作业，但是我们拥有的打印机台数为1台，若各进程竞争使用打印机，会造成的作业的错误，因此，必须使用进程互斥的手段来解决进程竞争的状态。设定 semaphor S=1 （代表着共享资源 打印机）；利用PV操作实现进程的互斥。

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20190712170442.png)

进程由1个空闲变为0空闲，再变为1个、2个阻塞，再变为1个阻塞，接着变为0空闲、1个空闲。

以上的PV操作实现了进程的互斥。

也就是`semaphore初始值 = 1`就是**相关进程多选1**，或者说进程互斥，本例是3个中只有1个能执行。

如果`semaphore初始值 = 3`就是相关进程最多3个同时执行，下面的生产者消费者问题就会用到选多个的情况。

**4、生产者和消费者的问题**

问题描述：一组生产者进程和一组消费者进程共享一块初始为空，大小确定的缓冲区，只有当缓冲区没满时，生产者进程才可以把信息放入缓冲区，否则就要等待；只有缓存区不为空时，消费者进程才能从中取出消息，否则就要等待。缓冲区一次只能一个进程访问（临界资源）。

- 问题分析：假定生产者是面包店，消费者是购物的人，缓存区为放面包的盒子(资源)；

- 关系分析：
  - 生产者和消费者对于资源的访问互斥：生产者把面包放入盒子，消费者才可以去拿走面包，或者消费者拿走面包后，生产者才可再放入面包；生产者放面包与消费者那面包不能同时进行。
  - 生产者与消费者相互依赖：只有生产者产面包，消费者才可以取走面包；只有消费者取走面包后，生产者才可以继续生产。

信号量的设定：

semaphore empty=n；代表当前的空格子数，对于生产者来说，空格子是生产者的资源；

semaphore full =0；代表当前具有面包的盒子数量，对于消费者来说，放面包的盒子是消费者资源；

semaphore mutex=1 ；代表临界区互斥信号量；

生产者生产一个面包，就会给消费者一个通行证，告诉消费者，可以拿走面包；

消费者消费一个面包，就会给生产者一个通行证，告诉生产者，可以生产面包；

代码如下：

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20190712182409.png)

特别注意，如果进程先互斥再执行生产消费（也就是生产者进程先执行`P(mutex)`，然后执行`P(empty)`；消费者执行`P(mutex)`，然后执行`P(full)`），这样可不可以？

答案是不可以。设想生产者进程已经将缓冲区放满，消费者进程并没有取产品，即empty = 0，当下次仍然是生产者进程运行时，它先执行P(mutex)封锁信号量，再执行P(empty)时将被阻塞，希望消费者取出产品后将其唤醒。轮到消费者进程运行时，它先执行P(mutex)，然而由于生产者进程已经封锁mutex信号量，消费者进程也会被阻塞，这样一来生产者、消费者进程都将阻塞，都指望对方唤醒自己，陷入了无休止的等待。进入死锁状态。

#### 操作

##### 1. 生成信号量集

原型`int semget(key_t key, int nsems, int semflg);`

> 如果已经创建，用另一进程调用传入相同的key就是获取信号量id

返回semid，失败返回-1。nsems是信号量个数。

常用：`sem_id = semget((key_t)1234, 1, 0666 | IPC_CREAT);   `

结果如下：

0x000004d2即1234，32768即返回值，666即权限，1即传参中填写的信号量个数

```
------ Semaphore Arrays --------
key        semid      owner      perms      nsems
0x000004d2 32768      root       666        1
```

##### 2. 控制信号量

原型`int semctl(int semid, int semnum, int cmd,[union semun semopts]);`

**读取信号量值** 常用：`sem_val = semctl(sem_id,semnum,GETVAL,0);`

**设置信号量初值** 常用：

```
//union semun 该联合体需要从semctl的man中copy，否则gcc报错
union semun semopts;//联合体，定义初值等
//给信号量赋初值
semopts.val = 3;
semctl(sem_id,semnum,SETVAL,semopts);
```

**删除信号量集** 常用：`semctl(sem_id,semnum,IPC_RMID,0); //semnum 随意写，集合会全删除`

这里写一个例子创建拥有6个num的信号量集，其num是从0~5，给他们写入初值为num+3，然后打印出来。

```
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

union semun {
	int              val;
	struct semid_ds *buf;
	unsigned short  *array;
	struct seminfo  *__buf;
};	//该联合体需要从semctl的man中copy，否则gcc报错

int main(int argc,char *argv[]){
	int i;//用于循环
	int sem_id;//创建的信号量的id
	union semun un_semval;//联合体定义初值等
	
	//创建信号量
	if((sem_id = semget((key_t)1234, 6, 0666 | IPC_CREAT)) == -1){
		perror("semget");
		exit(1);
	}
	printf("sem_id is %d\n",sem_id);
	
	for(i = 0;i<6;i++){
		//给信号量赋初值
		un_semval.val = i+3;
		semctl(sem_id,i,SETVAL,un_semval);
		
		//打印读取到的初值
		printf("semnum%d's val is %d\n",i,semctl(sem_id,i,GETVAL,0));
	}
	
	//错误示范
	printf("semnum%d's val is %d\n",6,semctl(sem_id,i,GETVAL,0));
	
	//删除信号量集，其中的semnum可任意写
	semctl(sem_id,0,IPC_RMID,0);
	
	return 0;
}
```

执行后可以看到符合预期，信号量初值为num+3，执行`ipcs -s`可知信号量集已经删除。如果不给信号量赋初值，可以试验得到初值默认为0。

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20190713232223.png)

##### 3. 操作信号量

原型`int semop(int semid, struct sembuf *sops, unsigned nsops);`

其中关于**sops指针参数**，一般会声明一个指针数组，比如`struct sembuf sops[8];`，然后传入指针数组名sops。关于**nsops参数**就是使用sops指针数组的个数，比如3就按顺序使用sops[0]、sops[1]、sops[2]，如果使用sops[1]时阻塞了就不会继续调用sops[2]，也就是使用sops[n]是递增顺序的。

结构体struct sembuf中有三个成员：

- sem_num：需要操作的信号量集中的信号量num，比如信号量集中的2；
- sem_op：0为阻塞到该信号量为0，1为递增信号 量，-1为递减信号量，**一般用1或者-1**；
- sem_flg：IPC_NOWAIT  不阻塞, SEM_UNDO 还原，表示进程结束（包括异常结束）就撤销本次semop操作；**一般写0**或某些情况SEM_UNDO ；

**P操作** 常用：`semop(sem_id,sops,1);`，其中`sops[0].sem_op = -1;`

**V操作** 常用：`semop(sem_id,sops,1);`，其中`sops[0].sem_op = 1;`

这里写一个简单的例子：

先创建有6个信号量的信号量集，赋初值0~5，操作分别执行0号对比0，1号减1为0，2号加1为3，3号减1为2，打印，然后1号减1为-1触发阻塞。

```
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

union semun {
	int              val;
	struct semid_ds *buf;
	unsigned short  *array;
	struct seminfo  *__buf;
};	//该联合体需要从semctl的man中copy，否则gcc报错

int main(int argc,char *argv[]){
	int i;//用于循环
	int sem_id;
	union semun un_semval;//联合体定义信号量初值等
	struct sembuf sops[6];//信号量操作的结构体
	
	//创建信号量
	printf("sem_id is %d\n",sem_id=semget((key_t)1234, 6, 0666 | IPC_CREAT));
	
	for(i = 0;i<6;i++){
		//给信号量赋初值
		un_semval.val = i;
		semctl(sem_id,i,SETVAL,un_semval);
		
		//打印读取到的初值
		printf("semnum%d's val is %d\n",i,semctl(sem_id,i,GETVAL,0));
	}
	
	//操作信号量
	sops[0].sem_num = 0;        /* Operate on semaphore 0 */
	sops[0].sem_op = 0;         /* Wait for value to equal 0 */
	sops[0].sem_flg = 0;

	sops[1].sem_num = 1;        /* Operate on semaphore 1 */
	sops[1].sem_op = -1;        /* decrement value by one */
	sops[1].sem_flg = 0;
	
	sops[2].sem_num = 2;        /* Operate on semaphore 2 */
	sops[2].sem_op = 1;         /* Increment value by one */
	sops[2].sem_flg = 0;
	
	sops[3].sem_num = 3;        /* Operate on semaphore 3 */
	sops[3].sem_op = -1;        /* decrement value by one */
	sops[3].sem_flg = 0;

	if(semop(sem_id, sops, 4) == -1) {
		perror("semop");
		exit(1);
	}
	
	//再次读取
	for(i = 0;i<6;i++){
		//打印读取到的初值
		printf("semnum%d's val is %d\n",i,semctl(sem_id,i,GETVAL,0));
	}
	
	//触发阻塞
	sops[0].sem_num = 1;        /* Operate on semaphore 1 */
	sops[0].sem_op = -1;        /* decrement value by one */
	sops[0].sem_flg = 0;
	if(semop(sem_id, sops, 1) == -1) {
		perror("semop");
		exit(1);
	}
	
	//删除信号量集，其中的semnum可任意写
	semctl(sem_id,0,IPC_RMID,0);
	
	return 0;
}
```

可以进行简化理解，semop作用简单表达为：如果sem_op为0，目标信号量为0放行；如果sem_op为 + ，目标信号量加上该值，直接放行；如果sem_op为 - ，目标信号量加上该值，执行后为负就阻塞，否找放行。

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20190714014711.png)

**注意**：上面全篇说信号量为负值是一种理解方法，可以实验得到多个进程阻塞时semctl得到的信号量值仍然是0，不过使用负值理解完全没问题。有这样的现象：已经阻塞了两个进程，读取信号量值为0，对信号量每加1次放行一个进程，如果用负值方法，信号量值从-2 -> -1 -> 0正好可以解释通过。

#### 例程

哲学家就餐问题规则：

- 5个哲学家围绕一张圆桌而坐，桌子上放着5支筷子，每两个哲学家之间放一支；
- 哲学家的动作包括思考和进餐，进餐时需要同时拿起他左边和右边的两支筷子；
- 思考时则同时将两支筷子放回原处。

如何保证哲学家们的动作有序进行？如：不出现相邻者同时要求进餐，不出现有人永远拿不到筷子；

解决思路：五只筷子相当于资源（初值为1的信号量，共5个），保证每次会有两位哲学家先进餐，进餐完后在换其他哲学家，能够保证五位哲学家都能进餐。

```
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<string.h>
 
static int semid;
 
union semun{
	int value;
};

//获得资源
static void get_sour(int num)
{
	struct sembuf sb[2]={
		{num,-1,0},			//左边的筷子
		{(num+1)%5,-1,0}	//右边的筷子
	};
	semop(semid,sb,2);		//获取两根筷子
}
 
//释放资源
static void free_sour(int num)
{
	struct sembuf sb[2]={
		{num,1,0},			//左边的筷子
		{(num+1)%5,1,0}		//右边的筷子
	};
	semop(semid,sb,2);		//释放两根筷子
 
}
 
static void phil(int num)
{
	while(1){
		printf("%d is thinking...\n",num);	//开始思考
		sleep(rand()%5);					//0~4s后
		get_sour(num);						//获取两根筷子
		printf("%d start eating...\n",num);	//吃饭
		sleep(rand()%3);					//0~2s后
		printf("%d end eating...\n",num);	//结束吃饭
		free_sour(num);						//释放两根筷子
	}
}
 
int main(int argc,char *argv[])
{
	union semun s;
	int i=0;
	
	srand(getpid());	//设置随机数种子
	semid=semget(1234,5,IPC_CREAT|0600);//创建信号量集，5个代表5根筷子
	if(semid == -1){
		perror("semget");
		exit(1);
	}
	
	//五个信号量均设初值1，表示5根筷子
	s.value=1;
	for(i=0;i<5;i++){
		semctl(semid,i,SETVAL,s);
	}
	
	//创建五个进程——一个父进程+4个子进程
	int num=0;			//为每个进程编号,父进程初始化编号为0
	for(i=1;i<5;i++){	//1,2,3,4
		pid_t pid=fork();
		if(pid == 0){
			num=i;
			break;
		}
	}
	phil(num);
}
```

结果如下，开局不错2个吃饭3个思考，由于引入了随机延时，这些都不是一定的。

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20190714030403.png)

### 共享内存

#### 介绍

所谓共享内存就是使得多个进程可以访问同一块内存空间，是最快的可用IPC形式。。

操作方法可以分类为 创建共享内存/获取共享内存id、连接到共享内存、和共享内存断开、控制共享内存（比如删除共享内存）。

#### 操作

##### 1. 创建共享内存

创建共享的命令：`int shmget(key_t key, size_t size, int shmflg);`


> 如果已经创建，用另一进程调用传入相同的key就是获取共享内存id

其中输入的key可自定义或者使用ftok函数的结果，size为共享内存的大小（比如4096字节），shmflg一般写`0666|IPC_CREAT`。返回值-1表示失败，正常返回值为创建的共享内存的shmid。



最简创建共享内存：

```
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(int argc,char *argv[]){
	int shm_id; // 共享内存的创建后的返回id
	shm_id = shmget((key_t)999,4096,0666|IPC_CREAT);//大小4096字节
	return 0;
}
```

查看创建的共享内存：

可以看到key为0x000003e7正好是999，大小为4096bytes。

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20190715184059.png)

##### 2. 连接到共享内存

连接到共享内存的命令：`void *shmat(int shmid, const void *shmaddr, int shmflg);`

其返回值为**共享内存段的地址**。

一般来说传入参数按照如下填写：

**shmid**：共享内存id，也就是shmget函数的返回值。

**shmaddr**：指定共享内存地址，一般用NULL，让操作系统选择未使用的内存。

**msgflg**：0，习惯用0，SHM_RDONLY为只读模式，其他为读写模式。

综上常用为：

```
char *shmaddr;
...;//创建和获取shmid等
shmaddr = (char *)shmat(shmid,NULL,0);
```

基于上面生成了id为32768的共享内存，这里往该共享内存写入数据，但是不断开连接：

```
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

int main(int argc,char *argv[]){
	int shm_id; // 共享内存的创建后的返回id
	char *shmaddr; // 共享内存连接后返回的地址
	shm_id = shmget((key_t)999,4096,0666|IPC_CREAT);//大小4096字节
	
	if((shmaddr = shmat(shm_id,NULL,0)) < (char *)0){//连接共享内存，返回地址
		perror("shmat");
		exit(1);
	}
	printf("share mem attached at %p\n",shmaddr);
	system("ipcs -m");
	
	strcpy( shmaddr, "I love you 3000 times!"); //给共享内存写入信息
	
	printf("Sleep 5s...\n");
	sleep(5);
	system("ipcs -m");
	
	return 0;
}
```

结果如下可以看到间隔5s打印`ipcs -m`信息连接数都是1，但是进程退出后会自动断开连接。

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20190715193856.png)

##### 3. 读取共享内存

即使写入共享内存的进程已经退出，去读共享内存依然可以读到数据，读到不会删除，简单来说可以把共享内存**理解为全局变量**。

```
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(int argc,char *argv[]){
	int shm_id; // 共享内存的创建后的返回id
	char *shmaddr; // 共享内存连接后返回的地址
	shm_id = shmget((key_t)999,4096,0666|IPC_CREAT);//大小4096字节
	
	if((shmaddr = shmat(shm_id,NULL,0)) < (char *)0){//连接共享内存，返回地址
		perror("shmat");
		exit(1);
	}
	printf("share mem attached at %p\n",shmaddr);
	
	printf("rec: %s\n",shmaddr); //读取共享内存信息
	shmdt(shmaddr);
	
	return 0;
}
```

结果是会读到最近的一次写入："I love you 3000 times!"，此时写入的进程已经退出。

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20190715194819.png)

尝试写共享内存用最快速度，然后读共享内存，发现没什么问题！不过保险起见，还是建议使用信号量进行同步。

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20190716032750.png)



##### 4. 和共享内存断开连接

断开共享内存连接的命令：`int shmdt(const void *shmaddr);`

其参数shmaddr就是使用shmat的返回值。

##### 5. 控制共享内存

控制共享内存的命令：`int shmctl(int shmid, int cmd, struct shmid_ds *buf);`

一般来说传入参数按照如下填写：

**shmid**：共享内存id，也就是shmget函数的返回值。

**cmd**参数：

- IPC_RMID：删除shm_id所指向的共享内存段，只有当shmid_ds结构的shm_nattch域为零时，才会真正执行删除命令，常用`shmctl(shmid,IPC_RMID,NULL);`；
- IPC_STAT：取shm_id所指向内存共享段的shmid_ds结构，对参数buf指向的结构赋值；
- IPC_SET：使用buf指向的结构对sh_mid段的相关结构赋值；

**buf**：备份结构体，习惯用NULL。

#### 例程

创建两个进程一个写一个读，写"end"退出两个进程，删掉共享内存。

ipcs_m_read.c的代码如下：

```
#include <unistd.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <sys/shm.h>  

#define TEXT_SZ 2048  
  
struct shared_use_st  
{  
    //作为一个标志，非0：表示可读，0表示可写
	int written;
	//记录写入和读取的文本	
    char text[TEXT_SZ];  
}; 
  
int main(void)  
{  
    int running = 1;//程序是否继续运行的标志  
    void *shm = NULL;//分配的共享内存的原始首地址  
    struct shared_use_st *shared;//指向shm  
    int shmid;//共享内存标识符  
    //创建共享内存  
    shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666|IPC_CREAT);  
    if(shmid == -1)  
    {  
        fprintf(stderr, "shmget failed\n");  
        exit(EXIT_FAILURE);  
    }  
    //将共享内存连接到当前进程的地址空间  
    shm = shmat(shmid, 0, 0);  
    if(shm == (void*)-1)  
    {  
        fprintf(stderr, "shmat failed\n");  
        exit(EXIT_FAILURE);  
    }  
    printf("\nMemory attached at %p\n", shm);  
    //设置共享内存  
    shared = (struct shared_use_st*)shm;  
    shared->written = 0;  
    while(running)//读取共享内存中的数据  
    {  
        //没有进程向共享内存定数据有数据可读取  
        if(shared->written != 0)  
        {  
            printf("You wrote: %s", shared->text);  
            sleep(rand() % 3);  
            //读取完数据，设置written使共享内存段可写  
            shared->written = 0;  
            //输入了end，退出循环（程序）  
            if(strncmp(shared->text, "end", 3) == 0)  
                running = 0;  
        }  
        else//有其他进程在写数据，不能读取数据  
            sleep(1);  
    }  
    //把共享内存从当前进程中分离  
    if(shmdt(shm) == -1)  
    {  
        fprintf(stderr, "shmdt failed\n");  
        exit(EXIT_FAILURE);  
    }  
    //删除共享内存  
    if(shmctl(shmid, IPC_RMID, 0) == -1)  
    {  
        fprintf(stderr, "shmctl(IPC_RMID) failed\n");  
        exit(EXIT_FAILURE);  
    }  
    exit(EXIT_SUCCESS);  
}
```

ipcs_m_write.c的代码如下：

```
#include <unistd.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <string.h>  
#include <sys/shm.h>  

#define TEXT_SZ 2048  
  
struct shared_use_st  
{  
    //作为一个标志，非0：表示可读，0表示可写
	int written;
	//记录写入和读取的文本	
    char text[TEXT_SZ];  
}; 
  
int main(void)  
{  
    int running = 1;  
    void *shm = NULL;		//存放连接共享内存地址
    struct shared_use_st *shared = NULL;
    char buffer[BUFSIZ + 1];//用于保存输入的文本  
    int shmid;  
    //创建共享内存  
    shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666|IPC_CREAT);  
    if(shmid == -1)  
    {  
        fprintf(stderr, "shmget failed\n");  
        exit(EXIT_FAILURE);  
    }  
    //将共享内存连接到当前进程的地址空间  
    shm = shmat(shmid, (void*)0, 0);  
    if(shm == (void*)-1)  
    {  
        fprintf(stderr, "shmat failed\n");  
        exit(EXIT_FAILURE);  
    }  
    printf("Memory attached at %p\n", shm);  
    //设置共享内存  
    shared = (struct shared_use_st*)shm;  
    while(running)//向共享内存中写数据  
    {  
        //数据还没有被读取，则等待数据被读取,不能向共享内存中写入文本  
        while(shared->written == 1)  
        {  
            sleep(1);  
            printf("Waiting...\n");  
        }  
        //向共享内存中写入数据  
        printf("Enter some text: ");  
        fgets(buffer, BUFSIZ, stdin);  
        strncpy(shared->text, buffer, TEXT_SZ);  
        //写完数据，设置written使共享内存段可读  
        shared->written = 1;  
        //输入了end，退出循环（程序）  
        if(strncmp(buffer, "end", 3) == 0)  
            running = 0;  
    }  
    //把共享内存从当前进程中分离  
    if(shmdt(shm) == -1)  
    {  
        fprintf(stderr, "shmdt failed\n");  
        exit(EXIT_FAILURE);  
    }  
    sleep(2);  
    exit(EXIT_SUCCESS);  
}
```

客户端发送信息：

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20190720092111.png)

服务器接收到信息：

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20190720092313.png)

### 综合例程

以下综合例程使用了【共享内存+信号量+消息队列】的组合来实现服务器进程与客户进程间的通信。

- 共享内存用来传递数据；
- 信号量用来同步；
- 消息队列用来 在客户端修改了共享内存后 通知服务器读取。

server.c

```
#include<stdio.h>
#include<stdlib.h>
#include<sys/shm.h>  // shared memory
#include<sys/sem.h>  // semaphore
#include<sys/msg.h>  // message queue
#include<string.h>   // memcpy

// 消息队列结构
struct msg_form {
    long mtype;
    char mtext;
};

// 联合体，用于semctl初始化
union semun
{
    int              val; /*for SETVAL*/
    struct semid_ds *buf;
    unsigned short  *array;
};

// 初始化信号量
int init_sem(int sem_id, int value)
{
    union semun tmp;
    tmp.val = value;
    if(semctl(sem_id, 0, SETVAL, tmp) == -1)
    {
        perror("Init Semaphore Error");
        return -1;
    }
    return 0;
}

// P操作:
//  若信号量值为1，获取资源并将信号量值-1 
//  若信号量值为0，进程挂起等待
int sem_p(int sem_id)
{
    struct sembuf sbuf;
    sbuf.sem_num = 0; /*序号*/
    sbuf.sem_op = -1; /*P操作*/
    sbuf.sem_flg = SEM_UNDO;

    if(semop(sem_id, &sbuf, 1) == -1)
    {
        perror("P operation Error");
        return -1;
    }
    return 0;
}

// V操作：
//  释放资源并将信号量值+1
//  如果有进程正在挂起等待，则唤醒它们
int sem_v(int sem_id)
{
    struct sembuf sbuf;
    sbuf.sem_num = 0; /*序号*/
    sbuf.sem_op = 1;  /*V操作*/
    sbuf.sem_flg = SEM_UNDO;

    if(semop(sem_id, &sbuf, 1) == -1)
    {
        perror("V operation Error");
        return -1;
    }
    return 0;
}

// 删除信号量集
int del_sem(int sem_id)
{
    union semun tmp;
    if(semctl(sem_id, 0, IPC_RMID, tmp) == -1)
    {
        perror("Delete Semaphore Error");
        return -1;
    }
    return 0;
}

// 创建一个信号量集
int creat_sem(key_t key)
{
    int sem_id;
    if((sem_id = semget(key, 1, IPC_CREAT|0666)) == -1)
    {
        perror("semget error");
        exit(-1);
    }
    init_sem(sem_id, 1);  /*初值设为1资源未占用*/
    return sem_id;
}


int main()
{
    key_t key;
    int shmid, semid, msqid;
    char *shm;
    char data[] = "this is server";
    struct shmid_ds buf1;  /*用于删除共享内存*/
    struct msqid_ds buf2;  /*用于删除消息队列*/
    struct msg_form msg;  /*消息队列用于通知对方更新了共享内存*/

    // 获取key值
    if((key = ftok(".", 'z')) < 0)
    {
        perror("ftok error");
        exit(1);
    }

    // 创建共享内存
    if((shmid = shmget(key, 1024, IPC_CREAT|0666)) == -1)
    {
        perror("Create Shared Memory Error");
        exit(1);
    }

    // 连接共享内存
    shm = (char *)shmat(shmid, 0, 0);
    if(shm == (char *)-1)
    {
        perror("Attach Shared Memory Error");
        exit(1);
    }


    // 创建消息队列
    if ((msqid = msgget(key, IPC_CREAT|0777)) == -1)
    {
        perror("msgget error");
        exit(1);
    }

    // 创建信号量
    semid = creat_sem(key);
    
    // 读数据
    while(1)
    {
        msgrcv(msqid, &msg, 1, 888, 0); /*读取类型为888的消息*/
        if(msg.mtext == 'q')  /*quit - 跳出循环*/ 
            break;
        if(msg.mtext == 'r')  /*read - 读共享内存*/
        {
            sem_p(semid);
            printf("%s\n",shm);
            sem_v(semid);
        }
    }

    // 断开连接
    shmdt(shm);

    /*删除共享内存、消息队列、信号量*/
    shmctl(shmid, IPC_RMID, &buf1);
    msgctl(msqid, IPC_RMID, &buf2);
    del_sem(semid);
    return 0;
}
```

client.c

```
#include<stdio.h>
#include<stdlib.h>
#include<sys/shm.h>  // shared memory
#include<sys/sem.h>  // semaphore
#include<sys/msg.h>  // message queue
#include<string.h>   // memcpy

// 消息队列结构
struct msg_form {
    long mtype;
    char mtext;
};

// 联合体，用于semctl初始化
union semun
{
    int              val; /*for SETVAL*/
    struct semid_ds *buf;
    unsigned short  *array;
};

// P操作:
//  若信号量值为1，获取资源并将信号量值-1 
//  若信号量值为0，进程挂起等待
int sem_p(int sem_id)
{
    struct sembuf sbuf;
    sbuf.sem_num = 0; /*序号*/
    sbuf.sem_op = -1; /*P操作*/
    sbuf.sem_flg = SEM_UNDO;

    if(semop(sem_id, &sbuf, 1) == -1)
    {
        perror("P operation Error");
        return -1;
    }
    return 0;
}

// V操作：
//  释放资源并将信号量值+1
//  如果有进程正在挂起等待，则唤醒它们
int sem_v(int sem_id)
{
    struct sembuf sbuf;
    sbuf.sem_num = 0; /*序号*/
    sbuf.sem_op = 1;  /*V操作*/
    sbuf.sem_flg = SEM_UNDO;

    if(semop(sem_id, &sbuf, 1) == -1)
    {
        perror("V operation Error");
        return -1;
    }
    return 0;
}


int main()
{
    key_t key;
    int shmid, semid, msqid;
    char *shm;
    struct msg_form msg;
    int flag = 1; /*while循环条件*/

    // 获取key值
    if((key = ftok(".", 'z')) < 0)
    {
        perror("ftok error");
        exit(1);
    }

    // 获取共享内存
    if((shmid = shmget(key, 1024, 0)) == -1)
    {
        perror("shmget error");
        exit(1);
    }

    // 连接共享内存
    shm = (char *)shmat(shmid, 0, 0);
    if(shm == (char *)-1)
    {
        perror("Attach Shared Memory Error");
        exit(1);
    }

    // 创建消息队列
    if ((msqid = msgget(key, 0)) == -1)
    {
        perror("msgget error");
        exit(1);
    }

    // 获取信号量
    if((semid = semget(key, 0, 0)) == -1)
    {
        perror("semget error");
        exit(1);
    }
    
    // 写数据
    printf("***************************************\n");
    printf("*                 IPC                 *\n");
    printf("*    Input r to send data to server.  *\n");
    printf("*    Input q to quit.                 *\n");
    printf("***************************************\n");
    
    while(flag)
    {
        char c;
        printf("Please input command: ");
        scanf("%c", &c);
        switch(c)
        {
            case 'r':
                printf("Data to send: ");
                sem_p(semid);  /*访问资源*/
                scanf("%s", shm);
                sem_v(semid);  /*释放资源*/
                /*清空标准输入缓冲区*/
                while((c=getchar())!='\n' && c!=EOF);
                msg.mtype = 888;  
                msg.mtext = 'r';  /*发送消息通知服务器读数据*/
                msgsnd(msqid, &msg, sizeof(msg.mtext), 0);
                break;
            case 'q':
                msg.mtype = 888;
                msg.mtext = 'q';
                msgsnd(msqid, &msg, sizeof(msg.mtext), 0);
                flag = 0;
                break;
            default:
                printf("Wrong input!\n");
                /*清空标准输入缓冲区*/
                while((c=getchar())!='\n' && c!=EOF);
        }
    }

    // 断开连接
    shmdt(shm);

    return 0;
}
```

其中，当`scanf()`输入字符或字符串时，缓冲区中遗留下了`\n`，所以每次输入操作后都需要清空标准输入的缓冲区。可以使用`fflush(stdin)`，这里使用了替代方案：`while((c=getchar())!='\n' && c!=EOF);`也就是不断通过getchar取出缓冲，EOF是文件结尾的返回值。

客户端发送信息：

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20190720093503.png)

服务端读取：

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20190720093717.png)

### 扩展

C语言中scanf、gets、fgets的区别？

#### scanf

scanf会读取标准输入缓冲区中的输入行内容直到遇到回车或空白（回车、tab、空格）。以换行为例，scanf会把换行符之前的数据全部读入到scanf指定的变量中，但**回车（\n）依然保留在缓冲区**，所以一般需要加入其他代码来清空缓存区：

```
//这里getchar()从缓冲区中读一个字符，直到读取到回车结束
//由于getchar()没有设置变量保存读取的字符，所以读取的字符全部被抛弃
//这样就把一行内容全部清空了（回车前面的内容包括回车）
while(getchar() != '\n')
	continue;
```

实验demo：

```
#include <stdio.h>

int main(int argc,char *argv[]){
	char str[10];//千万不要char *str，然后scanf("%s",str);，这是错误的，因为str没有指向有效的内存，它可能指向任何非法地址
	char c;
	printf("input string:  ");
	scanf("%s",str);//变量名已经是地址
	printf("output string:%s\n",str);////填变量名

	printf("input char:  ");
	scanf("%c",&c);//填变量地址
	printf("output char:%d\n",c - '\n');////填变量名
	return 0;
}
```

结果果然第二个输入`scanf("%c",&c);`直接从缓冲区取走了'\n'，所以对于这种情况需要加入清空输入缓冲，特别是使用`scanf("%c",&c);`的情况下输入了不止一个字符。

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20190716013213.png)

但是如果两个输入均是`scanf("%s",str);`，会发现第一个字符是回车、tab、空格均会被丢弃，所以不会出现问题。

优化版本程序：

```
#include <stdio.h>

int main(int argc,char *argv[]){
	char str[10];//千万不要char *str，然后scanf("%s",str);，这是错误的，因为str没有指向有效的内存，它可能指向任何非法地址
	char c;
	
	printf("input string:  ");
	scanf("%s",str);//变量名已经是地址
	while(getchar() != '\n'); //scanf后都加这句
	printf("output string:%s\n",str);////填变量名

	printf("input char:  ");
	scanf("%c",&c);//填变量地址
	while(getchar() != '\n'); //scanf后都加这句
	printf("output char:%c\n",c);////填变量名

	return 0;
}
```

结果如下：

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20190716020022.png)

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20190716020535.png)

#### fgets

fgets使用来处理文件的，不过可以设置文件指针是stdin（标准输入，一般指键盘），它在读取标准输入时在遇到回车时会直接回车之前**包括回车**都读出到指定的变量中；

```
#include <stdio.h>//fgets()用法

int main(int argc,char *argv[]){
	char buffer[BUFSIZ + 1];//用于保存输入的文本
	
	printf("input string:  ");
	fgets(buffer, BUFSIZ, stdin); //推荐用字符数组长度-1作为size（留空'\0'）
	printf("output string:%s\n",buffer);//填变量名
	
	printf("end\n");
	return 0;
}
```

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20190716025209.png)

#### gets-不建议使用

简单来说就是从**标准输入**取**一行**字符串（注：回车算下一行）。

因为该函数没有指定最大读取长度，有风险在新的标准中已经被替换，**不建议使用该函数**。编译时会报警告函数过时，建议使用fgets替代。

gets只有在遇到回车时才会把缓冲区中的内容**全部**读出了，然后**去掉回车（\n）**把回车前的内容全部写到变量中；

配合函数：`puts(s);`会字符串后自动加回车输出打印。

#### 其他

`int getchar()`  `int getc(stdin)`  `putchar(int c)`  `putc(int c,stdout)`

```
#include <stdio.h>

int main(int argc,char *argv[]){
	char c;
	c = getchar();
	putchar(c);
	c = getc(stdin);
	putc(c,stdout);
	
	printf("\n>>>end>>>\n");
	return 0;
}
```

可以看到获取单个字符也需要看情况选择下次输入前清空输入缓冲区。

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20190716031603.png)

另外了解到`fflush(stdin);`就是刷新标准输入缓冲区，可以试验一下，是否有效果。

