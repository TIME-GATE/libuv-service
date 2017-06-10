#ifndef __LIBUV_IO_SERVICE__
#define __LIBUV_IO_SERVICE__

#include <vector>
#include <string>
#include <uv.h>

// 网络通信层
class IIOService {

  public : 

    IIOService() {};
    virtual ~IIOService () {};
  
  public :

    // 连接/监听
    int listen(const char *host, int port);
    
    // 连接期货交易中心
    //int connect(const char *host, uint16_t port, int32_t seconds);

    // 停止本地服务/交易中心服务连接
    //void stop();

  private :
    
    // TUDO: 设置本地/交易中心服务监听地址/端口
    const char *host = "0.0.0.0";
    const int port = 7000;

    const char *host_ftp = "0.0.0.0";
    const int port_ftp = 7000;

};

#endif
