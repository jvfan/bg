---
layout:     post
title:      实现网络应用：ftp、tftp、nfs
subtitle:   基于最小linux实现网络数据传输和共享
date:       2019-7-21
author:     Jerry Chen
header-img: img/post-bg-debug.png
catalog: true
tags:
    - 应用
    - ftp
    - tftp
    - nfs
---



### 准备工作

基于上一篇文章：[搭建最小linux](/2019/07/20/使用BusyBox搭建最小文件系统/)

#### 确保同一网络环境

主要目标：开发板能ping通其他设备，其他设备亦可ping通开发板。

1. 连接网线，更新开发板`/etc/eth0-setting`文件，配置为同一网段地址。

   下图是我的配置，路由器ip是`192.168.2.1`，故我配置成了没被使用的`192.168.2.200`

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20190725040159.png)

   **重启开发板**，使用`ifconfig`查看inet地址变为设置值就基本ok了。

2. 设备之间相ping

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20190725040818.png)

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20190725040935.png)



#### 准备网络应用程序包

| 应用       | 对象                  | 使用方法                          | Web     |
| ---------- | --------------------- | --------------------------------- | ------- |
| ftp服务器  | 开发板                | [开始](#开发板启动ftp服务器)      | 自带    |
| ftp客户端  | Windows电脑文件管理器 | [使用](#windows文件管理器浏览ftp) | 自带    |
| tftp服务器 | Ubuntu12.04           | [开始](#ubuntu搭建tftp服务器)     | apt-get |
| tftp客户端 | 开发板                | [使用](#开发板获取tftp文件)       | 自带    |
| nfs服务器  | Ubuntu12.04           | [开始](#ubuntu搭建nfs服务器)      | apt-get |
| nfs客户端  | 开发板内核            | [使用](#开发板连接nfs服务器)      | 内核    |



### FTP服务

#### 开发板启动ftp服务器

开发板BusyBox自带ftp服务器应用，直接输入以下命令即可开启：

```
tcpsvd 0 21 ftpd -a root -w / &
```

#### Windows文件管理器浏览ftp

在PC文件管理器的地址栏输入开发板的ftp地址，即可**双向传递**文件。

```
ftp://192.168.2.200
```

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20190725043750.png)



### TFTP服务

#### Ubuntu搭建tftp服务器

1. 安装xinetd

   ```
   apt-get install xinetd
   ```

2. 安装tftp和tftpd

   ```
   apt-get install tftp tftpd
   ```

3. 建立tftp的配置文件

   ```
   vim /etc/xinetd.d/tftp
   ```

   内容如下（**务必检查tab填充，否则配置不生效**）：

   ```
   service tftp
   {
           socket_type     = dgram
           protocol                = udp
           wait                    = yes
           user                    = root
           server          = /usr/sbin/in.tftpd
           server_args     = -s /var/tftpboot
           disable         = no
           per_source      = 11
           cps                     = 100 2
           flags                   = IPv4
   }
   ```

4. 建立tftp服务器的文件目录

   ```
   mkdir /var/tftpboot
   ```

5. 重启xinetd服务，使配置生效

   ```
   sudo /etc/init.d/xinetd restart
   ```



接下来是**安装完成后的本机小测试**（只适用于上位机）

已知在本机tftp服务器根目录有文件hello（请事先创建），使用tftp下载到当前目录下：

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20190725052828.png)



#### 开发板获取tftp文件

开发板BusyBox自带tftp客户端应用，输入以下命令获取远程tftp的hello文件：

```
tftp 192.168.2.246 -g -l hello -r hello
```

一些参数信息：

`-g`下载文件；`-p`上传文件；`-l`本地文件名；`-r`远程文件名；

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20190725053319.png)



### NFS服务

#### Ubuntu搭建nfs服务器

1. 安装nfs服务

   ```
   apt-get install nfs-kernel-server
   ```

2. 创建共享文件夹

    ```
    mkdir /home/nfsroot
    ```

3. 修改添加共享目录配置

   ```
   vim /etc/exports
   ```

   在最后加上一句，给路径增加读写、同步写入内存硬盘、客户端root权限属性：（这些一定要加，否则可能会因为权限而拒绝）

   ```
   /home/nfsroot/ *(rw,sync,no_root_squash)
   ```

4. 重启服务

   ```
   /etc/init.d/portmap restart
   /etc/init.d/nfs-kernel-server restart
   ```

接下来是**安装完成后的本机小测试**（只适用于上位机）

已知在本机nfs共享目录有文件hello（请事先创建），使用mount挂载共享目录到`/mnt/nfs`目录下：

```
mkdir /mnt/nfs
mount -t nfs localhost:/home/nfsroot /mnt/nfs
```

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20190725061422.png)

取消上面的挂载执行：`umount /mnt/nfs`



#### 开发板连接nfs服务器

需要配置内核支持nfs，注意是kernel，不是最小linux文件系统。

##### 内核配置

此步骤在虚拟机中进行。

关于内核的下载，由于文件太大自行在光盘中寻找。

1. 进入解压后的内核目录，将配置文件替换为QtE的config文件

   ```
   cp -r config_for_linux_scp_elite .config
   ```

2. 进入配置界面，配置内核

   ```
   make menuconfig
   ```

   更改项清单如下：

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20190725121127.png)

   不更改红框中的项就可以跳到下一步，否则继续看本步骤下面的内容。

   内容A举例，下面断行非回车而是空格：

   ```
   root=/dev/nfs rw nfsroot=192.168.2.246:/home/nfsroot ip=192.168.2.200:192.168.2.246:192.168.2.1:255.255.255.0:iTOP:eth0:off rootfstype=ext4 init=/linuxrc console=ttySAC2,115200
   ```

   其中`192.168.2.246:/home/nfsroot`是搭建好的nfs服务器配置过的共享目录；`192.168.2.200:192.168.2.246:192.168.2.1:255.255.255.0:iTOP:eth0:off`分别是`开发板ip:nfs服务器ip:网关:子网掩码:任意名称:网卡名称:off`；

3. 配置ok后，编译生成内核镜像

   ```
   make zImage
   ```
   
4. **仅烧录内核**镜像到开发板，参见[烧录](/2019/06/18/3_掌握必要的烧写系统的技能/)

##### 开始连接

- **内核仅使能nfs客户端功能**

  开发板输入以下命令直接进行挂载（nolock很重要）

  ```
  mkdir /mnt/nfs
  mount -t nfs 192.168.2.246:/home/nfsroot /mnt/nfs -o nolock
  ```

  结果是远程目录（对内容无要求）被挂载到了本地目录中。

  `df`查看挂载；`umount /mnt/nfs`取消挂载；

  ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20190725121957.png)

- **内核使能nfs根文件系统**

  顾名思义，也就是以远程的文件作为系统进行启动，所以需要远程nfs共享目录里是一个完善的文件系统文件夹。

  1. 先清空nfs服务器上共享目录，然后**将最小linux的文件**全部拷贝进去。
  2. 万分注意**服务器共享目录**中`etc/init.d/ifconfig-eth0`第8行更改为`if grep -q nfs /proc/mounts ; then`
  3. 烧录完内核，启动开发板试试。

  如果提示`nfs: server 192.168.2.246 not responding, still trying`优先确认连接和nfs上的共享目录问题，因为使用nfs根文件系统后就完全不会使用到mmc里的文件系统了。
  
  ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20190725145829.png)

