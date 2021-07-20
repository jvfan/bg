---
layout:     post
title:      使用ssh工具连接到WSL（Windows子系统）
subtitle:   系统编程的准备工作 这里我使用的是Windows子系统进程学习
date:       2019-06-23
author:     Jerry Chen
header-img: img/post-bg-coffee.jpeg
catalog: true
tags:
    - 系统编程
    - 准备工作
    - Ubuntu
---

> 在接下来的学习中，我先跳出去先学习系统编程的内容。为此只需要一台有gcc的Linux平台即可，这里我使用的是Windows子系统Ubuntu18.04


- 首先是卸载重装一遍ssh服务，这里不是很确定是不是自带ssh服务有没有问题。

  这一步最好需要。

  ```
  sudo apt-get remove openssh-server
  sudo apt-get install openssh-server
  ```

- 编辑sshd_config文件，修改几处配置才能正常使用用户名/密码的方式连接

  ```
  sudo vim /etc/ssh/sshd_config
  ```

  `Port 22 #默认注释，使用缺省端口22，如果有端口占用可以自己修改`
  
  `PasswordAuthentication yes # 允许用户名密码方式登录`

  `PermitRootLogin yes # 允许root账户登录`

- 修改完之后重启ssh服务

  ```
  sudo service ssh restart
  ```

##### 这样putty和winscp就可以正常使用。

每次开机后（点开WSL），需要**先启动ssh服务**`sudo service ssh start`，才能正常使用。使用windows脚本的方法可以解决，这里不折腾。

