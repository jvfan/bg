---

layout:     post
title:      Docker 运行 GUI 程序
subtitle:   wslg + docker-desktop 环境下在容器中运行 gui 程序
date:       2022-09-02
author:     Jerry Chen
header-img: img/post-bg-coffee.jpeg
catalog: true
tags:
    - WSLg
    - docker-desktop
---

## 在 wsl2 运行步骤

### 运行环境

> 使用 wslg 中自带的 xserver 环境；

* wsl2 ubuntu22.04 环境，并且升级为了 wslg；

  升级方法：使用管理员权限 powershell 运行下面命令更新 wsl2 内核：

  ```powershell
  wsl --update
  ```

* wsl2 后端的 docker-desktop 环境；

### 主机侧的操作

1. 安装 x11-xserver-utils

   ```shell
   sudo apt install x11-xserver-utils
   ```

2. 允许所有用户访问 x11 的显示接口

   > 每新开启一个新终端都需要执行一次才能在当前终端中生效；

   ```shell
   xhost +
   ```

### docker 相关操作

1. docker 安装 ubuntu22.04

   ```shell
   docker run -it \
   -v /etc/localtime:/etc/localtime:ro \
   -v /tmp/.X11-unix:/tmp/.X11-unix \
   -e DISPLAY=unix$DISPLAY \
   -e GDK_SCALE \
   -e GDK_DPI_SCALE \
   ubuntu:jammy
   ```

2. 在上一步的容器中替换软件源：

   ```shell
   sed -i "s@http://.*archive.ubuntu.com@http://mirrors.tuna.tsinghua.edu.cn@g" /etc/apt/sources.list
   sed -i "s@http://.*security.ubuntu.com@http://mirrors.tuna.tsinghua.edu.cn@g" /etc/apt/sources.list
   ```

3. 安装 gedit：

   ```shell
   apt update
   apt install gedit -y
   ```

4. 运行 gedit：

   ```shell
   gedit
   ```

## 在 windows 运行步骤

### 运行环境

> 使用 VcXsrv 的 xserver 环境；

* wsl2 ubuntu22.04 环境；
* wsl2 后端的 docker-desktop 环境；
* windows 上安装了 xserver（VcXsrv）；

### 主机侧的操作

运行 xserver（VcXsrv）；

### docker 相关操作

1. docker 安装 ubuntu22.04

   ```shell
   docker run -it \
   -e DISPLAY=host.docker.internal:0.0 \
   ubuntu:jammy
   ```

2. 在上一步的容器中替换软件源：

   ```shell
   sed -i "s@http://.*archive.ubuntu.com@http://mirrors.tuna.tsinghua.edu.cn@g" /etc/apt/sources.list
   sed -i "s@http://.*security.ubuntu.com@http://mirrors.tuna.tsinghua.edu.cn@g" /etc/apt/sources.list
   ```

3. 安装 gedit：

   ```shell
   apt update
   apt install gedit -y
   ```

4. 运行 gedit：

   ```shell
   gedit
   ```

