---
layout:     post
title:      制作 ubuntu-base 根文件系统
subtitle:   制作 arm 根文件系统，并用 qemu 仿真
date:       2020-07-12
author:     Jerry Chen
header-img: img/post-bg-coffee.jpeg
catalog: true
tags:
    - LVGL
    - QEMU
---

### 前提

* [QEMU 仿真入门](2020/07/08/QEMU仿真入门/) 

### 开始

> 参考[瑞芯微 Wiki](http://opensource.rock-chips.com/wiki_Distribution) 
>
> 参考[全志 Wiki](http://linux-sunxi.org/Build_Instructions_for_Ubuntu) 

#### 下载 ubuntu-base

下载地址：[ubuntu-base-18.04.5](http://cdimage.ubuntu.com/ubuntu-base/releases/18.04.5/release/) 

选择下载：[ubuntu-base-18.04.5-base-armhf.tar.gz](http://cdimage.ubuntu.com/ubuntu-base/releases/18.04.5/release/ubuntu-base-18.04.5-base-armhf.tar.gz) 

1. 创建工作文件夹

   ```shell
   mkdir ubuntu-base
   ```

2. 下载并解压缩 ubuntu-base-18.04.5-base-armhf.tar.gz

   ```shell
   tar -vxf ubuntu-base-18.04.5-base-armhf.tar.gz -C ubuntu-base
   ```

   解压后的文件夹如下：

   ```shell
   jerry@CLOU-PC ubuntu-base → ls
   bin  boot  dev  etc  home  lib  media  mnt  opt  proc  root  run  sbin  srv  sys  tmp  usr  var
   ```

#### 安装 qemu-user-static

> 这个工具用于挂载 arm 文件系统；

```shell
sudo apt install qemu-user-static
```

#### 准备 chroot 环境

> 1. 在 ubuntu-base 的解压目录执行；
> 2. WSL 下 resolv.conf 中 DNS 配置是变动的，每次挂载请检查需不需要再次覆盖到 arm 文件系统中；

> 以下若目标是 arm64 则拷贝 `qemu-aarch64-static`

```shell
cp /usr/bin/qemu-arm-static usr/bin/

# 拷贝主机的 DNS 设置到 arm 文件系统（为了可以上网）
cp /etc/resolv.conf etc/resolv.conf
```

编辑目标 arm 文件系统的源：

> 如有新的，建议先把 https 替换 http；

```shell
vim etc/apt/sources.list
```

```
# 默认注释了源码镜像以提高 apt update 速度，如有需要可自行取消注释
deb http://mirrors.tuna.tsinghua.edu.cn/ubuntu-ports/ bionic main restricted universe multiverse
# deb-src http://mirrors.tuna.tsinghua.edu.cn/ubuntu-ports/ bionic main restricted universe multiverse
deb http://mirrors.tuna.tsinghua.edu.cn/ubuntu-ports/ bionic-updates main restricted universe multiverse
# deb-src http://mirrors.tuna.tsinghua.edu.cn/ubuntu-ports/ bionic-updates main restricted universe multiverse
deb http://mirrors.tuna.tsinghua.edu.cn/ubuntu-ports/ bionic-backports main restricted universe multiverse
# deb-src http://mirrors.tuna.tsinghua.edu.cn/ubuntu-ports/ bionic-backports main restricted universe multiverse
deb http://mirrors.tuna.tsinghua.edu.cn/ubuntu-ports/ bionic-security main restricted universe multiverse
# deb-src http://mirrors.tuna.tsinghua.edu.cn/ubuntu-ports/ bionic-security main restricted universe multiverse

# 预发布软件源，不建议启用
# deb http://mirrors.tuna.tsinghua.edu.cn/ubuntu-ports/ bionic-proposed main restricted universe multiverse
# deb-src http://mirrors.tuna.tsinghua.edu.cn/ubuntu-ports/ bionic-proposed main restricted universe multiverse
```

#### 挂载 arm 文件系统

> 在 ubuntu-base 的解压目录上级目录执行

编写 ch-mount.sh 用于挂载文件系统：

> 如果需要挂载主机根目录就在 arm 文件系统目录中事先创建 host 目录，且把下面的相关挂载指令取消屏蔽；

```shell
#!/bin/bash

function mnt() {
    echo "MOUNTING"
    sudo mount -t proc /proc ${2}proc
    sudo mount -t sysfs /sys ${2}sys
    sudo mount -o bind /dev ${2}dev
    sudo mount -o bind /dev/pts ${2}dev/pts
    #sudo mount --bind / ${2}host
    sudo chroot ${2} /bin/bash
}

function umnt() {
    echo "UNMOUNTING"
    sudo umount ${2}proc
    sudo umount ${2}sys
    sudo umount ${2}dev/pts
    sudo umount ${2}dev
    #sudo umount ${2}host
}

if [ "$1" == "-m" ] && [ -n "$2" ] ;
then
    mnt $1 $2
elif [ "$1" == "-u" ] && [ -n "$2" ];
then
    umnt $1 $2
else
    echo ""
    echo "Either 1'st, 2'nd or both parameters were missing"
    echo ""
    echo "1'st parameter can be one of these: -m(mount) OR -u(umount)"
    echo "2'nd parameter is the full path of rootfs directory(with trailing '/')"
    echo ""
    echo "For example: ch-mount -m /media/sdcard/"
    echo ""
    echo 1st parameter : ${1}
    echo 2nd parameter : ${2}
fi
```

挂载虚拟文件系统：

> 退出后需要取消挂载 `./ch-mount.sh -u ubuntu-base/`

```
chmod +x ch-mount.sh
sudo ./ch-mount.sh -m ubuntu-base/
```

#### arm 文件系统中的操作

##### 更新源

```shell
chmod 777 /tmp
apt update
```

##### 安装基础的软件包

> 下面的 ssh 安装比较慢，如果只需要 ssh server，可选择安装 `apt-get install openssh-server`

```shell
apt install udev       #如不装，会无法找到文件系统的控制节点 ttyAMA0，卡死在 kernel 调试
apt install sudo       #sudo 命令
apt install vim        #vim 编辑器
apt install ssh        #ssh 的 client 和 server
apt install iputils-ping #ping 和 ping6
apt install net-tools  #ifconfig，netstat，route，arp 等
apt install ifupdown          #ifup，ifdown 等工具
apt install ethtool    #ethtool 命令，显示、修改以太网设置
#apt install wireless-tools    #iwconfig 等工具，显示、修改无线设置
#apt install network-manager   #Network Manager 服务和框架，高级网络管理
apt install rsyslog           #系统 log 服务
apt install bash-completion   #bash 命令行补全
apt install htop              #htop 工具，交互式进程查看器
apt install kmod              #可使用 insmod 和 lsmod
```

对于 bash 的自动补全功能，需要安装 bash-completion，此外还需要修改 `/etc/bash.bashrc`，将下面的脚本注释去掉：

```shell
# enable bash completion in interactive shells
if ! shopt -oq posix; then
   if [ -f /usr/share/bash-completion/bash_completion ]; then
     . /usr/share/bash-completion/bash_completion
   elif [ -f /etc/bash_completion ]; then
     . /etc/bash_completion
   fi
fi
```

##### 添加新用户到 sudo 组

> 下面的命令已经把 jerry 添加到 sudo 组，可使用 sudo 权限；
>
> 如果 jerry 不是 sudo 组成员想用 sudo 权限，可修改 `/etc/sudoers`，增加：`jerry  ALL=(ALL:ALL) ALL`

```shell
useradd -s '/bin/bash' -m -G adm,sudo jerry

# 设置 jerry 用户密码
passwd jerry
# 设置 root 用户密码
passwd root
```

##### 设置 host 相关信息

```shell
# 设置主机名称
echo 'ubuntu-arm' > /etc/hostname

# 设置本机入口 IP：
echo "127.0.0.1 localhost" >> /etc/hosts
echo "127.0.1.1 ubuntu-arm" >> /etc/hosts
```

#### 打包镜像

##### 退出 arm 文件系统

```shell
exit
./ch-mount.sh -u ubuntu-base/
```

##### 使用 dd 指令制作空的固件容器

查看 ubuntu-base 的文件夹大小：

```shell
jerry@CLOU-PC public → sudo du -sh ubuntu-base
410M    ubuntu-base
```

接着制作一个空容器，格式化为 ext3：

> 大小：1024 * 1M

```shell
dd if=/dev/zero of=ubuntu.img bs=1M count=1024
mkfs.ext3 ubuntu.img
```

##### 将文件系统制作到 ubuntu-rootfs.img

```shell
mkdir ubuntu-mount
sudo mount ubuntu.img ubuntu-mount
sudo cp -rfp ubuntu-base/* ubuntu-mount
sudo umount ubuntu-mount

# 检查并修复
e2fsck -p -f ubuntu.img
# 经过调整大小后大小略大于 410M
resize2fs -M ubuntu.img
```

#### QEMU 测试 

```shell
qemu-system-arm \
-nographic \
-M vexpress-a9 \
-m 1024M \
-kernel zImage \
-dtb vexpress-v2p-ca9.dtb \
-sd ubuntu.img \
-append "root=/dev/mmcblk0 rw console=ttyAMA0"
```

