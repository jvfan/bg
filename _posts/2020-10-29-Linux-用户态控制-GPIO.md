---

layout:     post
title:      Linux 用户态控制 GPIO
subtitle:   交互式控制 GPIO
date:       2020-10-29
author:     Jerry Chen
header-img: img/post-bg-debug.png
catalog: true
tags:
    - GPIO
---

### 内核配置

> 一般情况下，内核已配置支持 GPIO；

```
#make menuconfig ARCH=arm

  -> GPIO Support (GPIOLIB [=y])
```

启用支持后，可以通过用户态 `/sys/class/gpio/` 目录控制 GPIO；

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20201029105747.png)

### 操作方法

#### 1.导出一个引脚作为 GPIO

> GPIO 号可去类似 `linux/arch/arm/mach-s3c6400/include/mach/gpio.h` 的文件查询；

```shell
#227 为 GPIO 号
echo 227 > export
```

如果引脚申请成功，就会出现一个 `gpio227/` 的目录：

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20201029111003.png)

#### 2.操作 gpio227

进入 `gpio227` 目录，会有很多属性文件：

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20201029112137.png)

##### 设置 gpio 方向

```shell
echo "out" > direction
echo "in" > direction
```

##### 设置 gpio 输出值

> 需要 gpio 方向为 out；

```shell
echo 1 > direction
echo 0 > direction
```

##### 设置 gpio 中断触发模式

> 需要 gpio 方向为 in；

```shell
# 非中断引脚
echo "none" > edge
# 上升沿触发
echo "rising" > edge
# 下降沿触发
echo "falling" > edge
# 边沿触发（上升下降沿都触发）
echo "both" > edge
```

#### 3.释放导出的引脚

> 需要回到 `/sys/class/gpio/` 目录执行；

```shell
#227 为 GPIO 号
echo 227 > unexport
```

### 监控中断

1. 操作 gpiolib

   ```shell
   cd /sys/class/gpio
   echo 227 > export
   
   cd gpio227
   echo "in" > direction
   echo "both" > edge
   ```

2. 编写小程序，使用 poll 监控 value 的 fd 的事件

   ```c
   #include <stdio.h>
   #include <unistd.h>
   #include <fcntl.h>
   #include <poll.h>
   
   int main(){
       int fd = open("/sys/class/gpio/gpio227/value", O_RDONLY);
       if (fd < 0)    {
           perror("open '/sys/class/gpio/gpio227/value' failed!\n");
           return -1;
       }
   
       struct pollfd fds[1];    //仅监控 1 个 fd
       fds[0].fd = fd;          //将目标 fd 加入监控
       fds[0].events = POLLPRI; //设置轮询关心的事件：有紧急数据可读
       while (1){
           if (poll(fds, sizeof(fds), 0) == -1){ //阻塞 0s，会重新赋值事件
               perror("poll failed!\n");
               return -1;
           }
   
           if (fds[0].revents & POLLPRI){ //有紧急数据可读 事件实际发生
               if (lseek(fd, 0, SEEK_SET) == -1){
                   perror("lseek failed!\n");
                   return -1;
               }
   
               char buffer[16];
               int len;
               if ((len = read(fd, buffer, sizeof(buffer))) == -1){
                   perror("read failed!\n");
                   return -1;
               }
               buffer[len] = 0;
               printf("%s", buffer);
           }
       }
       return 0;
   }
   ```

3. 实际操作 gpio227，物理拉高拉低 gpio227，观察打印“1” ‘0’；

