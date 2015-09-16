#ifndef _MY_MUTEX_H
#define _MY_MUTEX_H


#include <pthread.h>

class IMutex
{
public:
	virtual ~IMutex() {}

	virtual void Lock() const = 0;
	virtual void Unlock() const = 0;
};

class MyMutex : public IMutex
{
public:
	MyMutex();
	~MyMutex();

	virtual void Lock() const;
	virtual void Unlock() const;

private:
	mutable pthread_mutex_t m_mutex;
};

class Lock
{
public:
	Lock(const IMutex&);
	~Lock();

private:
	const IMutex& m_mutex;
};

#endif