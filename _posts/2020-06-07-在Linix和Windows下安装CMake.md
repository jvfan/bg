---

layout:     post
title:      Linux 和 Windows 上安装 CMake
subtitle:   CMake 的安装和简单使用
date:       2020-06-07
author:     Jerry Chen
header-img: img/post-bg-debug.png
catalog: true
tags:
    - CMake
---

### 获取软件

[官网下载](https://cmake.org/download/) 

该页面有源代码和编译好的二进制文件，我们均下载二进制版本；

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20200607233908.png)

### Linux 上安装

> 也可使用 `sudo apt-get install cmake` 的方式安装，如果版本大于 3.10 就可用，否则就需要从官网下载最新版本；

#### 卸载旧版本

如果系统有预装 CMake 就先卸载它；

```
sudo apt-get remove cmake
```

#### 下载 CMake 并解压

> /opt 目录存放可选的程序，比如 firefox 测试版安装到 /opt/firefox_beta；/usr 目录让软件包管理工具（apt）管理；/usr/local 目录存放手动安装的软件，也可放自定义的脚本；

```
sudo mkdir -p /opt
cd /opt

# 下载
sudo wget https://github.com/Kitware/CMake/releases/download/v3.17.3/cmake-3.17.3-Linux-x86_64.tar.gz
# 解压
sudo tar -vxf cmake-3.17.3-Linux-x86_64.tar.gz
```

解压完成后如图：

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20200608002311.png)

#### 创建软链接

```
sudo mv /opt/cmake-3.17.3-Linux-x86_64 /opt/cmake-3.17.3
sudo ln -sf /opt/cmake-3.17.3/bin/* /usr/bin/
```

完成后查看版本：

```
cmake --version
```

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20200608002708.png)

如果你的系统支持 GUI，你可以输入 `cmake-gui` 进行启动；

### Windows 上安装

> 安装前也要卸载以前装的旧版本；

#### 下载安装包

[点这下载 cmake-3.17.3-win64-x64.msi](https://github.com/Kitware/CMake/releases/download/v3.17.3/cmake-3.17.3-win64-x64.msi) 

#### 进行安装

选择添加环境变量：

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20200608003212.png)

选择安装路径：

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20200608003358.png)

打开这样的 GUI 界面就安装成功了；

### CMake 简单使用

#### 单个 main.c

##### 编写文件

项目结构：

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20200608115922.png)

编写：main.cpp

```
#include <iostream>
using namespace std;

int main(int argc, char *argv[]){
    cout << "hello cmake!" << endl;
    return 0;
}
```

编写：CMakeLists.txt

> project 设置工程名；
>
> add_executable 设置目标二进制文件，格式：`二进制名 cxx文件1 [cxx文件2] [...]` 

```
cmake_minimum_required(VERSION 3.10)

# set the project name
project(CMakeDemo)

# add the executable
add_executable(main main.cpp)
```

##### 使用 cmake

> 使用默认编译工具链 gcc 和 g++，后面会提到如何指定编译器；

建立 build 文件夹，并使用 cmake 生成 Makefile：

```
mkdir build
cd build
cmake ..
```

##### 进行 make

没有报错就可继续编译：

```
make
```

编译后 build 文件夹下会生成二进制文件 main，执行它试试；

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20200608121801.png)

#### 单文件夹多文件

##### 编写文件

项目结构：

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/Snipaste_2020-06-08_13-37-32.png)

编写 main.cpp：

```
#include "simple_print.h"

int main(int argc, char *argv[]){
    simple_print("hello cmake!");
    return 0;
}
```

编写 simple_print.cpp：

```
#include "simple_print.h"

void simple_print(string sStr){
    cout << sStr << endl;
}
```

编写 simple_print.h：

```
#ifndef __SIMPLE_PRINT_H_
#define __SIMPLE_PRINT_H_

#include <iostream>
#include <string>
using namespace std;

void simple_print(string sStr);

#endif //__SIMPLE_PRINT_H_
```

