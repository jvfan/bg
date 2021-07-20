---
layout:     post
title:      建立Gitee图床配合PicGo使用
subtitle:   Gitee 作为图床，PicGo 负责上传
date:       2020-05-16
author:     Jerry Chen
header-img: img/post-bg-coffee.jpeg
catalog: true
tags:
    - 图床
    - Gitee
    - PicGo
---

### 准备工具

* [PicGo 安装包](https://github.com/Molunerfinn/PicGo/releases) 
* [Gitee](https://gitee.com/)：Your-Image-Hosting

### 开始

#### 安装 PicGo 工具

1. 下载安装 PicGo

   [GitHub 下载](https://github.com/Molunerfinn/PicGo/releases) 

2. 打开 PicGo 安装  gitee 插件

   * 首先安装 [node.js](https://nodejs.org/en/) 

     > 需要安装在默认位置，也就是 C 盘的 XXX 下，建议一路next（不用管非必要复选框）；

   * 搜索“gitee”，安装  `gitee(zhanghuid)` 插件

     ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20200516150333.png)


#### Gitee 上新建图床仓库

1. 填入仓库名称

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20200516150705.png)

2. **选择公开仓库**，**选择使用 Readme 文件初始化仓库**，其他默认

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20200516150751.png)

3. 创建好的仓库如下

   你可以更改下 README.md 中的内容，也可以删除英文版的说明。

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20200516151247.png)

4. 创建一个“私人令牌”

   `设置` --> `安全设置` --> `私人令牌` --> `新建`

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20200516151623.png)

   成功后会生成一个令牌码，这个码只在创建的时候告诉你一次，保存它，后面需要填入到 PicGo 的 Gitee 插件里。

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20200516152057.png)

#### 配置 Gitee 插件

1. 找到 PicGo “图床设置” 中的 “Gitee 图床”，进行设置

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20200516152528.png)

2. 重新启动一些功能性开关（先关再打开）

   > 有些功能必须这样操作下才生效，可能是bug，比如“上传后自动复制URL”功能

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20200516152945.png)

### 配合 Typora 愉快使用

#### 配置

1. 检查 PicGo 设置，开启 PicGo-Server

   > 注意监听端口需要是默认的 36677

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/image-20200516153510136.png)

2. 对 Typora 进行配置

   `文件` --> `偏好设置...` --> `图形`

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/Snipaste_2020-05-16_15-42-40.png)

   

#### 使用方法

1. 本机图片拖入到编辑器，自动上传且图片地址自动变更

2. 剪贴板图片 `Ctrl + V` 进行粘贴，这样需要右键手动上传图片

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/image-20200516154926782.png)

