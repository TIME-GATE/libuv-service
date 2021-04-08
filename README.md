### 一、简介

&nbsp;&nbsp;&nbsp;&nbsp;这是一个基于libuv的跨平台TCP框架:

#### 已完成
- [x] 封装libuv通信层
- [x] mysql引擎
- [x] 信号/信号量
- [x] 进程通信:共享内存、信号量
- [x] 线程管理

#### 进行中
- [ ] 线程池/线程同步
- [ ] 任务队列
- [ ] mysql事务
- [ ] 封装protobuf
- [ ] 自动构建
- [ ] 单元测试
- [ ] 性能测试


--------------

### 二、项目结构

```text
|____messages           # protobuf
| |____README.md
|____Makefile           # Make文件
|____deps       # 依赖库
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
| |____utils            # 工具类
| | |____timeutils.h
| | |____timeutils.cc
| | |____*.*
| |____io               # libuv
| | |____response.h
| | |____response.cc
| | |____libuv.h
| | |____libuv.cc
| | |____*.*  
| |____server.cc        # 入口
| |____*.*
```
### 五、编译执行

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


备注

```
c++11 -std=c++11
mac os 10.13.4
gcc 4.2.1
```