编写：CMakeLists.txt

> project 设置工程名，执行时会生成一个变量 PROJECT_NAME；
>
> add_executable 设置目标二进制文件，格式：`二进制名 cxx文件1 [cxx文件2] [...]` 

```
cmake_minimum_required(VERSION 3.10)

# set the project name
project(CMakeDemo)

# add the executable
add_executable(main main.cpp simple_print.cpp)
```

##### 使用 cmake

> 使用默认编译工具链 gcc 和 g++，后面会提到如何指定编译器；

建立 build 文件夹，并使用 cmake 生成 Makefile：

```
mkdir build
cd build
cmake ..
```

##### 进行 make

没有报错就可继续编译：

```
make
```

编译后 build 文件夹下会生成二进制文件 main，执行它试试；

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/Snipaste_2020-06-08_13-42-27.png)

#### 单文件夹多文件2

##### 编写文件

项目结构：

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/Snipaste_2020-06-08_15-27-02.png)

编写 main.cpp：

```
#include "simple_print.h"
#include "simple_print2.h"

int main(int argc, char *argv[]){
    simple_print("hello cmake!");
    return 0;
}
```

编写 simple_print.cpp：

```
#include "simple_print.h"

void simple_print(string sStr){
    cout << sStr << endl;
}
```

编写 simple_print.h：

```
#ifndef __SIMPLE_PRINT_H_
#define __SIMPLE_PRINT_H_

#include <iostream>
#include <string>
using namespace std;

void simple_print(string sStr);

#endif //__SIMPLE_PRINT_H_
```

编写 simple_print2.cpp：

```
#include "simple_print2.h"

void simple_print2(void){
    cout << "test test test..." << endl;
}
```

编写 simple_print2.h：

```
#ifndef __SIMPLE_PRINT2_H_
#define __SIMPLE_PRINT2_H_

#include <iostream>
using namespace std;

void simple_print2(void);

#endif //__SIMPLE_PRINT2_H_
```

编写：CMakeLists.txt

> add_library 可选链接库类型：STATIC，SHARED 或 MODULE；

```
# 设置 CMake 最低可用版本
cmake_minimum_required(VERSION 3.10)

# 设置工程名称和其他属性
project(CMakeDemo   VERSION 1.0
                    DESCRIPTION "我的 CMake Demo"
                    LANGUAGES CXX)

# 生成动态链接库 simple_print.so
add_library(simple_print SHARED simple_print.cpp simple_print.h)
# 指定标准为 c++ 11
target_compile_features(simple_print PUBLIC cxx_std_11)

# 生成动态链接库 simple_print2.so
add_library(simple_print2 SHARED simple_print2.cpp simple_print2.h)
# 指定标准为 c++ 11
target_compile_features(simple_print2 PUBLIC cxx_std_11)

# 生成可执行文件
add_executable(main main.cpp)
# 添加依赖
target_link_libraries(main simple_print simple_print2)
```

##### 使用 cmake

> 使用默认编译工具链 gcc 和 g++，后面会提到如何指定编译器；

建立 build 文件夹，并使用 cmake 生成 Makefile：

```
mkdir build
cd build
cmake ..
```

##### 进行 make

没有报错就可继续编译：

```
make
```

编译后 build 文件夹下会生成 `libsimple_print.a`、`libsimple_print2.a` 和二进制文件 main，执行它试试；

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/Snipaste_2020-06-08_15-35-29.png)

#### 多文件夹多文件

##### 编写文件

项目结构：

> app_version.h 是由 app_version.h.in 进行 cmake 后生成的；

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/Snipaste_2020-06-08_16-29-02.png)

编写 main.cpp：

```
#include "app_version.h"
#include "simple_print.h"
#include "simple_print2.h"

int main(int argc, char *argv[]){
    simple_print("hello cmake!");
    simple_print("project version is " MY_VERSION);
    simple_print2();
    return 0;
}
```

