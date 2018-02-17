//============================================================================
// Name        : mutex_auto.cpp
// Author      : Slobodchikov RS
// Version     :
// Copyright   : romka rabotaet
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <unistd.h>

using namespace std;

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
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_DEFAULT);
    pthread_mutex_init(&m_mutex, NULL);
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
  pthread_mutex_t& m_mutex;

  // запрещаем копирование
  CMutexLock(const CMutexLock&);
  CMutexLock& operator=(const CMutexLock&);
public:
  // занимаем мютекс при конструировании объекта
  CMutexLock(pthread_mutex_t& mutex): m_mutex(mutex)
  {
	  pthread_mutex_lock(&m_mutex);

	  cout<<"Lock"<<endl;
  }
  // освобождаем мютекс при удалении объекта
  ~CMutexLock()
  {
	  pthread_mutex_unlock(&m_mutex);
	  cout<<"Unlock"<<endl;
  }
};


CAutoMutex g_mutex;
int data = 0;


void* locker(void *a)
{

	cout<<"Locker thread"<<endl;

	{
		SCOPE_LOCK_MUTEX(g_mutex.get());

		data = *(int*)a;

		cout<<"data = "<<data<<endl;

		sleep(1);
	}

	cout<<"Locker finished"<<endl;

	return NULL;
}

void *waiter(void *)
{
	cout<<"Waiter thread"<<endl;
	{
		cout<<"wait..."<<endl;
		SCOPE_LOCK_MUTEX(g_mutex.get());
		cout<<"Got mutex waiter()"<<endl;

		sleep(3);

		data = 111;
		cout<<"data = "<<data<<endl;

	}

	cout<<"Waiter finished"<<endl;
	return NULL;
}

int main() {

	cout<<"data = "<<data<<endl;

	pthread_t lock_thread, wait_thread;

	int arg = 1;

	pthread_create(&lock_thread, NULL, locker, &arg);
	pthread_create(&wait_thread, NULL, waiter, NULL);


	pthread_join(wait_thread, NULL);
	pthread_join(lock_thread, NULL);

	cout<<"data = "<<data<<endl;

	return 0;
}
