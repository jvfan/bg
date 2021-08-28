---
layout:     post
title:      新装 WSL2 以及安装 docker 和 code_server
subtitle:   关于如何新装和使用 WSL2
date:       2020-05-17
author:     Jerry Chen
header-img: img/post-bg-debug.png
catalog: true
tags:
    - WSL2
    - WSLg
    - docker
    - code-server
---

> 需要 windows10 升级到 2004

### 准备工具

- WSL2-Ubuntu20.04：windows 下的 linux 子系统
- [GitHub：code-server](https://github.com/cdr/code-server)

### WSL2 部分

#### 安装 WSL2

1. 首先启用 Windows 功能

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/image-20200517152329817.png)

   另外通过 `PowerShell` 命令行也可以开启：

   ```powershell
   dism.exe /online /enable-feature /featurename:Microsoft-Windows-Subsystem-Linux /all /norestart
   dism.exe /online /enable-feature /featurename:VirtualMachinePlatform /all /norestart
   ```

2. 更新 WSL 2 Linux 内核

   点此下载内核更新包 [X86](https://wslstorestorage.blob.core.windows.net/wslblob/wsl_update_x64.msi) 

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/image-20200517155913532.png)

3. 将 WSL2 设置为默认版本

   通过管理员权限 `PowerShell` 命令行进行配置：

   ```powershell
   wsl --set-default-version 2
   ```

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/image-20200517160902059.png)

4. 接着就可以去应用商店安装 Linux 分发版了

#### 安装 Linux 分发版

1. 打开应用商店搜索 “Ubuntu 20”并安装

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/image-20200517163132629.png)

2. 安装完成后启动一次

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/image-20200517163317374.png)

3. 查看已经安装的 WSL 版本

   管理员权限 `PowerShell` 中输入：

   ```powershell
   wsl -l -v
   ```

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/image-20200517163507513.png)

4. 如果曾经安装过 WSL 一代，可使用如下命令更改版本（也可2降1）

   管理员权限 `PowerShell` 中输入：

   ```powershell
   wsl --set-version Ubuntu-20.04 2
   ```

#### [可选] 配置 Ubuntu20.04（阿里云仓库）

1. 换阿里源

   ```shell
   sudo vim /etc/apt/sources.list
   ```

   用下面的内容替换原内容：

   ```
   deb http://mirrors.aliyun.com/ubuntu/ focal main restricted universe multiverse
   deb-src http://mirrors.aliyun.com/ubuntu/ focal main restricted universe multiverse
   
   deb http://mirrors.aliyun.com/ubuntu/ focal-security main restricted universe multiverse
   deb-src http://mirrors.aliyun.com/ubuntu/ focal-security main restricted universe multiverse
   
   deb http://mirrors.aliyun.com/ubuntu/ focal-updates main restricted universe multiverse
   deb-src http://mirrors.aliyun.com/ubuntu/ focal-updates main restricted universe multiverse
   
   deb http://mirrors.aliyun.com/ubuntu/ focal-proposed main restricted universe multiverse
   deb-src http://mirrors.aliyun.com/ubuntu/ focal-proposed main restricted universe multiverse
   
   deb http://mirrors.aliyun.com/ubuntu/ focal-backports main restricted universe multiverse
   deb-src http://mirrors.aliyun.com/ubuntu/ focal-backports main restricted universe multiverse
   ```

2. 更新缓存和升级

   ```shell
   sudo apt-get update
   sudo apt-get upgrade
   ```

#### [可选] wsl2 升级 wslg

> 先决条件 1：已经安装 wsl2；
>
> 先决条件 2：win10 内部版本在 21362+ 或者系统是 win11；

