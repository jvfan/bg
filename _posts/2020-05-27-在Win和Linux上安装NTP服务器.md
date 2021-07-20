---
layout:     post
title:      在 Win 和 Linux 上安装NTP服务器
subtitle:   安装 NTP 服务器和问题的解决
date:       2020-05-27
author:     Jerry Chen
header-img: img/post-bg-coffee.jpeg
catalog: true
tags:
    - 桥接网络
    - NTP 对时
    - NTP 服务器
---

### NTP 工作原理

1. NTP（Network Time Protocol）基于 UDP 协议进行传输，端口号是 123；

2. 同步时钟过程需要记录四个时间点：

   * 离开被同步设备时间 T1；
   * 达到同步设备时间 T2；
   * 离开同步设备时间 T3（标准时间）；
   * 回到被同步设备时间 T4

   由此可计算往返延迟时间为：`(T4-T1)-(T3-T2)`，设备时间差 Δt 为：`((T2-T1)+(T4-T3))/2`；

   认为 T3 是标准时间，那么 T4 时刻的标准时间是：T3+Δt；

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/image-20200527140429395.png)

### Ubuntu 上安装 NTP

#### [可选]对虚拟机对时

为了在 Ubuntu 虚拟机上搭建一个 NTP 服务器，首先应保证其时间准确，故先对 Ubuntu 进行对时；

```
# 同步阿里云的 NTP 服务器时间
sudo ntpdate ntp1.aliyun.com
```

此外，阿里云提供了 7 个 NTP 时间服务器：

```
ntp1.aliyun.com
ntp2.aliyun.com
ntp3.aliyun.com
ntp4.aliyun.com
ntp5.aliyun.com
ntp6.aliyun.com
ntp7.aliyun.com
```

#### 安装 NTP 服务器

1. 安装 ntp 软件包

   ```
   sudo apt install ntp -y
   ```

   安装完成后，查看所有服务状态：

   ```
   service --status-all
   ```

   如下表示 ntp 服务已经正常启动：

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/image-20200527143801373.png)

2. 修改配置 `sudo vim /etc/ntp.conf`

   * server 关键字设定上层 NTP 服务器

     ```
     # perfer 表示『优先使用』的服务器
     server [IP or hostname] [prefer]
     ```

   * restrict 关键字用来管理权限

     ```
     restrict [IP] [mask netmask_IP] [parameter]
     ```

     如果你没有在 parameter 的地方加上任何参数的话，这表示『该 IP 或网段不受任何限制』；

   * driftfile 关键字记录时间差异

     ```
     driftfile [可以被ntpd写入的目录与档案]
     ```

   我的最终配置：

   ```
   # 将原来所有 server 关键字语句全部屏蔽，然后加入
   server ntp1.aliyun.com prefer
   server ntp2.aliyun.com
   server ntp3.aliyun.com
   server ntp4.aliyun.com
   
   # 加入网段允许对时（不允许修改服务器时间）
   restrict 192.168.2.0 mask 255.255.255.0 nomodify
   ```

3. 重启 NTP 服务

   ```
   sudo service ntp restart
   # 或者直接脚本重启
   sudo /etc/init.d/ntp restart
   ```

4. 观察 NTP 的运行状态

   > 当客户端使用 ntpdate 进行对时的时候，对时服务器先和上层 NTP 服务器对时；

   ```
   watch ntpq -p
   ```

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/image-20200527151246261.png)

特别注意：

如果开启了 ntp 服务器，又要使用 ntpdate 进行手动对时，就需要先把 ntp 服务器先停止：

```
sudo service ntp stop
```

#### 虚拟机设置桥接的正确方法

1. 进入虚拟网络编辑器添加一个桥接

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/image-20200527170142022.png)

   添加好后如下：

   * 一个桥接
   * 一个 NAT

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/image-20200527170848375.png)

2. 编辑虚拟机设置

   > 原网络适配器使用的是 NAT，这里给虚拟机添加一个新网络。

   添加一个网络适配器，并配置为自定义桥接模式。

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/image-20200527171134165.png)

3. 登入到虚拟机中，会发现多了一个 eth1

   如果网关有 DHCP，那么 eth1 会自动分配 IP；

   如果没有网关，那么手动设置 IP：

   > 最好进 ubuntu 可视化手动配置 IPV4；

   ```
   # 我桥接的网卡 IP 为 192.168.2.1
   # 我这里桥接 IP 设为 192.168.2.3
   sudo ip addr add 192.168.2.3/24 dev eth1
   ```

   使用我的嵌入式板（IP: 192.168.2.2）ping 一下：

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/image-20200527172822714.png)

#### 在嵌入式板上测试 NTP 服务器

> 我的 Ubuntu 桥接的 IP 为 192.168.2.3
>
> 如果失败，就先关闭 Win 上相应网络的防火墙再尝试；

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20200528140147.png)

### Windows 10 上安装 NTP

> 操作之前确保当前用户不受域管理，如受域管理请备份好系统盘文件后，尝试退出域，这个操作会删除当前用户；

#### [可选]对 Win10 对时

路径一：

“控制面板” --> “时钟和区域” --> “设置时间和日期” --> “Internet 时间（选项卡）” --> “更改设置”

路径二：

“设置” --> “日期和时间” --> “添加不同时区的时钟” --> “Internet 时间（选项卡）” --> “更改设置”

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20200528142528.png)

更改服务器为：`ntp1.aliyun.com`

接着点击“立即更新”即可。

#### 启用 NTP 服务器

1. 打开注册表编辑器

   `WIN + R` 打开运行，输入 `regedit`

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20200528141016.png)

2. 展开注册表`HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\W32Time\Config`

   双击 AnnounceFlags 项，将值改为：5，意思是自身为可靠的时钟源；

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20200528141333.png)

3. 展开注册表 `HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\W32Time\TimeProviders\NtpServer`

   双击 Enabled 项，将值改为：1，意思是启用该服务；

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20200528141518.png)

4. 在管理员 CMD 下重启 w32time 服务

   ```
   net stop w32time
   net start w32time
   ```

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20200528141734.png)

5. 本机验证，有时间回显说明配置成功

   ```
   w32tm /stripchart /computer:127.0.0.1
   ```

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20200528141927.png)

#### 在嵌入式板上测试 NTP 服务器

> 我的 Win IP 为 192.168.2.1，每次重启电脑都需要检查有没有启动 w32time；
>
> 如果失败，就先关闭 Win 上相应网络的防火墙再尝试；

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20200528142724.png)

