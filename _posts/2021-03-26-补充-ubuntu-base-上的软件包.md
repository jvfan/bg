---

layout:     post
title:      补充 ubuntu-base 上的软件包
subtitle:   按需补充软件包
date:       2021-03-26
author:     Jerry Chen
header-img: img/post-bg-coffee.jpeg
catalog: true
tags:
    - ubuntu-base
---

### ubuntu-base 必要软件包

1. lrzsz

   > 包含 rz、sz 等工具，支持 ssh、uart 方式；

   ```shell
   sudo apt-get install lrzsz
   ```

2. iputils-ping

   > 包含 ping 等工具；

   ```shell
   sudo apt-get install iputils-ping
   ```

3. sudo

   > 提权工具，会生成 sudo 用户组；

   ```shell
   sudo apt-get install sudo
   ```

4. curl

   > 文件传输工具；

   ```shell
   sudo apt-get install curl
   ```

5. wget

   > 专业下载工具；

   ```shell
   sudo apt-get install wget
   ```

6. net-tools [官方已停止维护]

   > 包含 ifconfig、route、arp、netstat 等工具；

   ```shell
   sudo apt-get install net-tools
   ```

7. iproute2

   > 包含 ip、ss（Socket Statistics） 命令，是 net-tools 的替代品；

   | 用途           | net-tools（被淘汰） | iproute2         |
   | -------------- | ------------------- | ---------------- |
   | 地址和链路配置 | ifconfig            | ip addr, ip link |
   | 路由表         | route               | ip route         |
   | 邻居           | arp                 | ip neigh         |
   | VLAN           | vconfig             | ip link          |
   | 隧道           | iptunnel            | ip tunnel        |
   | 组播           | ipmaddr             | ip maddr         |
   | 统计           | netstat             | ss               |

   ```shell
   sudo apt-get install iproute2
   ```

8. ifupdown

   > 包含 ifup、ifdown 等工具，相比 `ifconfig xxx up` 的优势是支持配置文件；

   ```shell
   sudo apt-get install ifupdwon
   ```

9. netcat

   > nc 是 Linux 下的一个用于调试和检查网络工具包，可用于创建 TCP/IP 连接；
   >
   > `nc -l 2389` 运行 tcp 服务器，监听 2389 端口；
   >
   > `nc localhost 2389` 连接本地 IP 的 2389 端口；

   ```shell
   sudo apt-get install netcat
   ```

10. telnet

    > 是一种远程登录的工具；
    >
    > 也可用于检查某个端口是否开放： `telnet IP:Port`

    ```shell
    sudo apt-get install telnet
    ```

11. ntp

    > 包含 ntpdate 等工具；

    ```shell
    sudo apt-get install ntp
    ```

12. sntp

    > 简单的 ntp 工具；

    ```shell
    sudo apt-get install sntp
    ```

13. kmod

    > 驱动加载卸载工具，包含 insmod、rmmod、lsmod 等；

    ```shell
    sudo apt-get install kmod
    ```

14. usbutils

    > 包含 lsusb 工具；

    ```shell
    sudo apt-get install usbutils
    ```

15. oepnssh-server

    > 包含 sshd、sftp 等工具；

    ```shell
    sudo apt-get install oepnssh-server
    ```

16. bc

    > 基础计算器；

    ```shell
    sudo apt-get install bc
    ```

17. nano

    > 轻量级编辑器；

    ```shell
    sudo apt-get install nano
    ```

18. rsyslog

    > 日志系统，还包括 cron 计划任务等；
    >
    > 记录接口：
    >
    > * 内核空间 c 程序：printk()
    > * 用户空间 c 程序：syslog()
    > * 用户空间 shell：logger

    ```shell
    sudo apt-get install rsyslog
    ```

19. systemd

    > 守护进程，还包括日志查看器 journalctl 等；

    ```shell
    sudo apt-get install systemd
    ```

20. udev

    > 管理设备，可监控热插拔、自动挂载 usb 设备等；

    ```shell
    sudo apt-get install udev
    ```

21. gdb

    > 调试工具；

    ```shell
    sudo apt-get install gdb
    ```

22. docker-ce

    安装办法：[点这里](https://docs.docker.com/engine/install/ubuntu/) 

### ubuntu-base 按需安装的软件包

1. tree：树形展示目录；
2. psmisc：包含 pstree 等命令，树形展示进程；
3. vim：编辑器，vi 升级版本；
4. bash-completion：bash 的自动补全增强工具，可补全一些命令的参数；
5. sysstat：包含 sar、iostat 等命令，展示磁盘 io 读写情况等；
6. iotop：显示每个进程的 io 读写情况 swap 写入情况；
7. iftop：查询网络流量情况；
8. nethogs：显示每个进程的流量情况；
9. htop：top 升级版本，有高亮等色彩，还可交互；
10. unzip：解压工具；
11. nmap：网络端口扫描；
12. lsof：显示所有被打开的文件；
13. bind-utils：包含 dig、nslookup、host 等 DNS 相关工具；