编写 simple_print.cpp：

```
#include "simple_print.h"

void simple_print(string sStr){
    cout << sStr << endl;
}
```

编写 simple_print.h：

```
#ifndef __SIMPLE_PRINT_H_
#define __SIMPLE_PRINT_H_

#include <iostream>
#include <string>
using namespace std;

void simple_print(string sStr);

#endif //__SIMPLE_PRINT_H_
```

编写 simple_print2.cpp：

```
#include "simple_print2.h"

void simple_print2(void){
    cout << "test test test..." << endl;
}
```

编写 simple_print2.h：

```
#ifndef __SIMPLE_PRINT2_H_
#define __SIMPLE_PRINT2_H_

#include <iostream>
using namespace std;

void simple_print2(void);

#endif //__SIMPLE_PRINT2_H_
```

编写 app_version.h.in

```
#ifndef __APP_VERSION_H_
#define __APP_VERSION_H_

#define MY_VERSION "@PROJECT_VERSION@"

#endif //__APP_VERSION_H_
```

编写：CMakeLists.txt

```
# 设置 CMake 最低可用版本
cmake_minimum_required(VERSION 3.10)

# 设置工程名称和其他属性
project(CMakeDemo   VERSION 1.0
                    DESCRIPTION "我的 CMake Demo"
                    LANGUAGES CXX)

# 由 app_version.h.in 生成 app_version.h
configure_file (
    "${PROJECT_SOURCE_DIR}/version/app_version.h.in"
    "${PROJECT_SOURCE_DIR}/version/app_version.h"
)

# 生成静态链接库 simple_print.a
add_library(simple_print STATIC
            simple_print/simple_print.cpp
            simple_print/simple_print.h)
# 指定头文件夹
target_include_directories(simple_print PUBLIC simple_print)
# 指定标准为 c++ 11
target_compile_features(simple_print PUBLIC cxx_std_11)

# 生成静态链接库 simple_print2.a
add_library(simple_print2 STATIC
            simple_print/simple_print2.cpp
            simple_print/simple_print2.h)
# 指定头文件夹
target_include_directories(simple_print2 PUBLIC simple_print2)
# 指定标准为 c++ 11
target_compile_features(simple_print2 PUBLIC cxx_std_11)

# 生成可执行文件
add_executable(main main/main.cpp)
target_include_directories(main PUBLIC version)
# 添加依赖
target_link_libraries(main simple_print simple_print2)
```

##### 使用 cmake

> 使用默认编译工具链 gcc 和 g++，后面会提到如何指定编译器；

建立 build 文件夹，并使用 cmake 生成 Makefile：

```
mkdir build
cd build
cmake ..
```

##### 进行 make

没有报错就可继续编译：

```
make
```

编译后 build 文件夹下会生成 `libsimple_print.a`、`libsimple_print2.a` 和二进制文件 main，执行它试试；

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/Snipaste_2020-06-08_16-35-43.png)

### CMake 常用指令和模板

##### 模式

debug 模式（默认），会包含调试信息，且不做任何优化：

```
mkdir Debug
cd Debug 
cmake -DCMAKE_BUILD_TYPE=Debug .. 
make
```

release 模式，进行优化：

```
mkdir Release
cd Release 
cmake -DCMAKE_BUILD_TYPE=Release .. 
make
```

> 一般工程里只放一个 CMakeList；当然，每个模块作为单独项目也有一个 CMakeList，可通过总工程用 add_subdirectory(your_subdir_target) 包含模块；
>
> 如果编写的是工程就把head和src的文件遍历写入 add_execueable 、add_library，设置include_directory等，基本就可以生成了。
>
> 如果编的是个库，那可以新建 test 目录下再写一个 cmakelist，用 taget_library_link 和 add_directory（子模块的 target）就可以。

##### 模板一

> 注意，这个 CMakeList 进行 make 可能报错，仅供参考；