1. 下载 gpu 驱动

   > 因特尔或 AMD 的 GPU 驱动必装，Nvidia 可选；

   - [WSL 的英特尔 GPU 驱动程序](https://downloadcenter.intel.com/download/30579/Intel-Graphics-Windows-DCH-Drivers)
   - [WSL 的 AMD GPU 驱动程序](https://community.amd.com/community/radeon-pro-graphics/blog/2020/06/17/announcing-amd-support-for-gpu-accelerated-machine-learning-training-on-windows-10)
   - [WSL 的 Nvidia GPU 驱动程序](https://developer.nvidia.com/cuda/wsl)

2. 管理员权限 powershell 命令更新到 wslg

   ```powershell
   wsl --update
   ```

3. [可选] 在 linux shell 中安装一些 gui 应用

   ```shell
   # 在 linux shell 中执行
   sudo apt update
   
   # Gedit 编辑器
   sudo apt install gedit -y
   
   # Nautilus 文件管理器
   sudo apt install nautilus -y
   ```

#### [可选] 解决 wsl2 出现 Vmmem 内存占用过大

1. 进入 WSL2，设置定时任务：每 15 分钟删除缓存，并创建一个文件

   `sudo crontab -e -u root` 添加如下的行：

   ```shell
   */15 * * * * sync; echo 3 > /proc/sys/vm/drop_caches; touch /root/drop_caches_last_run
   ```

2. 开机自启动 cron 服务

   `vim ~/.bashrc` 添加如下的行：

   ```shell
   [ -z "$(ps -ef | grep cron | grep -v grep)" ] && sudo /etc/init.d/cron start &> /dev/null
   ```

3. 允许启动 cron 服务而无需输入 root 密码

   `sudo visudo` 添加以下行：

   ```
   %sudo ALL=NOPASSWD: /etc/init.d/cron start
   ```

4. 重启 WSL2

   powershell 上杀死 WSL `wsl --shutdown`，然后再次启动它；

5. 跟踪定时任务，判断它是否正常工作

   因为在定时任务中创建了 /root/drop_caches_last_run 文件，可以通过查看该文件的修改日期跟踪目标任务的工作情况：

   ```shell
   sudo stat -c '%y' /root/drop_caches_last_run
   ```

#### [可选] 限制 WSL2 内存使用

1. 先使用管理员权限 powershell 命令关闭 wsl： `wsl --shutdown`

2. 接着在用户文件夹下新建一个 .wslconfig 文件  `C:\Users\<yourUserName>\.wslconfig`，内容如下：

   ```
   [wsl2]
   memory=3GB
   swap=3GB
   localhostForwarding=true
   ```

#### [可选] 使用 wsl 命令迁移 WSL 到非系统盘

1. 使用 powershell 关闭所有发行版：

   ```powershell
   wsl --shutdown
   ```

2. 导出 ubuntu20.04 发行版：

   ```powershell
   wsl --export Ubuntu-20.04 D:\WSL\Ubuntu-20.04.tar
   ```

3. 注销 ubuntu20.04 发行版：

   ```powershell
   wsl --unregister Ubuntu-20.04
   ```

4. 从 `D:\WSL\Ubuntu-20.04.tar` 导入 ubuntu20.04 发行版到文件夹 `D:\WSL\Ubuntu-20.04` ：

   ```powershell
   wsl --import Ubuntu-20.04 D:\WSL\Ubuntu-20.04 D:\WSL\Ubuntu-20.04.tar --version 2
   ```

5. [可选] 删除中间导出文件：

   ```powershell
   rm D:\WSL\Ubuntu-20.04.tar
   ```

#### [可选] 使用 LxRunOffline 迁移 WSL 到非系统盘

下载一个编译好的 [LxRunOffline](https://github.com/DDoSolitary/LxRunOffline/releases)（我下载的是 LxRunOffline-v3.5.0-msvc.zip），解压缩后使用该工具进行管理；

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20200923095300.png)

* 查看已经安装的 WSL：

  ```powershell
  .\LxRunOffline.exe list
  ```

  ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20200923095614.png)

* 迁移 Ubuntu-20.04

  先使用命令关闭 wsl： `wsl --shutdown`

  ```powershell
  .\LxRunOffline.exe move -n Ubuntu-20.04 -d E:\WSL\Ubuntu-20.04
  ```

* 查看 Ubuntu-20.04 所在目录

  ```powershell
  .\LxRunOffline.exe get-dir -n Ubuntu-20.04
  ```

  ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20200923100332.png)

### docker 部分

#### 安装预编译 docker-ce（阿里云仓库）

> 注意：docker.io 是旧版本，这里是安装社区版本的 docker-ce

1. 安装 docker

   ```shell
   # step 1: 安装必要的一些系统工具
   sudo apt-get update
   sudo apt-get -y install apt-transport-https ca-certificates curl software-properties-common
   # step 2: 安装GPG证书
   curl -fsSL http://mirrors.aliyun.com/docker-ce/linux/ubuntu/gpg | sudo apt-key add -
   # Step 3: 写入软件源信息
   sudo add-apt-repository "deb [arch=amd64] http://mirrors.aliyun.com/docker-ce/linux/ubuntu $(lsb_release -cs) stable"
   # Step 4: 更新并安装Docker-CE
   sudo apt-get -y updatesudo apt-get -y install docker-ce
   ```

2. 解决遇到的问题“no installation candidate”

   原因是 Ubuntu20.04 的软件库中没有 docker-ce；

   改用 Ubuntu19.04(Disco) 的软件库即可，方法如下：

   * 去掉 `/etc/apt/sources.list` 中新增的docker源

   * 输入以下命令新增 disco 的 docker 源

     > 安装完成后，可以把该源删除

     ```shell
     sudo add-apt-repository "deb [arch=amd64] http://mirrors.aliyun.com/docker-ce/linux/ubuntu disco stable"
     ```

   * 再次安装

     > 如果出现 `configuring grub-pc` 就直接回车，不进行选择（三个框留空）；

     ```shell
     sudo apt-get -y update
     sudo apt-get -y install docker-ce
     ```

#### docker 配置（包括添加用户到 docker 组）

1. 启动服务（每次开机都要执行一次）

   > WSL2 上没有 systemctl

   ```shell
   sudo service docker start
   ```

2. 初步进行测试

   ```shell
   sudo docker run hello-world
   ```

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/image-20200517202211982.png)

