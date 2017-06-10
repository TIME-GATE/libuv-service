#ifndef __LIBUV_IO_SERVICE__
#define __LIBUV_IO_SERVICE__

#include <vector>
#include <string>
#include <uv.h>

// 会话列表
//typedef std::vector<sid_t> SidList

// 网络通信层
class IIOService {

  public : 

    IIOService();
    virtual ~IIOService ();

  public :
   
    // 接受连接事件, 定义连接回调
    static void onConnect(uv_stream_t *server, int status);
  
  public :

    // 连接/监听
    int listen(const char *host, int port);
    
    // 连接期货交易中心
    int connect(const char *host, uint16_t port, int32_t seconds);

    // 停止本地服务/交易中心服务连接
    void stop();
    
    // 申请连接内存
    static void read(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);

    // 发送数据
    static void send(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf);

  private :
    
    // TUDO: 设置本地/交易中心服务监听地址/端口
    const char *host = "0.0.0.0";
    const int port = 7000;

    const char *host_ftp = "0.0.0.0";
    const int port_ftp = 7000;
  
  private :

  static uv_loop_t *loop;
  struct sockaddr_in addr;
  const int  default_backlog = 128;

};

#endif
