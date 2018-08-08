### 一、简介

&nbsp;&nbsp;&nbsp;&nbsp;基于Node底层网络库libuv写的一个TCP服务，包含如下功能:
```text
1、基于libuv编写底层通信框架
2、基于pthread编写线程安全管理模块，如线程池等
3、封装mysql、leveldb、protobuf等数据通信中间件
4、其他常用模块,如共享内存块、事务、日志等
```

逐步完善中...

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
- [x] 线程

--------------

### 四、进行中

- [x] 线程池
- [x] 任务队列
- [x] mysql事务
- [x] protobuf


### 运行

* server
```
./server.o

```

![image](http://github.com/TIME-GATE/libuv-service/blob/master/doc/liuv-server.png)

* client

```
node scripts/net_client.js
```

![image](http://github.com/TIME-GATE/libuv-service/blob/master/doc/node-client.png)