3. [可选] 配置国内 docker 源（163源）

   ```shell
   sudo vim /etc/docker/daemon.jsonsudo service docker restart
   ```

   添加如下内容：

   > 注意 tab 必须写正确。

   ```
   {
           "registry-mirrors": ["https://registry.docker-cn.com","http://hub-mirror.c.163.com"]
   }
   ```

4. [可选] 添加当前用户到 docker 用户组，这样可以不加 sudo 运行 docker

   ```shell
   sudo adduser $USER docker
   # 刷新 docker 组
   newgrp docker
   ```

   测试运行结果：

   > 如果不行就重启服务，关闭窗口再重新打开

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/image-20200517202718881.png)

### code-server 部分

#### 1、docker 方式

1. docker 安装 code-server

   ```shell
   sudo mkdir -p /home/coder/project
   sudo chown -R jerry /home/coder
   docker run --name code-server -d --env PASSWORD="ojbk" -p 127.0.0.1:8080:8080 -v "/home/coder/project:/home/coder/project" -u "$(id -u):$(id -g)" codercom/code-server:latest
   ```

2. 浏览器访问 `localhost:8080` 

   > 端口是设置的 8080；
   >
   > 密码是环境变量设的 “ojbk”；

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/image-20200517214316300.png)

3. 设置容器自启动

   > 依赖于 docker 服务已启动

   ```shell
   docker update --restart=always code-server
   ```

#### 2、发布版方式

1. 下载

   [code-server 发布版下载](https://github.com/cdr/code-server/releases)

2. 解压后运行

   ```shell
   export PASSWORD="123456"
   ./code-server --bind-addr localhost:8080
   ```
