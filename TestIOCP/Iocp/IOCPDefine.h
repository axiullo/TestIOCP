#ifndef IOCP_DEFINE
#define IOCP_DEFINE

#include <WinSock2.h>

const int DataBuffSize  = 2 * 1024;

struct IOData
{
	/** �ص��ṹ */
	OVERLAPPED overLapped;
	/** ���ݻ��� */
	WSABUF wsaBuff;
	/** ���� */
	char szBuffer[DataBuffSize];
	/** ���ݳ��� */
	int BufferLen;
	/** IOͶ������ */
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