```
# 指定最小 CMake 版本
cmake_minimum_required(VERSION 3.10)

# 项目名称
project(lvgl)

# 设置变量 C 标准为 11
set(CMAKE_C_STANDARD 11)#C11

# 设置变量 C++ 标准为 17
set(CMAKE_CXX_STANDARD 17)#C17

# 设置变量 打开 C++ 依赖
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 全局包含目录，这里包含程序目录
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})

# 查找满足规则的文件名放到 INCLUDES 变量
file(GLOB_RECURSE INCLUDES "lv_drivers/*.h" "lv_examples/*.h"  "lvgl/*.h"  "./*.h" )

# 查找满足规则的文件名放到 SOURCES 变量
file(GLOB_RECURSE SOURCES  "lv_drivers/*.c" "lv_examples/*.c"  "lvgl/*.c" )

# 设置二进制文件路径
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin) 

# 找库，命名靶为 SDL2
find_package(SDL2 REQUIRED SDL2)

# 全局包含文件夹，这个变量 SDL2_INCLUDE_DIRS 可能是找到的库中声明好的
include_directories(${SDL2_INCLUDE_DIRS})

# 生成二进制文件
add_executable(main main.c mouse_cursor_icon.c ${SOURCES} ${INCLUDES})

# 给靶 main 链接 SDL2 库
target_link_libraries(main PRIVATE SDL2 )

# 增加一个没有输出的目标 run
# 这样生成 Makefile 后，输入 make run 就可以执行 ${EXECUTABLE_OUTPUT_PATH}/main
add_custom_target (run COMMAND ${EXECUTABLE_OUTPUT_PATH}/main)
```

##### 模板二

```
# 指定 CMake 最小版本
cmake_minimum_required(VERSION 3.6)

# 项目名称
project(BoostCoroutineDemo)

# c++标准
set(CMAKE_CXX_STANDARD 11)

# 指定生成的版本
set(CMAKE_BUILD_TYPE "DEBUG")

# 指定编译选项
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall")

# 设置变量指定源代码
set(SOURCE_FILES main.cpp)

# 指定全局头文件目录
include_directories("/usr/local/boost-1.57/include/")

# 指定全局静态和动态文件目录
link_directories("/usr/local/boost-1.57/lib")

# 生成目标文件
add_executable(BoostCoroutineDemo ${SOURCE_FILES})

# 链接库文件 
target_link_libraries(BoostCoroutineDemo libboost_system.a libboost_thread.a)
target_link_libraries(BoostCoroutineDemo pthread)
```

##### 模板三

> 从路径找到文件名（不含后缀）的 CMakeLists.txt；

```
# 递归找到所有 *.xxx 并赋值到变量 SRC_FILES
FILE(GLOB_RECURSE SRC_FILES "*.c" "*.cc" "*.cpp" "*.h" "*.hpp")
# 遍历 SRC_FILES 的每项赋值到 FILE_PATH
FOREACH(FILE_PATH ${SRC_FILES})
    MESSAGE(${FILE_PATH})
    # 正则匹配到文件名
    STRING(REGEX REPLACE ".+/(.+)\\..*" "\\1" FILE_NAME ${FILE_PATH})
    MESSAGE(${FILE_NAME})
ENDFOREACH(FILE_PATH)
```

##### 模板四

> 遍历所有子目录并添加到全局包含目录的函数 include_sub_directories_recursively，其参数 root_dir 需要输入完整路径；

以下是 “多文件夹多文件”的另一个 CMakeLIst 写法：

