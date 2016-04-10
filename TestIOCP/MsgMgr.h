#pragma once
#include "Iocp/IOCPDefine.h"

class MsgMgr
{
public:
	static MsgMgr Instance();
	void ProcessData(const char* msg);

private:
	MsgMgr(void);
	~MsgMgr(void);
};
