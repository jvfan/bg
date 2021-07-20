---

layout:     post
title:      mosquitto 的基本编译
subtitle:   windows、linux、arm 三端编译
date:       2021-01-28
author:     Jerry Chen
header-img: img/post-bg-coffee.jpeg
catalog: true
tags:
    - mosquitto
---

> 本文介绍不带 SSL/TLS（通常指 HTTPS 的协议加密层），不带 CJSON，带 pthread 的情况下编译 mosquitto；

### CMakeLists.txt 脚本的修改

1. 从 [官网](https://mosquitto.org/download) 下载 mosquitto 源码，并解压

2. 根据下面的描述修改 CMakeLists.txt 脚本

   * 屏蔽 TLS

     ```diff
     + set (WITH_TLS OFF)
     if (WITH_TLS)
     ```

   * 屏蔽 CJSON

     ```diff
     - FIND_PACKAGE(cJSON)
     + #FIND_PACKAGE(cJSON)
     + set (CJSON_FOUND OFF)
     if (CJSON_FOUND)
     ```

   * 屏蔽 CLIENTS

     > Windows 平台编译 client_shared.c 会报错（可能是 VS2019 的 CMake 工程对某些写法不支持的原因），所以设置不生成 clients；

     ```diff
     + set (WITH_CLIENTS OFF)
     if (WITH_CLIENTS)
     ```
     
   * 屏蔽 DOCUMENTATION
   
     > 屏蔽 TLS 后 Linux 平台生成文档会卡住，所以设置不生成文档；
   
     ```diff
     + set (DOCUMENTATION OFF)
     if (DOCUMENTATION)
     ```

### 多平台编译库文件

#### Windows 平台

##### 安装 pthreads-w32 库

在 [这里](http://sourceware.org/pub/pthreads-win32/) 下载最新版本 [pthreads-w32-2-9-1-release.zip](http://sourceware.org/pub/pthreads-win32/pthreads-w32-2-9-1-release.zip)，解压后拷贝 pthreads-w32-2-9-1-release 目录到 C 盘，并重命名为 pthreads；

确保 CMakeLists.txt 脚本中 WIN32 的 pthread 库路径能正常找到；

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20210128113630.png)

##### 解决 pthreads-w32 的 timespec 和 Windows SDK 重定义的问题

修改 C:\pthreads\Pre-built.2\include\pthread.h 文件：

```diff
#if !defined( PTHREAD_H )
#define PTHREAD_H
+ #define HAVE_STRUCT_TIMESPEC
```

##### VS2019 CMake 工程编译

1. 使用 VS2019 打开 mosquitto 目录下的 CMakeLists.txt；

   > 会在工程目录下多出 .vs 和 out 目录；

   ![img](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20201209121817.png)

2. 等待 cmake 缓存构建完成后，“生成” --> “全部生成”；

   > C 库文件在 lib 目录，CPP 库文件在 lib/cpp 目录；

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20210128115132.png)

#### Linux 平台

按如下步骤编译：

```shell
mkdir build
cd build
cmake ..
make
```

> C 库文件在 lib 目录，CPP 库文件在 lib/cpp 目录；

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20210128120713.png)

#### arm32 平台

1. 修改 CMakeLists.txt，设置交叉编译器：

   > 建议紧接着 project(xxx) 后面，即下一行；

   ```cmake
   set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
   set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)
   ```

2. 启动编译

   ```shell
   mkdir build
   cd build
   cmake ..
   make
   ```

   > C 库文件在 lib 目录，CPP 库文件在 lib/cpp 目录；

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20210128120713.png)

