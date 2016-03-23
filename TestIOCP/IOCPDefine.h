#ifndef IOCP_DEFINE
#define IOCP_DEFINE

#include <WinSock2.h>

const int DataBuffSize  = 2 * 1024;

struct IOData
{
	/** 重叠结构 */
	OVERLAPPED overLapped;
	/** 数据缓冲 */
	WSABUF wsaBuff;
	/** 数据 */
	char szBuffer[DataBuffSize];
	/** 数据长度 */
	int BufferLen;
	/** IO投递类型 */
	int OpType;

	IOData()
	{
		ZeroMemory(szBuffer, DataBuffSize);
		BufferLen = 0;
	}
};

struct HANDLE_Data
{
	SOCKET acceptSocket;
	SOCKADDR_IN ClientAddr;

	void clear()
	{
		acceptSocket = INVALID_SOCKET;
		ZeroMemory((void*)&ClientAddr, sizeof(ClientAddr));
	}
};

#endif