```
# 设置 CMake 最低可用版本
cmake_minimum_required(VERSION 3.10)

# 设置工程名称和其他属性
project(CMakeDemo   VERSION 1.0
                    DESCRIPTION "我的 CMake Demo"
                    LANGUAGES CXX)

function(include_sub_directories_recursively root_dir)
    if (IS_DIRECTORY ${root_dir})               # 当前路径是一个目录吗，是的话就加入到包含目录
        #        if (${root_dir} MATCHES "include")
        message("include dir: " ${root_dir})
        include_directories(${root_dir})
        #        endif()
    endif()

    file(GLOB ALL_SUB RELATIVE ${root_dir} ${root_dir}/*) # 获得当前目录下的所有文件，让如ALL_SUB列表中
    foreach(sub ${ALL_SUB})
        if (IS_DIRECTORY ${root_dir}/${sub})
            include_sub_directories_recursively(${root_dir}/${sub}) # 对子目录递归调用，包含
        endif()
    endforeach()
endfunction()

# 由 app_version.h.in 生成 app_version.h
configure_file (
    "${PROJECT_SOURCE_DIR}/version/app_version.h.in"
    "${PROJECT_SOURCE_DIR}/version/app_version.h"
)

# 设置变量 C 标准为 99
set(CMAKE_C_STANDARD 99)
# 设置变量 C++ 标准为 11
set(CMAKE_CXX_STANDARD 11)
# 设置变量 打开 C++ 依赖
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 全局包含目录，这里包含程序目录
include_directories(${PROJECT_SOURCE_DIR})
include_sub_directories_recursively(${PROJECT_SOURCE_DIR}/simple_print)
include_sub_directories_recursively(${PROJECT_SOURCE_DIR}/version)

# 查找满足规则的文件名放到 SOURCES 变量
file(GLOB_RECURSE SOURCES "simple_print/*.cpp")

# 设置二进制文件路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin) 

# 生成可执行文件
add_executable(main main/main.cpp ${SOURCES})

# 增加一个没有输出的目标 run
# 这样生成 Makefile 后，输入 make run 就可以执行 ${EXECUTABLE_OUTPUT_PATH}/main
add_custom_target (run COMMAND ${EXECUTABLE_OUTPUT_PATH}/main)
```

##### 模板五

