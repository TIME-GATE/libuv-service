### 一、简介

```text
  业余时间用C++封装libuv写的一个后端服务，包含:
1、通信协议: TCP, HTTP
2、数据协议: protobuf、JSON(TUDO)
3、数据库: mysql
4、并发控制: 线程 epoll 定时器 信号
```

--------------

### 二、项目结构

```text
|____messages           # protobuf
| |____README.md
|____Makefile           # Make文件
|____dependencies       # 依赖库
| |____README.md
| |____*.*
|____README.md
|____scripts            # 脚本
| |____start_service.sh 
| |____README.md
| |____*.*
|____doc                # 文档
| |____README.md
|____data               # 临时文件
| |____*.*
|____autogen.sh
|____src                # 源文件
| |____database
| | |____MysqlEngine.h
| | |____MysqlEngine.cpp
| |____routes.h
| |____test
| | |____*.*
| |____utils            # 工具类，如时间戳等
| | |____timeutils.h
| | |____timeutils.cpp
| | |____*.*
| |____io
| | |____response.h
| | |____response.cpp
| | |____libuv.h
| | |____libuv.cpp
| | |____demo.cpp
| | |____*.*
| |____routes.cpp
| |____sdk             # 第三方SDK
| |____server.cpp
| |____*.*
```

--------------

### 三、已完成
- [x] 封装libuv通信层
- [x] mysql引擎
- [x] 信号signal

--------------

### 四、进行中

- [x] 线程及线程池
- [x] 任务队列
- [x] mysql事务
- [x] protobuf
