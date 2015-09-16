#include "MyMutex.h"


MyMutex::MyMutex()
{
	pthread_mutex_init(&m_mutex, NULL);
}


MyMutex::~MyMutex()
{
	pthread_mutex_destroy(&m_mutex);
}


void MyMutex::Lock() const
{
	pthread_mutex_lock(&m_mutex);
}


void MyMutex::Unlock() const
{
	pthread_mutex_unlock(&m_mutex);
}



Lock::Lock(const IMutex& m) : m_mutex(m)
{
	m_mutex.Lock();
}


Lock::~Lock()
{
	m_mutex.Unlock();
}

