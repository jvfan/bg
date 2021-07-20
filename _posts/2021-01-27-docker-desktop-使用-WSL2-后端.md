---

layout:     post
title:      docker-desktop 使用 WSL2 后端
subtitle:   安装桌面版 docker 并启用 WSL2 支持
date:       2021-01-27
author:     Jerry Chen
header-img: img/post-bg-coffee.jpeg
catalog: true
tags:
    - docker-desktop
    - WSL2
---

> WSL2 作为后端要求 Windows 2004 及以上版本，安装 WSL2 请参考以前的文章；

### 安装 docker-desktop

[docker-desktop 官网](https://www.docker.com/products/docker-desktop) 

1. 下载最新版本并安装，注意勾选启用 WSL 2 支持 （如果电脑已经安装 WSL2，应该是默认勾选的）；

2. 打开 docker-desktop，跳过向导（网络原因，这个会很坑）；

3. Settings -> General 中勾选 “Use WSL 2 based engine”；

4. Settings -> Resources -> WSL Integration 中勾选 “Ubuntu-20.04”

   > 设置默认 WSL 发行版命令：`wsl --set-default ubuntu-20.04`

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20210127122045.png)

5. Settings -> Docker Engine 中配置修改 "registry-mirrors" 镜像仓库地址

   > 阿里云专属镜像加速地址 [申请点这](https://cr.console.aliyun.com/cn-hangzhou/instances/mirrors)
   
   - 科大镜像：https://docker.mirrors.ustc.edu.cn
   - 网易：https://hub-mirror.c.163.com
   - 阿里云：https://__your_id__.mirror.aliyuncs.com
   - 七牛云加速器：https://reg-mirror.qiniu.com

   ```json
   {
     "registry-mirrors": [
       "https://docker.mirrors.ustc.edu.cn",
       "https://hub-mirror.c.163.com",
       "https://reg-mirror.qiniu.com"
     ],
     "insecure-registries": [],
     "debug": false,
     "experimental": false,
     "features": {
       "buildkit": true
     }
   }
   ```

6. 在 WSL2 调用 docker 命令创建 app 能在桌面 GUI 程序中看到信息以及控制就成功了；

   > 需要运行一个 docker 实例；

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20210127123017.png)

### 迁移 docker 的 WSL2 后端存储位置

启用 docker-desktop 的 WSL2 支持后，会由 docker-desktop 创建两个 WSL2 的发行版：

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20210127134124.png)

这些发行版默认存储路径都是 C 盘，可以将它们迁移到非系统盘；

迁移方法：

> 只需要迁移 docker-desktop-data 即可，另一个发行版（docker-dekstop）占用空间很小，且更新 Docker-Desktop.exe 后会恢复该发行版到其原始位置；

1. 首先完全退出 docker-desktop

2. 使用 powershell 关闭所有发行版：

   ```powershell
   wsl --shutdown
   ```

3. 将发行版 docker-desktop-data 导出到 `D:\WSL\docker-desktop-data.tar`

   > docker images 和 apps 存储在该镜像中，不会丢失；

   ```powershell
   wsl --export docker-desktop-data D:\WSL\docker-desktop-data.tar
   ```

4. 注销 docker-desktop-data：

   ```powershell
   wsl --unregister docker-desktop-data
   ```

5. 从 `D:\WSL\docker-desktop-data.tar` 导入发行版 docker-desktop-data 到文件夹 `D:\WSL\docker-desktop-data` ：

   ```powershell
   wsl --import docker-desktop-data D:\WSL\docker-desktop-data D:\WSL\docker-desktop-data.tar --version 2
   ```

6. [可选] 删除中间导出文件

   ```powershell
   rm D:\WSL\docker-desktop-data.tar
   ```

### 外部主机访问 WSL2

#### 配置 portproxy

> 请先运行服务再配置 portproxy，否则运行服务时会提示端口占用；

* 命令行方式：

  powershell 管理员执行：

  > 假设 WSL2 开启了 8080 端口（本机可通过 127.0.0.1:8080 访问服务），输入如下指令后外部主机可以通过 {your-ip}:8080 访问服务；

  ```powershell
  netsh interface portproxy add v4tov4 listenport=8080 connectaddress=127.0.0.1 listenaddress=* protocol=tcp
  ```

* Gui 方式：

  下载 [PortProxyGUI](https://github.com/zmjack/PortProxyGUI)，Github 上提供 .netcore 和 .net3.5 的版本，建议用 VS2019 编译一个 .net4.5.2 的版本；
  
  运行后，右键新建一条规则：
  
  ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20210201120037.png)
  

#### 允许端口入站

* 方式一，关闭防火墙：

  关闭 {your-ip} 对应网卡的防火墙（需要看你的网络是属于专用还是公用网络）：

  > 控制面板 --> 系统和安全 --> Windows Defender 防火墙

  ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20210201120336.png)

* 方式二，新建入站规则：

  1. 启用防火墙（控制面板 --> 系统和安全 --> Windows Defender 防火墙）；

  2. 进入防火墙高级设置：

     ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20210201121642.png)

  3. 新建入站规则

     在“入站规则”上右击新建规则：

     ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20210201122016.png)

     选择端口规则：

     ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20210201122215.png)

     输入希望从外访问的 TCP 8080 端口：

     ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20210201122430.png)

     选择允许连接：

     ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20210201122614.png)

     何时应用规则？默认即可：

     ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20210201122656.png)

     最后做个简单描述，点击完成即可外部访问 8080 端口：

     ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20210201122957.png)

