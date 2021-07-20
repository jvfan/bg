---

layout:     post
title:      使用 Dockerfile 制作容器镜像
subtitle:   标准办法进行制作
date:       2021-03-26
author:     Jerry Chen
header-img: img/post-bg-coffee.jpeg
catalog: true
tags:
    - Dockerfile
---

### 确定基础镜像

首先在 [docker-hub](https://hub.docker.com/) 中搜索目标镜像作为我们的基础镜像：

1. 搜索 ubuntu

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20210326190442.png)

2. 点击进去后在 Tags 下找到 ubuntu:bionic 的 armv7 版本

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20210326184630.png)

### 制作 Dockerfile 并构建

1. 新建一个目录 mydocker，并进入该目录

   ```shell
   mkdir mydocker
   cd mydocker
   ```

2. 新建 appctl-daemon.c，并交叉编译生成 appctl-daemon

   ```shell
   arm-linux-gnueabihf-gcc appctl-daemon.c -o appctl-daemon
   ```

   appctl-daemon.c 的内容如下：

   ```c
   #include <stdio.h>
   #include <unistd.h>
   
   int main(int argc, char *argv[])
   {
       while(1){
           printf("hello world!\n");
           sleep(5);
       }
       return 0;
   }
   ```

3. 拷贝主机侧的时区文件到当前目录，命名为 localtime

   ```shell
   cp /usr/share/zoneinfo/Asia/Shanghai localtime
   ```

4. 新建 Dockerfile

   > 确保当前路径存在 localtime、appctl-daemon；
   >
   > FROM 中填写我们确定好的基础镜像；
   >
   > 语法参考：[官网文档](https://docs.docker.com/engine/reference/builder/) 

   ```dockerfile
   FROM --platform=linux/arm/v7 ubuntu:bionic
   COPY ./localtime /etc/localtime
   RUN echo 'Asia/Shanghai' >/etc/timezone \
       && mkdir /usr/local/monitor \
       && mkdir /home/zxlib \
       && mkdir /usr/local/exapps
   COPY ./appctl-daemon /usr/local/monitor/
   RUN chmod +x /usr/local/monitor/appctl-daemon
   ENTRYPOINT /usr/local/monitor/appctl-daemon
   ```

   此时当前目录下应该存在如下文件：

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20210326185558.png)

5. 进行构建

   ```shell
   sudo docker build -t basic_img-arm:1.0 .
   ```

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20210326185846.png)

### 运行制作好的容器镜像

> 每隔 5s 打印一次 "hello world"，和预期一致；

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20210326190124.png)