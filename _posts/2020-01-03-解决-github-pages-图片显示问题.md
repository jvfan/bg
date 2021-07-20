---

layout:     post
title:      解决 github pages 图片显示问题
subtitle:   通过 hosts 进行重定向
date:       2020-01-03
author:     Jerry Chen
header-img: img/post-bg-debug.png
catalog: true
tags:
    - other
---

> 推荐使用阿里云公共DNS


### 方法1：重定向

1. 获取元素IP地址

   打开[IPAddress.com](https://www.ipaddress.com/)，在搜索框输入它的域名，搜索元素所在的二级域名，比如`raw.githubusercontent.com`

2. 将搜索到的结果添加到hosts（`C:\Windows\System32\drivers\etc\hosts`）

   我这里是：`185.199.108.133 raw.githubusercontent.com`

#### 我的重定向表

以下是我的重定向表，请根据自己搜索的结果更新此表。

```
# GitHub Start 
140.82.114.3    github.com
185.199.108.133 raw.github.com
185.199.108.133 user-images.githubusercontent.com
185.199.108.133 raw.githubusercontent.com
185.199.108.133 avatars0.githubusercontent.com
185.199.108.133 avatars1.githubusercontent.com
185.199.108.133 avatars2.githubusercontent.com
185.199.108.133 avatars3.githubusercontent.com
185.199.108.133 avatars4.githubusercontent.com
185.199.108.133 avatars5.githubusercontent.com
185.199.108.133 avatars6.githubusercontent.com
185.199.108.133 avatars7.githubusercontent.com
185.199.108.133 avatars8.githubusercontent.com
# GitHub End
 
# my github-pages start
185.199.108.153	loee.xyz
# my github-pages end
```

### 方法2：阿里云DNS

IPv4：`223.5.5.5`和`223.6.6.6`

IPv6：`2400:3200::1`和`2400:3200:baba::1`

自行设置或者见[官方帮助页面](http://www.alidns.com/setup/)。

