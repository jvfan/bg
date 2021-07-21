---
layout: interview
title: "INTERVIEW"
description: "I will put the interview questions here."
header-img: "img/home-bg.jpg"
---

### linux 和 os

1. fd

   fd 是一个整数，open 时产生，起到索引作用；

   PCB（Process Control Block）进程控制块中保存着一份文件描述符表，每个表项都指向一个已打开文件的指针。进程可以通过 PCB 中文件描述符表找到某个 fd 对应的文件指针 filp；

   fopen 返回的是一个 FILE 结构（包含 fd）的指针，故 FILE 结构函数可以看作是对 fd 操作的封装，优点是带有 I/O 缓存；

2. netstat

   显示网络状态信息：网络连接、路由表、接口状态、masquerade 连接，多播成员；

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20210713132352.png)

   命令输出结果可分为两部分：

   * Active Internet connections：有源 TCP 连接
     * Proto 使用的协议：tcp、udp；
     * Recv-Q 和 Send-Q 指接收队列和发送队列，一般应该是 0，否则表示数据包在队列中堆积；
     * State 显示套接字状态；
   * Active UNIX domain sockets：有源 Unix 域套接口，仅能用于本机通信，效率比网络套接字高一倍；
     * Proto 使用的协议：unix；
     * RefCnt 表示连接到本套接口上的进程号；
     * Types 显示套接口的类型；
     * Path 连接到套接口的进程使用的路径；

3. tcpdump（*）

   抓包工具，截取本地网络接口的数据；

4. ipcs

   显示进程间通信设施状态的工具，可以显示消息队列、共享内存、信号量的信息；

5. ipcrm

   手动解除系统上共享内存的分配；

6. 查看 cpu 信息

   ```shell
   cat /proc/cpuinfo
   cat /proc/meminfo
   ```

7. 查看硬盘信息

   ```shell
   # 简略信息
   df -lh
   
   # 详细信息
   cat /proc/scsi/scsi
   ```

8. 查看以太网网卡信息

   ```shell
   dmesg | grep eth
   ```

9. 显示系统内核版本号、名称、机器类型

   ```shell
   uname -a
   ```

10. awk（*）

    awk 是一种样式扫描和处理语言。使用它可以通过创建简短的程序实现读取输入文件、为数据排序、处理数据、对输入执行计算以及生成报表等功能；

    ```shell
    # 使用方法（会对文件的每一行都执行 AWK 脚本）
    awk '{pattern + action}' {filenames}
    ```

11. sed（*）

    Stream Editor 文本流编辑，sed 是一个“非交互式的”面向字符流的编辑器；

12. grep（*）

13. iostat（*）

    > `apt-get install sysstat` 进行安装；

    I/O statistics（输入/输出统计）能对系统的磁盘操作活动进行监视，同时也会汇报 CPU 使用情况；

14. top（*）

    `top -p <进程号>` 监视进程信息；

    `top -H -p <进程号>` 监视进程信息，并显示线程信息；

15. 共享内存的使用和实现原理

16. 共享内存段被映射进进程空间之后，存在于进程空间的什么位置？

17. 共享内存段最大限制是多少？

    ```shell
    ➜  ~ sysctl kernel.shmmax
    kernel.shmmax = 18446744073692774399
    ```

18. c++ 进程内存空间分布（*）

19. elf 是什么？

20. gdb

21. 如何定位内存泄漏？

    `top` 指令观察系统的内存总额变化 --> `top -p <pid>` 观察某个进程内存变化 --> `top -H -p <pid>` 观察该进程中线程的内存变化；

22. 动态链接和静态链接的区别？

23. 32 位系统一个进程最多多少推内存？

24. 线程和进程的区别和使用场景（从 cpu 调度、上下文切换、数据共享、多核 cpu 利用率、资源占用等方面回答）？哪些共享是一个线程私有的？

    寄存器是线程私有的；

25. 写一个 c 程序辨别系统是 64 位还是 32 位？

    ```c
    int k = ~0;
    if((unsigned int)k > 4294967296)
        cout << "64 bits system" < <endl;
    else
        cout << "32 bits system" << endl;
    ```

26. 写一个 c 程序辨别系统是大端还是小端字节序？（*）

    使用联合体（其中一个成员使用 char 类型），通过它输出的是 int 的高字节还是低字节进行辨别；

27. 列出常见的信号，信号怎么处理？

28. 系统如何将一个信号通知到进程？

29. i++ 是否是原子操作？

    不是原子操作，i++ 分为三个阶段：内存到寄存器、寄存器自增、写回内存；

30. linux 系统的同步机制有哪些？

31. 什么是死锁？如何避免死锁？

32. linux 系统的异步机制有哪些？

33. exit() 与 _exit() 的区别？

34. 如何实现守护进程？

35. linux 的内存管理机制是什么？

36. linux 的任务调度机制是什么？

37. 标准库函数和系统调用的区别？

38. linux 系统的五种 I/O 模式

### C 基础

1. 宏定义和展开

2. 位操作

