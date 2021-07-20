---

layout:     post
title:      tcpdump + wireshark 配合使用进行抓包分析
subtitle:   tcpdump 抓包，wireshark 解析
date:       2021-03-23
author:     Jerry Chen
header-img: img/post-bg-coffee.jpeg
catalog: true
tags:
    - tcpdump
    - wireshark
---

### Linux 下使用 tcpdump 抓包

#### 进行监听

```shell
tcpdump -i FE0 host 192.168.2.3 -w hello.cap
```

* `-i FE0` 表示使用 FE0 网卡；
* `host 192.168.2.3` 表示筛选含有 IP 为 `192.168.2.3` 的包文件；
* `-w hello.cap` 表示保存为 hello.cap 文件；

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20210323163344.png)

注意监听完成后，按下 ctrl+C 停止监听才会立刻保存信息包到文件；

#### 模拟和 192.168.2.3 通信

开启另一个控制台，ping 目标地址：

```shell
ping 192.168.2.3
```

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20210323164103.png)

#### 停止监听

按下 ctrl+C 停止 tcpdump，会在当前目录生成一个 hello.cap 的文件，拷贝该文件到 windows；

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20210323164547.png)

### Windows 下使用 wireshark 解析抓包文件

> wireshark 使用需要先安装 npcap（nmap 的组件之一）  [这是官网](https://nmap.org/npcap/)  [点这里下载](https://nmap.org/npcap/dist/npcap-1.31.exe) 

使用 wireshark 打开 hello.cap，在上方输入 icmp 即可筛选心跳帧（4 发 4 收）；

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20210323164916.png)

更多的过滤条件（这些条件还可以使用 and 和 or 进行连接）：

* `tcp.port == 80` 过滤来自 80 端口的 TCP 数据
* `udp.port == 12345` 过滤来自 12345 端口的 UDP 数据
* `ip.src == 192.168.0.1` 过滤源 IP 为 192.168.0.1 的数据
* `ip.dst == 192.168.0.1` 过滤目的 IP 为 192.168.0.1 的数据

