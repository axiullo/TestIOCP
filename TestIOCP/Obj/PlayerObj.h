#pragma once
#include "../Iocp/IOCPDefine.h"

class PlayerObj
{
public:
	PlayerObj(void);
	~PlayerObj(void);

	void SetSockInfo(SOCKET s, const char* ip);
	void ClearSockInfo();

private:
	SOCKET m_sock;
	char m_IP[32];
};
