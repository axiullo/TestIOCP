#pragma once
#include <map>
#include <vector>

#include "IOCPDefine.h"

using namespace std;


enum IO_OPERATION_TYPE
{
	IO_TYPE_ACCEPT,
	IO_TYPE_READ,
	IO_TYPE_WRITE,
};

typedef map<SOCKET, HANDLE_Data*> ClientMap;
typedef map<SOCKET, IOData*> DataMap;

class IOCPNet
{
public:
	IOCPNet(void);
	~IOCPNet(void);

	int Start(int nPort, const char* pAddress);
	int End();
	void Send(SOCKET s, const char* szData);
	void Receive(SOCKET s, const char* szData);
	
	void ClearClientSocket(SOCKET s);

	DWORD WSAAcceptProcess();

	void ProcessIOData(SOCKET s, IOData* pData);

	int GetThreadCloseNum()
	{
		return m_nThreadCloseNum;
	}

	void ReduceThreadCloseNum();

private:
	void ClearAllClientSocket();
	bool InitClientSocket();
	ClientMap::iterator DelelteHandleData(ClientMap::iterator iter);
	
public:
	HANDLE m_CompletePort;
	int m_nListenPort;
	char m_szServerAddress[20];
	SOCKET m_ServerSocket;
	ClientMap m_mapClientSocket;
	vector<SOCKET> m_vecFreeSocket;
	vector<SOCKET> m_vecUseSocket;
	//DataMap m_mapIOData;

	int m_nConnectNum;
	int m_nThreadCloseNum;
	int m_nWorkThreadNum;
	bool m_bRun;
};
