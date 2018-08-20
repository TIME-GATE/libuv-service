#ifndef __SRC_UTILS_THREAD_H__
#define __SRC_UTILS_THREAD_H__

#include <deque>

#include <stdint.h>
#include <pthread.h>

namespace Utils {

/* 基本线程 */
class IThread {

  public:
    IThread();

    virtual ~IThread();

    // 开启的回调
    virtual bool onStart() = 0;

    // 处理业务
    virtual void onExecute() = 0;

    // 停止的回调
    virtual void onStop() = 0;

  public:
    // 开启
    bool start();

    // 停止
    void stop();

    static bool check(pthread_t id);

    // ID
    pthread_t id() const { return m_ThreadID; }

    // 运行中
    bool isRunning() const { return m_Status == eRunning; }

    // 分离
    void setDetach() { m_IsDetach = true; }

    // 设置栈大小
    void setStackSize(uint32_t size) { m_StackSize = size; }

  private:
    enum {
      eReady = 0,
      eRunning = 1,
      eStoping = 2,
      eStoped = 3,
    };

    void notify();

    static void * threadfunc(void * arg);

  private:

    bool m_IsDetach;
    uint32_t m_StackSize;

    int8_t m_Status;
    pthread_t m_ThreadID;

    pthread_cond_t m_CtrlCond;
    pthread_mutex_t m_CtrlLock;

};

#if 0

#endif

}

#endif