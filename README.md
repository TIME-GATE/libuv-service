### 一、简介

```text
  业余时间用C++封装libuv写的一个后端服务，包含:
1、通信协议: TCP
2、数据协议: protobuff、JSON(TUDO)
3、数据库: mysql
4、并发控制: 线程、进程 epoll 定时器 信号(TUDO)
```

### 二、项目结构

```text
|____messages           # protobuf文件
| |____README.md
|____Makefile           # Make文件
|____dependencies       # 依赖库
| |____README.md
|____README.md
|____scripts            # 脚本
| |____start_service.sh 
| |____README.md
|____doc                # 文档
| |____README.md
|____data               # 临时文件
| |____traderDialogRsp.con
| |____traderTradingDay.con
| |____README.md
| |____traderPrivate.con
| |____traderPublic.con
| |____traderQueryRsp.con
|____autogen.sh
|____src                # 源文件
| |____database
| | |____MysqlEngine.h
| | |____MysqlEngine.cpp
| |____routes.h
| |____test
| | |____README.md
| |____util
| | |____README.md
| |____io
| | |____response.h
| | |____response.cpp
| | |____libuv.h
| | |____libuv.cpp
| | |____demo.cpp
| |____routes.cpp
| |____a.out
| |____sdk             # 第三方SDK
| | |____libthostmduserapi.so
| | |____ThostFtdcTraderApi.h
| | |____ThostFtdcUserApiDataType.h
| | |____ThostFtdcUserApiStruct.h
| | |____error.dtd
| | |____error.xml
| | |____ChildThostFtdcTraderApi.h
| | |____ThostFtdcMdApi.h
| | |____libthosttraderapi.so
| |____server.cpp
```

### 三、性能测试（TUDO）