```
cmake_minimum_required(VERSION 3.5)
                           
project(main)         
#MESSAGE(FATAL_ERROR "${CMAKE_BUILD_TYPE}")

if(CMAKE_COMPILER_IS_GNUCC)
    message("COMPILER IS GNUCC")    
    ADD_DEFINITIONS ( -std=c++11 )  
endif(CMAKE_COMPILER_IS_GNUCC)

# 1. protobuf
# 1.1. Find required protobuf package
find_package(Protobuf REQUIRED)
if(PROTOBUF_FOUND)
    message(STATUS "protobuf library found")
else()
    message(FATAL_ERROR "protobuf library is needed but cant be found")
endif()
# 1.2. 生成pb.h、pb.cc必须要加的指令    
include_directories(${PROTOBUF_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS ${CMAKE_SOURCE_DIR}/include/protobuf/proto/config.proto)
# 1.3. head file path，头文件目录
message(${PROTO_HDRS})
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include/protobuf)

# 2. CUDA
set(CUDA_DIR "/usr/local/cuda")
find_package(CUDA)
SET(CUDA_NVCC_FLAGS --disable-warnings;-std=c++11;-O3;-gencode arch=compute_75,code=sm_75)
#CUDA_ADD_LIBRARY(${PROJECT_NAME} SHARED)
message(${CUDA_NVCC_FLAGS})

# 3. opencv
#set(OpenCV_DIR "/data/wuh/open_source/opencv-3.4.6/build")
#find_package(OpenCV REQUIRED)
#include_directories(${OpenCV_INCLUDE_DIRS})
link_directories("/usr/local/lib")
include_directories("/usr/local/include/opencv4")
link_directories("")

#message(${SOURCE_FILES})

# 4. tensorRT
include_directories(/data/wuh/software/TensorRT-5.1.2.2/include)
link_directories(/data/wuh/software/TensorRT-5.1.2.2/lib)
link_directories(/data/wuh/project/test_algorithm_module/algorithm_module/build/)
LINK_LIBRARIES(algorithm)


# 5. 头文件
# 5.1. 定义函数，用于递归添加头文件
function(include_sub_directories_recursively root_dir)
    if (IS_DIRECTORY ${root_dir})               # 当前路径是一个目录吗，是的话就加入到包含目录
        message("include dir: " ${root_dir})
        include_directories(${root_dir})
    endif()

    file(GLOB ALL_SUB RELATIVE ${root_dir} ${root_dir}/*) # 获得当前目录下的所有文件，让如ALL_SUB列表中
    foreach(sub ${ALL_SUB})
        if (IS_DIRECTORY ${root_dir}/${sub})
            include_sub_directories_recursively(${root_dir}/${sub}) # 对子目录递归调用，包含
        endif()
    endforeach()
endfunction()
# 5.2. 添加头文件
include_sub_directories_recursively(${CMAKE_SOURCE_DIR}/include) # 对子目录递归调用，包含
MESSAGE(STATUS "CMAK_SOURCE_DIR" ${CMAKE_SOURCE_DIR})

# 6. 添加源文件
FILE(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/*.cu  ${CMAKE_SOURCE_DIR}/src/*.cpp)
#message(${SOURCE_FILES})

# 7. 添加链接库
LINK_LIBRARIES(opencv_ml opencv_objdetect opencv_imgproc opencv_core opencv_highgui opencv_imgcodecs opencv_shape opencv_videoio opencv_video)
LINK_LIBRARIES(nvcaffe_parser nvinfer nvinfer_plugin nvparsers)
LINK_LIBRARIES(avcodec avformat swscale avutil)
LINK_LIBRARIES(cuda nvcuvid)
#LINK_LIBRARIES(algorithm)
LINK_LIBRARIES(glog)

# 8.source directory，源文件目录
AUX_SOURCE_DIRECTORY(test DIR_SRCS)

# 9. 设置环境变量，编译用到的源文件全部都要放到这里，否则编译能够通过，
#但是执行的时候会出现各种问题，比如"symbol lookup error xxxxx , undefined symbol"
SET(ALL_SRCS ${DIR_SRCS} ${PROTO_SRCS} ${SOURCE_FILES} ${M_INCLUDE_FILES})

# 10.add executable file，添加要编译的可执行文件
# ADD_EXECUTABLE(${PROJECT_NAME} ${ALL_SRCS})
CUDA_ADD_EXECUTABLE(${PROJECT_NAME} ${ALL_SRCS})

# 11. 链接目标文件与库文件，添加可执行文件所需要的库，比如我们用到了libm.so（命名规则：lib+name+.so），就添加该库的名称
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${PROTOBUF_LIBRARIES})
target_link_libraries(${PROJECT_NAME} /usr/local/cuda-10.1/lib64/libcudart.so)
target_link_libraries(${PROJECT_NAME} /usr/local/cuda-10.1/lib64/libcudnn.so)
```

##### 模板五

```
cmake_minimum_required (VERSION 3.10)

INCLUDE_DIRECTORIES(../../thirdparty/comm)

FIND_LIBRARY(COMM_LIB comm ../../thirdparty/comm/lib NO_DEFAULT_PATH)
FIND_LIBRARY(RUNTIME_LIB rt /usr/lib  /usr/local/lib NO_DEFAULT_PATH)

link_libraries(${COMM_LIB} ${RUNTIME_LIB})

ADD_DEFINITIONS(
-O3 -g -W -Wall
 -Wunused-variable -Wunused-parameter -Wunused-function -Wunused
 -Wno-deprecated -Woverloaded-virtual -Wwrite-strings
 -D__WUR= -D_REENTRANT -D_FILE_OFFSET_BITS=64 -DTIXML_USE_STL
)

add_library(lib_demo
        cmd.cpp
        global.cpp
        md5.cpp
)

link_libraries(lib_demo)
add_executable(demo
        main.cpp
)

# link library in static mode
target_link_libraries(demo libuuid.a)
```

