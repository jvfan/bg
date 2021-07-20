---

layout:     post
title:      在 Windows 上安装 Rust(MSVC)
subtitle:   基于 MSVC，即 VS 上的编译器
date:       2020-06-13
author:     Jerry Chen
header-img: img/post-bg-debug.png
catalog: true
tags:
    - rust
---

### 获取软件

前提：安装 VS For C++ 桌面（含有 MSVC），参考[这里](https://jvfan.github.io/2020/06/06/搭建LVGL模拟器环境/#vs2019-离线包的制作) 

[官网下载页面](https://www.rust-lang.org/zh-CN/learn/get-started) 

下载：[rustup-init.exe（64位）](https://static.rust-lang.org/rustup/dist/x86_64-pc-windows-msvc/rustup-init.exe) 

### 进行安装

#### 设置环境变量

一、软件包安装目录

> 设置 CARGO_HOME 和 RUSTUP_HOME 是为了将软件安装到指定路径；

```
CARGO_HOME=D:\Program Files\Rust\cargo
RUSTUP_HOME=D:\Program Files\Rust\rustup
```

二、rustup 国内服务器

> 这里选清华源，可安装 nightly 版本；

```
RUSTUP_UPDATE_ROOT=https://mirrors.tuna.tsinghua.edu.cn/rustup
RUSTUP_DIST_SERVER=https://mirrors.tuna.tsinghua.edu.cn/rustup
```

#### 安装

1. 双击 rustup-init.exe

   核对如下显示信息：

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20200613225453.png)

2. 选择第二项”Customize installation“进行自定义

   > host triple 不用改（MSVC），如果你用 mingw 就改为 `x86_64-pc-windows-gnu`；
   >
   > default toolchain 选择 nightly 日更版本；

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20200613230217.png)

3. 再次检查，无误后进行安装

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20200613230638.png)

4. 出现如下显示就安装成功

   出现提示：Cargo's bin 目录需要添加到环境变量，后面会加；

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20200613232944.png)

5. 确认 rust 以及 rustup 安装成功

   > rustc 是 rust 的编译器，类似 gcc；

   ```
   rustc -V
   ```

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20200613233642.png)

   > rustup 是 rust 的安装和管理工具，类似 apt；

   ```
   rustup --help
   ```

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20200613233554.png)

### 小试牛刀

#### cargo 的设置

1. 在系统环境变量的 path 中添加 `%CARGO_HOME%\bin`

2. 确认 cargo 安装成功

   > cargo 是 rust 的包和项目管理工具，类似 cmake；

   ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20200613234108.png)

3. 给 cargo 设置国内源

   在 CARGO_HOME 目录下建立一个名为 config 的文件

   > 我放在了 `D:\Program Files\Rust\cargo\config`

   ```
   [source.crates-io]
   replace-with = 'tuna'
   
   [source.tuna]
   registry = "https://mirrors.tuna.tsinghua.edu.cn/git/crates.io-index.git"
   ```

#### 使用 rls 之前的配置

> rls 是 rust 适配 VSCode 的调试组件；

强行使用 VSCode 会报错：

error: component 'rls' for target 'x86_64-pc-windows-msvc' is unavailable for download for channel nightly

这是因为当前 rust 版本过高，rls 暂不支持；

这个网站可以查询当前 所有工具链的适配情况：

[https://rust-lang.github.io/rustup-components-history](https://rust-lang.github.io/rustup-components-history/x86_64-pc-windows-msvc.html) 

**解决方法：**

```
# 查看当前已经安装的版本信息
rustup show

# 安装 2020.6.10 的版本
rustup install nightly-2020-06-10 

# 安装成功后进行切换
rustup default nightly-2020-06-10-x86_64-pc-windows-msvc

# [可选] 安装组件，VSCode 插件也会提示安装
rustup component add rls rust-analysis rust-src
```

#### 使用 VSCode

##### 安装 VSCode 的插件

* rust 插件

  有代码提示才是正常的，否则 rust 可能装的有问题；

  ![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20200614004602.png)

* C/C++ 插件：调试代码时使用，后面介绍；

##### 简单编译

编译前，文件夹下只有 main.rs 文件：

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20200614005216.png)

main.rs

```
fn main(){
    print!("Hello, world!");
}
```

使用控制台编译：

> 这样编译不带有调试信息，不能断点调试；

```
rustc main.rs
```

编译后：

生成了 main.exe 目标文件；

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20200614005352.png)

执行 main.exe：

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20200614005451.png)

##### 使用 cargo 创建工程

控制台使用 cargo 创建名为 hello 的工程，并使用 VSCode 打开 hello 工程文件夹；

```
cargo new hello
```

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20200614005741.png)

当前目录下可用：

> 默认编译出来的含有调试信息，可断点调试；

```
# 编译当前工程，生成 target 目录
cargo build
# 运行当前工程二进制目标文件
cargo run
```

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20200614010129.png)

##### cargo + 插件调试

配置默认生成任务：

“终端” --> “配置默认生成任务” --> "cargo build"

```
{
	"version": "2.0.0",
	"tasks": [
		{
			"type": "cargo",
			"subcommand": "build",
			"problemMatcher": [
				"$rustc"
			],
			"group": {
				"kind": "build",
				"isDefault": true
			},
			"label": "Rust: cargo build"
		}
	]
}
```

配置运行和调试：

“创建 launch.json” --> “C++ (Windows)”

```
{
    // 使用 IntelliSense 了解相关属性。 
    // 悬停以查看现有属性的描述。
    // 欲了解更多信息，请访问: https://go.microsoft.com/fwlink/?linkid=830387
    "version": "0.2.0",
    "configurations": [
        {
            "name": "(Windows) 启动",
            "type": "cppvsdbg",
            "request": "launch",
            "program": "${workspaceFolder}/target/debug/hello.exe",
            "args": [],
            "stopAtEntry": false,
            "cwd": "${workspaceFolder}",
            "environment": [],
            "externalConsole": false
        }
    ]
}
```

发现 rust 文件不能打断点，允许任何文件打断点即可：

“文件” --> “首选项” --> “设置” --> “工作区” --> “功能” --> “调试” --> “允许在任何文件中设置断点”；

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20200614011750.png)

以上操作全部完成后就可以：

`ctrl + shift + B` 生成，接着 `F5` 调试；

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20200614012013.png)

