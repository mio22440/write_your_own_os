
## 一、简介

学习 WriteYourOwnOperatingSystem 做的实验，完成到内存管理部分，每个课程实验代码都单独做了存档，并排好顺序，使用 VirtualBox 运行即可

> youtube 视频链接 https://youtu.be/1rnA6wpF0o4
>
> bilibili 视频链接     [Write Your Own Operating System in 1 Hour_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1Ng411x7As?p=1)

## 二、构建与运行

step 1 进入要运行的项目目录

```shell
$ cd 1-first_program
```

step 2 使用 make 完成构建

```shell
$ make
```

构建完成后在目录下会生成 mykernel.iso 文件，将该文件放至创建的虚拟机硬盘所在目录即可

step 3 启动虚拟机即可查看效果



## 三、各目录对应的视频内容

1B.上个例子中没有调用构造函数

2.在virtualbox中运行

3.重定义基本类型
*每次传输前记得把vm目录下的启动介质mykernel.iso删除，否则在win中不会自动覆盖，而是重命名一个新的

4.定义全局描述符表GDT数据结构并定义相应操作

5.完成访问硬件外设的port相关，并顺带优化了printf

6.完成中断管理相关，可以注册中断服务函数了

7.获取键盘输入

8.获取鼠标输入

9.驱动抽象

10.整理目录结构，使用命名空间

11.PCI PeripheralComponentInterconnect

12.设备识别？

13.VGA设备，图像

14.图像界面的初步框架(注意，这里添加的几个文件没编译，仅仅添加了而已)

15.桌面和窗口

16.多任务

17.内存管理







