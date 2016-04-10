#pragma once
#include "../Iocp/IOCPDefine.h"

class MyLock
{
public:
	MyLock(void);
	~MyLock(void);

	void Lock();
	void UnLock();

private:
	CRITICAL_SECTION cs; 
}; 