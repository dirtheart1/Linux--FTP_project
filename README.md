# Linux--FTP_project
# 项目名称

项目名称  -  FTP文件传输服务器

## 项目简介

该项目是一个FTP（文件传输协议）服务器，允许用户通过网络连接并在服务器和客户端之间传输文件。项目旨在实现高效的文件传输，同时提供基本的文件管理功能。

## 目录结构

```
/project
	ReadMe.md
    /inc
        server.h
        cmd.h
        msg.h
        pkg.h
        client.h
    /lib
    /src
        server.c
        cmd.c
        msg.c
        pkg.c
        client.c
        Makefile
        *.o
    应用层协议.doc

```

### 项目子模块介绍

#### 1. inc 目录

- **server.h**: 服务器核心模块的头文件，包含服务器初始化和启动函数的声明。
- **cmd.h**: 命令处理模块的头文件，定义了命令处理函数的接口。
- **msg.h**: 消息处理模块的头文件，声明了处理消息的函数和数据结构。
- **pkg.h**: 数据封装模块的头文件，包含数据打包和解包的函数声明。
- **client.h**: 客户端模块的头文件，定义了与客户端交互的函数接口。

#### 2. lib 目录

（如果有的话，描述在 lib 目录下的内容）

#### 3. src 目录

- **server.c**: 服务器核心模块的源代码，实现了服务器初始化、启动和监听功能 ；连接后应对各种命令进行反应。
- **cmd.c**: 命令处理模块的源代码，包含处理各种命令的实现。      添加了多客户端并发支持，线程执行函数
- **msg.c**: 消息处理模块的源代码，处理从客户端接收的各种消息。  还有接收服务器收到命令后发送的回复数据包以及相关处理。     以及针对命令封装数据包与发送函数。
- **pkg.c**: 数据封装模块的源代码，实现数据的打包和解包功能。
- **client.c**: 客户端模块的源代码，实现了与客户端交互的功能。
- **Makefile**: 项目的编译配置文件，定义了如何编译和链接源代码。
- **\*.o**: 编译生成的目标文件。

### 通信协议

详情见“应用层协议.doc"

![](D:\ubuntu-22.04\share\二阶段\3.网络编程\20230818_项目\My_FTP\屏幕截图 2023-08-22 145258.png)

## 如何运行

1. 编译项目：

   Linux系统下在项目根目录运行 `make` FTPsever 与make FTPclient 命令，将会分别生成可执行文件。

2. 执行程序：分别运行生成的可执行文件，启动服务器：运行生成的可执行文件，启动服务器程序。 

3. 连接客户端：使用客户端与服务器建立连接。

4.  执行命令：通过客户端发送命令，与服务器进行交互。         输入help查看命令说明

## 项目依赖

- 本项目依赖于某些头文件，这些头文件位于 `/inc` 目录。
- 使用了线程池和队列，需要确保系统支持 POSIX 线程库。



## 历史记录

- 项目启动日：2023年8月19日
- 对通信协议数据包进行了详细的分析与设计，将数据包建立都统一设计为一个指定命令的函数，以方便执行与其他功能的调用。     建立了两种数据包的解包函数。       认识到了无符号字符串转变有符号要考虑的问题。
- 增加FTP消息结构体 及 消息处理函数，对收发信息进行进一步的处理。   同时添加一些新的消息类型枚举并入命令号CMD_NO
- 对原本的下载  上传相关命令的执行函数 进行修改，变为send_file 与recv_file 两个通用性函数，以减少上传下载命令相关设计的复杂性。
- 初步完成了要求的基本功能     ls      下载      上传      退出连接
- 添加了切换本地与服务器工作目录  以及  帮助菜单  功能
- 测试并完善了各种操作的错误排查。
- 原为单线程服务器，现已添加支持多客户端的线程并发。 可多个客户同时登录服务器进行操作。



可增（待增模块）：

1. **用户认证与安全性增强**：添加用户身份验证功能，确保只有经过授权的用户可以访问服务器。实施加密通信，以确保数据传输的安全性。

2. **数据库支持**：将文件信息、用户信息等存储在数据库中，实现更灵活的数据管理。

3. **日志记录**：实现一个详细的日志系统，记录服务器的活动和用户操作，以便进行故障排除和审计。

4. ***文件夹递归上传/下载***：支持上传和下载整个文件夹，包括其子文件夹和文件。

5. ***多用户支持(已实现）***：扩展服务器以支持多个并发用户连接，每个用户都有自己的工作目录和权限。 （线程并发）

6. **文件权限管理**：支持文件和目录的权限设置，确保只有授权用户可以访问特定文件或目录。

7. **断点续传**：在文件传输过程中，实现断点续传功能，允许在传输中断后恢复传输。

8. **服务器监控与管理**：添加一个监控面板，用于实时查看服务器的状态和性能，同时提供服务器的远程管理功能。
 
   



## 关于作者































