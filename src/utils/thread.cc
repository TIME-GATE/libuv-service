
#include <vector>

#include <errno.h>
#include <signal.h>

#include "time.h"
#include "thread.h"
#include "timeutils.h"

namespace Utils {

IThread::IThread() {
  m_Status = eReady;
  m_IsDetach = false;
  m_StackSize = 0;
  m_ThreadID = 0;
  pthread_cond_init(&m_CtrlCond, NULL);
  pthread_mutex_init(&m_CtrlLock, NULL);
}

IThread::~IThread() {
  if (!m_IsDetach) {
    pthread_join(m_ThreadID, NULL);
  }

  pthread_cond_destroy(&m_CtrlCond);
  pthread_mutex_destroy(&m_CtrlLock);
}

bool IThread::start() {

  int32_t rc = 0;
  pthread_attr_t attr;

  pthread_attr_init(&attr);

  if (m_StackSize > 0) {
    pthread_attr_setstacksize(&attr, m_StackSize);
  }

  if (m_IsDetach) {
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  }

  m_Status = eRunning;

  rc = pthread_create(&m_ThreadID, &attr, threadfunc, this);  
  pthread_attr_destroy(&attr);

  return (rc == 0);
}

void IThread::stop() {
  pthread_mutex_lock(&m_CtrlLock);

  if (m_Status == eReady) {
    m_Status = eStoped;
  } else if (m_Status == eRunning) {
    m_Status = eStoping;
  }

  while (m_Status != eStoped && pthread_self() != m_ThreadID) {
    pthread_cond_wait(&m_CtrlCond, &m_CtrlLock);
  }

  pthread_mutex_unlock(&m_CtrlLock);
}

void IThread::notify() {
  pthread_mutex_lock(&m_CtrlLock);

  m_Status = eStoped;
  pthread_cond_signal(&m_CtrlCond);

  pthread_mutex_unlock(&m_CtrlLock);
}

bool IThread::check(pthread_t id) {
  int32_t rc = pthread_kill(id, 0);
  return (rc != ESRCH && rc != EINVAL);
}

void * IThread::threadfunc(void * arg) {
  
  IThread * thread = (IThread *)arg;

  sigset_t mask;
  sigfillset(&mask);
  

  pthread_sigmask(SIG_SETMASK, &mask, NULL);

  // 线程开始
  if (thread->onStart()) {
    while (thread->isRunning()) {
      thread->onExecute();
    }
  }

  // 线程停止了
  thread->onStop();

  // 通知调用者, 线程已经停止了
  thread->notify();

  return NULL;
}

}


/* 测试Thread Demo */
class DemoIThread : public Utils::IThread {

public:
  DemoIThread(uint8_t count) : m_Count(count) {
    pthread_mutex_init(&m_Lock, NULL);
  }

  ~DemoIThread() {
    pthread_mutex_destroy(&m_Lock);
  }

  void process() {

    pthread_mutex_lock(&m_Lock);
    while (--m_Count >= 0) {
      Utils::TimeUtils::sleep(1000);
      printf("count now is : %d\n", m_Count);
    }

    pthread_mutex_unlock(&m_Lock);
  }

public:
  bool onStart() {
    if (m_Count >= 0) {
      return true;
    }

    return false;
  }

  void onExecute() {
    this->process();
  }

  void onStop(){
    m_Count = 0;
  }

private:
  int m_Count;
  pthread_mutex_t m_Lock;

  uint8_t m_Number;
  std::vector<DemoIThread *> m_DemoIThread;
};

int main() {
  Utils::IThread * thread1 = new DemoIThread(10);
  Utils::IThread * thread2 = new DemoIThread(10);

  printf("####### Thread start #######\n");

  thread1->start();
  thread2->start();

  printf("####### do otherwise #######\n");

  thread1->stop();
  thread2->stop();

  printf("####### Thread stop #######\n");

}