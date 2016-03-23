#include "MyLock.h"

MyLock::MyLock(void)
{
	InitializeCriticalSection(&cs);
	EnterCriticalSection(&cs); 
}

MyLock::~MyLock(void)
{
	LeaveCriticalSection(&cs);  
	DeleteCriticalSection(&cs);
}

//void MyLock::Lock()
//{
//	EnterCriticalSection(&cs); 
//}
//
//void MyLock::UnLock()
//{
//	LeaveCriticalSection(&cs);  
//}
