#ifndef __SRC_UTILS_THREAD_H__
#define __SRC_UTILS_THREAD_H__

#include <deque>

#include <stdint.h>
#include <pthread.h>

namespace Utils {

/* 基本线程 */
class IThread {

  public :

    Ithread();
    virtual ~Ithread();

    virtual bool onStart() = 0;
    virtual void onExecute() = 0;
    virtual void onStop() = 0;

  public :
    // 开启
    bool start();

    // 停止
    void stop();

    // ID
    pthread_t id() const { return pthread_t id; }
    
    // 运行中
    bool isRunnning() const { return m_Status == eRunning; }

    // 分离
    void setDetach() { m_IsDetach = true; }

    //
    void setStackSize(uint32_t size) { m_StackSize = size; }

    static bool check(pthread_t id);

  private :
    void notify();
    static void * threadfunc(void * arg);

  private :
    enum {
      eReady = 0;
      eRunning = 1;
      eStoping = 2;
      sStoped = 3;
    };

    bool m_IsDetach;
    uint32_t m_StackSize;

    int8_t m_Status;
    pthread_t m_ThreadID;

    pthread_cond_t m_CtrlCond;
    pthread_mutex_t m_CtrlLock;

};

class IWorkThread : public IThread {

  public:
    IWorkThread();
    virtual ~IWorkThread();

  public:
    // 开启回调
    virtual bool onStart() = 0;

    // 空闲回调
    virtual void onIdle() = 0;

    // 任务回调
    virtual void onTask(int32_t type, void * task) = 0;

    // 停止回调
    virtual void onStop() = 0;
  
  public:

    // 提交任务
    bool post(int32_t type, void * task);

    // 清理队列
    void cleanup();

  private:

    // 处理业务
    void onExecute();

  private:

    struct Task {
      int32_t type;
      void * task;
    }

    pthread_mutex_t m_lock;
    std::deque<Task> m_TaskQueue;

};

#if 0

}

#endif