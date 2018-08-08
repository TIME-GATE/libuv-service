#include <signal.h>

#include "./io/libuv.h"
#include "./utils/timeutils.h"
// #include "./base/base.h"

/* 全局选项 0: 退出 1: 运行 2: 重新加载 3: 刷新日志 */
int g_ServerRunStatus = 0;

static void signal_handle(int signo);
static void initialize(const char * module);
static void finalize(const char * module);

void signal_handle(int signo) {
  switch(signo) {
    case SIGINT :
      g_ServerRunStatus = 0;
      break;
    case SIGTERM :
      g_ServerRunStatus = 0;
      break;
    case SIGHUP :
      g_ServerRunStatus = 2;
      break;
    case SIGUSR1 :
      g_ServerRunStatus = 3;
      break;
  }
}

void initialize(const char * module) {
  bool rc = false;

  // 信号初始化
  signal(SIGPIPE, SIG_IGN);
  signal(SIGINT, signal_handle);
  signal(SIGHUP, signal_handle);
  signal(SIGTERM, signal_handle);
  signal(SIGUSR1, signal_handle);

  // TODO: 加载系统配置
}

void finalize(const char * module) {
  // TODO: 卸载系统配置
}

int main(int argc, char ** argv){

  std::string module;
  // 服务初始化
  initialize(module.c_str());
  
  printf("###################### BEGIN START SERVICE ######################\n");
  
  IIOService server;
  g_ServerRunStatus = server.listen("0.0.0.0", 7000);

  // 开启线程服务
  while (g_ServerRunStatus) {
    Utils::TimeUtils::sleep(100);
  }
  
  // LOG_INFO("###################### START SERVICE FAILED ######################\n");

  printf("###################### START SERVICE FAILED ######################\n");

  // 销毁
  finalize(module.c_str());

  return 0;
}
