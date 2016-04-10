#include "MyLock.h"

MyLock::MyLock(void)
{
	InitializeCriticalSection(&cs); 
}

MyLock::~MyLock(void)
{ 
	DeleteCriticalSection(&cs);
}

void MyLock::Lock()
{
	EnterCriticalSection(&cs); 
}

void MyLock::UnLock()
{
	LeaveCriticalSection(&cs);  
}
