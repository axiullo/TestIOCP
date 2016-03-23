#include "PlayerObj.h"

PlayerObj::PlayerObj(void)
:m_sock(INVALID_SOCKET)
{
	ZeroMemory(m_IP, sizeof(m_IP));
}

PlayerObj::~PlayerObj(void)
{
}

void PlayerObj::SetSockInfo( SOCKET s, const char* ip )
{
	m_sock = s;
	strncpy_s(m_IP, ip, sizeof(m_IP));
}

void PlayerObj::ClearSockInfo()
{
	m_sock = INVALID_SOCKET;
	ZeroMemory(m_IP, sizeof(m_IP));
}