3. 指针操作和计算

   * 指针的四要素
   * 指针变量的长度
   * const、volatile 修饰指针的含义
   * 堆和栈上的指针
   * 什么是指针的释放
   * 数据结构中的指针
   * 函数指针
   * 指针作为函数的参数
   * 指向指针的指针
   * 指针与地址的区别
   * 指针与数组名的关系
   * 怎样防止指针的越界使用问题
   * 指针的类型转换
   * 什么是指针退化
   * 指针 p++

4. 内存分配的三种方式

   * 从静态存储区域分配
   * 在栈上创建
   * 从堆上分配

5. sizeof 一个结构体求大小

6. 各类常用库函数的实现（memcpy 考虑内存重叠）

7. 哪些库函数属于高危函数

   如 strcpy；

8. 写一个宏 `OFFSETOF(s, m)` 实现求 m 在 s 中的偏移量（s 是结构类型，m 是 s 的成员）

   ```c
   #define OFFSETOF(s, m) ({s s1;(void*)(&s1)-(void*)(&s1->m);})
   ```

### C++ 基础

1. inline 的作用（*）

   在 c/c++ 中，为了解决一些频繁调用的小函数大量消耗栈空间（栈内存）的问题，引入了 inline 修饰符，表示为内联函数。

   ```c++
   #include <stdio.h>
    
   inline const char *num_check(int v)
   {
       return (v % 2 > 0) ? "奇" : "偶";
   }
    
   int main(void)
   {
       int i;
       for (i = 0; i < 100; i++)
           printf("%02d   %s\n", i, num_check(i));
       return 0;
   }
   ```

   上面的例子就是标准的内联函数的用法，使用 inline 修饰带来的好处我们表面看不出来，但在内部的工作使得每个 for 循环的内部任何调用 num_check 的地方都换成了 (i%2>0)?"奇":"偶"，这样就避免了频繁调用函数对栈内存重复开辟所带来的消耗。

2. 重载和重写的概念；（*）

3. 一个 string 类的完整实现（operator= 是关键）

4. 虚函数的作用和实现原理

5. sizeof 一个类求大小（注意成员变量、函数、虚函数、继承等等对大小的影响）

6. 指针和引用的区别（*）

7. 多重类构造和析构的顺序

   先调用基类的构造函数，再调用派生类的构造函数；

   先构造的后析构，后构造的先析构；

8. stl 各容器的实现原理

   STL 共有六大组件：容器、算法、迭代器、仿函数、适配器；

   序列式容器有：

   * vector
   * list
   * deque
   * map
   * stack
   * queue
   * heap
   * priority_queue
   * slist

   关联式容器有：

   * set
   * map
   * multiset
   * multimap
   * hash_xxx

9. extern c 是干嘛的（结合编译器的函数名修饰机制回答）（*）

10. volatile 是干嘛的（结合 cpu 的寄存器缓存机制回答）

11. static、const 的用法；（*）

   static 主要功能是隐藏，其次因为 static 变量存放在静态存储区，所以它具备持久性和默认值 0；
   
   const：
   * 修饰变量，说明该变量不可以被改变；
   * 修饰指针，分为指向常量的指针和指针常量；
   * 常量引用，经常用于形参类型，即避免了拷贝，又避免了函数对值的修改；
   * 修饰成员函数，说明该成员函数内不能修改成员变量。

12. 智能指针（*）

### 数据结构

1. 各类树结构的实现和应用
2. 各类排序：大根堆的实现，快排（如何避免最糟糕的状态？），bitmap 的运用等
3. hash 相关（如为什么一般 hashtable 的桶数会取一个素数？如何有效避免 hash 结果值的碰撞）

### 网络编程

1. tcp 和 udp 的区别？

2. udp 调用 connect 有什么作用？

3. tcp 连接中时序图和状态图（*）

4. socket 服务端的实现，select 和 epoll 的区别？

5. epoll 有哪些触发模式，有啥区别？

6. 大规模连接的场景，并发模型怎么设计？

7. tcp 结束连接怎么握手，time_wait 状态是什么，为什么会有 time_wait 状态？哪一方会有 time_wait 状态，如何避免 time_wait 状态占用资源？

8. tcp 头多少字节？哪些字段？

9. 什么是滑动窗口？

10. connect 会阻塞怎么解决？

    设置非阻塞，然后用 select 检测状态；

11. 如果 select 返回可读，结果只读到 0 字节，什么情况？

12. keepalive 是什么？如何使用？

13. 列举你知道的 tcp 选项，并说明其作用？

14. socket 什么情况下可读？

15. 流量控制与拥塞控制的区别，节点计算机怎样感知网络拥塞了？（*）

    在某段时间，若对网络中某一资源的需求超过了该资源所能提供的可用部分，网络性能就要变坏，这种情况就叫做网络拥塞。

    使用 tcp 的四种拥塞控制算法进行处理。

### 数据库

1. sql 语言
2. 服务器数据库大规模数据怎么设计？
3. db 各种性能指标

---

<a href="https://github.com/jvfan/jvfan.github.io/edit/master/tab_interview/README.md" style="color:#cccccc;">🖊 需要改动?点这里编辑</a>
