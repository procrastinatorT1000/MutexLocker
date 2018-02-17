//============================================================================
// Name        : mutex_auto.cpp
// Author      : Slobodchikov RS
// Version     :
// Copyright   : romka rabotaet
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

typedef unsigned long DWORD;

#define SCOPE_LOCK_MUTEX(hMutex) CMutexLock _tmp_mtx_capt(hMutex);

class CAutoMutex
{
  pthread_mutex_t m_mutex;

  CAutoMutex(const CAutoMutex&);
  CAutoMutex& operator=(const CAutoMutex&);

public:
  CAutoMutex()
  {
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&m_mutex, &attr);
    pthread_mutexattr_destroy(&attr);
  }
  ~CAutoMutex()
  {
    pthread_mutex_destroy(&m_mutex);
  }
  pthread_mutex_t& get()
  {
    return m_mutex;
  }
};

class CMutexLock
{
  pthread_mutex_t m_mutex;

  // запрещаем копирование
  CMutexLock(const CMutexLock&);
  CMutexLock& operator=(const CMutexLock&);
public:
  // занимаем мютекс при конструировании объекта
  CMutexLock(pthread_mutex_t mutex): m_mutex(mutex)
  {
    const DWORD res = WaitForSingleObject(m_mutex, INFINITE);
    assert(res == WAIT_OBJECT_0);
  }
  // освобождаем мютекс при удалении объекта
  ~CMutexLock()
  {
    const BOOL res = ReleaseMutex(m_mutex);
    assert(res);
  }
};

int main() {
	cout << "lala" << endl; // prints lala
	return 0;
}
