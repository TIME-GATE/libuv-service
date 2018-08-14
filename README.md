### 一、简介

&nbsp;&nbsp;&nbsp;&nbsp;项目的初衷是研究Node底层网络库libuv，发现基于libuv的资料较少,t同时也为了能再系统地学习下C++及其应用，所以基于libuv编写了一个TCP后端服务以供分享，该项目作为一个简易的通用框架模型，希望其可以在跨平台应用编程实践中带来一些价值，具体功能如下:

```text
1、基于libuv编写底层通信框架
2、基于pthread编写线程安全管理模块，如线程同步
3、封装mysql、protobuf等数据通信中间件
4、其他常用模块,如共享内存、事务、日志等
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
|____scripts            # 脚本 node客户端通信
| |____start_service.sh
| |____net_client.js 
| |____README.md
| |____*.*
|____doc                # 文档
| |____README.md
|____data               # 临时文件、日志文件
| |____*.*
|____autogen.sh
|____src                # 源文件
| |____database
| | |____MysqlEngine.h
| | |____MysqlEngine.cc
| |____routes.h
| |____test             # 测试
| | |____*.*
| |____utils            # 工具类，如时间戳等
| | |____timeutils.h
| | |____timeutils.cc
| | |____*.*
| |____io               # libuv
| | |____response.h
| | |____response.cc
| | |____libuv.h
| | |____libuv.cc
| | |____*.*  
| |____server.cc        # 项目入口
| |____*.*
```

--------------

### 三、已完成
- [x] 封装libuv通信层
- [x] mysql引擎
- [x] 信号signal
- [x] 进程通信:共享内存、信号量

--------------

### 四、进行中

- [x] 线程池
- [x] 任务队列
- [x] mysql事务
- [x] 线程管理
- [x] 封装protobuf
- [x] 自动构建
- [x] 单元测试
- [x] 性能测试



### 五、运行

* server

```
./server.o
```

![image](https://github.com/TIME-GATE/libuv-service/blob/master/doc/libuv-server.png)

* client

```
node scripts/net_client.js
```

![image](https://github.com/TIME-GATE/libuv-service/blob/master/doc/node-client.png)




### 五、备注

```
c++11
mac os 10.13.4
```