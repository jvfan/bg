
系统文件的烧写参见这篇文章：[如何烧写到iTop4412](http://jvfan.github.io/2019/06/18/3_掌握必要的烧写系统的技能/)


以下Web文件是一套适配好的最小linux系统，适合2G内存SCP的iTop4412：

| 文件                 | 类型       | 注释                               | Web                                                          |
| -------------------- | ---------- | ---------------------------------- | ------------------------------------------------------------ |
| u-boot-iTOP-4412.bin | bootloader | uboot                              | [下载](https://github.com/jvfan/jvfan.github.io/raw/master/res/systemfiles/u-boot-iTOP-4412.bin) |
| zImage               | kernel     | 内核，包括驱动                     | [下载](https://github.com/jvfan/jvfan.github.io/raw/master/res/systemfiles/zImage) |
| ramdisk-uboot.img    | ramdisk    | 虚拟内存盘，放文件系统init相关脚本 | [下载](https://github.com/jvfan/jvfan.github.io/raw/master/res/systemfiles/ramdisk-uboot.img) |
| system.img           | system     | 文件系统，大小限制这里放最小linux  | [下载](https://github.com/jvfan/jvfan.github.io/raw/master/res/systemfiles/system.img) |

