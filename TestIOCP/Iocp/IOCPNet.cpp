//////////////////////////////////////////////////////////////////////////
/*
1.没有为IOMsg创建一个池，现在试验有消息就创建，使用完就释放。
2.没有使用WSAIoctl创建客户端socket和接收。
*/
//////////////////////////////////////////////////////////////////////////

#include "IOCPNet.h"
#include "Utils.h"
#include "../Obj/MyLock.h"
#include "GlogHelper.h"

#include <stdio.h>
#include <algorithm>

#pragma warning(disable:4996) //全部关掉

DWORD WINAPI AcceptThread(void* pParam);
DWORD WINAPI WorkThread(void* pParam);

const int MAX_CONNECT_NUM = 10;

IOCPNet::IOCPNet(void)
{
	m_ServerSocket = INVALID_SOCKET;
	m_nListenPort = -1;
	ZeroMemory(m_szServerAddress, 0);

	m_vecFreeSocket.reserve(MAX_CONNECT_NUM);
	m_vecUseSocket.reserve(MAX_CONNECT_NUM);
	m_mapClientSocket.clear();

	m_nConnectNum = 0;
}

IOCPNet::~IOCPNet(void)
{
	End();
}

int IOCPNet::Start( int nPort, const char* pAddress )
{
	WSADATA wsaData;
	int nError = WSAStartup(MAKEWORD(2,2), &wsaData);

	if(nError == SOCKET_ERROR)
	{
		return -1;
	}

	/** 创建完成端口 */
	m_CompletePort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);

	if (m_CompletePort == NULL)
	{
		return -1;
	}

	m_bRun = true;
	/** 创建工作线程 */ 
	SYSTEM_INFO sys;
	GetSystemInfo(&sys);

	int NumberOfConcurrentThreads = sys.dwNumberOfProcessors * 2;
	m_nThreadCloseNum = NumberOfConcurrentThreads;
	m_nWorkThreadNum = NumberOfConcurrentThreads;

	for (int n = 0; n < NumberOfConcurrentThreads; ++n)
	{
		DWORD nThreadID = 0;
		HANDLE h = CreateThread(NULL, 0, WorkThread, this, 0, &nThreadID);

		if (h == NULL)
		{
			printf( " CreateThread() failed with error %d\n " , WSAGetLastError());
			return -1;
		}

		printf("create workerthread id = %d \n", nThreadID);
		CloseHandle(h);
	}

	/** 绑定端口 */
	m_ServerSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED); //非阻塞
	//socket(AF_INET, SOCK_STREAM, 0);  //阻塞 

	if(m_ServerSocket == INVALID_SOCKET)
	{
		printf("%d \n", WSAGetLastError());
		return -1;
	}

	m_nListenPort = nPort;
	strcpy(m_szServerAddress, pAddress);

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(nPort);
	addr.sin_addr.s_addr = inet_addr(m_szServerAddress);

	nError = bind(m_ServerSocket, (LPSOCKADDR)&addr, sizeof(addr));

	if (nError == SOCKET_ERROR)
	{
		printf("%d, \n", WSAGetLastError());
		return -1;
	}

	nError = listen(m_ServerSocket, SOMAXCONN);

	if (nError == SOCKET_ERROR)
	{
		return -1;
	}

	HANDLE hMain = CreateThread(NULL, 0, AcceptThread, this, 0, 0);
	CloseHandle(hMain);

	return 0;
}

int IOCPNet::End()
{
	m_bRun = false;
	WSACleanup();

	for(int i = 0; i < m_nWorkThreadNum; ++i)
	{
		PostQueuedCompletionStatus(m_CompletePort, 1, 0, NULL);
	}

	ClearAllClientSocket();	

	while(m_nThreadCloseNum > 0)
	{
		//printf("cur close num %d\n", m_nThreadCloseNum);
	}

	return 0;
}

void IOCPNet::ClearAllClientSocket()
{
	ClientMap::iterator iter = m_mapClientSocket.begin();

	while(iter != m_mapClientSocket.end())
	{
		if (iter->first != INVALID_SOCKET)
		{
			closesocket(iter->first);
		}

		iter = DelelteHandleData(iter);
	}
}

void IOCPNet::ClearClientSocket( SOCKET s )
{
	MyLock lock;

	ClientMap::iterator iter = m_mapClientSocket.find(s);

	if (iter != m_mapClientSocket.end())
	{
		DelelteHandleData(iter);
	}

	vector<SOCKET>::iterator retIter = find(m_vecUseSocket.begin(), m_vecUseSocket.end(), s);

	if (retIter != m_vecUseSocket.end())
	{
		m_vecFreeSocket.push_back(*retIter);
		m_vecUseSocket.erase(retIter);
	}

	//是否要清理IOData
}

ClientMap::iterator IOCPNet::DelelteHandleData( ClientMap::iterator iter )
{
	HANDLE_Data* pHandleData = iter->second;
	ClientMap::iterator ret = m_mapClientSocket.erase(iter);
	pHandleData->clear();
	delete pHandleData;
	pHandleData = NULL;

	return ret;
}

bool IOCPNet::InitClientSocket()
{
	//这个初始化应该是 当用acceptEx时使用的，因为socket可以预先定义。而WSAAccept的socket不固定。
	for (int n = 0; n < MAX_CONNECT_NUM; ++n)
	{
		HANDLE_Data* data = new HANDLE_Data();

		if (!data)
		{
			return false;
		}

		data->acceptSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

		m_vecFreeSocket.push_back(data->acceptSocket);
		m_mapClientSocket.insert(make_pair(data->acceptSocket, data));
	}

	return true;
}

DWORD IOCPNet::WSAAcceptProcess()
{
	IOCPNet* pThis = this;

	while (true)
	{
		HANDLE_Data* hAccept = NULL;
		SOCKADDR_IN clientAddr;
		int len = sizeof(clientAddr);

		int acceptSocket = WSAAccept(pThis->m_ServerSocket, (sockaddr*)&clientAddr, &len, NULL, NULL);

		if (!m_bRun)
		{
			return 0;
		}

		if(acceptSocket == INVALID_SOCKET)
		{
			printf("create socket fail %d \n", WSAGetLastError());
			/*return -1;*/
			continue;
		}

		if (m_nConnectNum >= MAX_CONNECT_NUM)
		{
			closesocket(acceptSocket);
			printf("server has already connect full cur = %d  max = %d \n", m_nConnectNum, MAX_CONNECT_NUM);
			continue;
		}

		hAccept = new HANDLE_Data;

		if (!hAccept)
		{
			closesocket(acceptSocket);
			printf("new HANDLE_Data fail \n");
			return -1;
		}

		hAccept->acceptSocket = acceptSocket;
		memcpy(&hAccept->ClientAddr, &clientAddr, sizeof(SOCKADDR_IN));

		pThis->m_mapClientSocket.insert(make_pair(hAccept->acceptSocket, hAccept));
		printf("socket[%d] port[%d] ip[%s] connect \n", (int)hAccept->acceptSocket, hAccept->ClientAddr.sin_port, inet_ntoa(hAccept->ClientAddr.sin_addr));

		HANDLE h = CreateIoCompletionPort((HANDLE)hAccept->acceptSocket, pThis->m_CompletePort, (DWORD)hAccept, 0);

		if (h == NULL)
		{
			closesocket(hAccept->acceptSocket);
			pThis->ClearClientSocket(hAccept->acceptSocket);
			printf("CreateIoCompletionPort fail %d \n", WSAGetLastError());
			continue;
		}
		
		IOData* data = new IOData;
		if (!data)
		{
			return -1;
		}

		ZeroMemory(&data->overLapped, sizeof(data->overLapped));
		data->wsaBuff.len = DataBuffSize;
		data->wsaBuff.buf = data->szBuffer;
		data->OpType = IO_TYPE_READ;

		DWORD RecvBytes;  
		DWORD Flags = 0;  

		int nError = NO_ERROR;
		nError = WSARecv(hAccept->acceptSocket, &data->wsaBuff, 1, &RecvBytes, &Flags, &data->overLapped, NULL);

		if (nError == SOCKET_ERROR)
		{
			int nError = WSAGetLastError();

			if (nError != WSA_IO_PENDING )
			{
				printf("PostIoMessage error socket=%d type=%d error=%d \n", hAccept->acceptSocket, IO_TYPE_READ, nError);
			}	
		}
	}

	return 0;
}

void IOCPNet::Send( SOCKET s, const char* szData )
{
	if (m_mapClientSocket.find(s) == m_mapClientSocket.end())
	{
		return;
	}

	int nSize = strlen(szData);
	if (nSize > DataBuffSize)
	{
		printf("socket %d send data too long len = %d \n", (int)s, nSize);
		return;
	}

	IOData* data = new IOData;
	if (!data)
	{
		return;
	}

	ZeroMemory(&data->overLapped, sizeof(data->overLapped));
	memcpy(data->szBuffer, szData, nSize);
	data->wsaBuff.len = nSize;
	data->wsaBuff.buf =data->szBuffer;
	data->OpType = IO_TYPE_WRITE;

	DWORD outFlags = 0;
	DWORD dwOut = 0;

	if(WSASend(s, &data->wsaBuff, 1, &dwOut, 0, &data->overLapped, NULL) == SOCKET_ERROR)
	{
		if  (WSAGetLastError()  !=  ERROR_IO_PENDING)
		{
			printf( " WSASend() failed with error %d\n " , WSAGetLastError());
			return ;
		}
	}
}

void IOCPNet::Receive( SOCKET s, const char* szData )
{
	printf("%s \n", szData);
}

void IOCPNet::ProcessIOData( SOCKET s, IOData* pData )
{
	if(pData->OpType == IO_TYPE_READ)
	{
		Receive(s, pData->wsaBuff.buf);
		delete pData;
		pData = NULL;

		char* pOut = "server say hello";
		Send(s, pOut);
	}
	else if (pData->OpType == IO_TYPE_WRITE)
	{
		delete pData;
		pData = NULL;

		IOData* pNewData = new IOData;

		if(!pNewData)
			return;

		DWORD RecvBytes;  
		DWORD Flags = 0;  
		ZeroMemory(pNewData->szBuffer, DataBuffSize);
		ZeroMemory(&pNewData->overLapped, sizeof(OVERLAPPED));
		pNewData->wsaBuff.len = DataBuffSize;
		pNewData->wsaBuff.buf = pNewData->szBuffer;
		pNewData->OpType = IO_TYPE_READ;
		int nRet = WSARecv(s, &pNewData->wsaBuff, 1, &RecvBytes, &Flags, &pNewData->overLapped, NULL);
	}
}

void IOCPNet::ReduceThreadCloseNum()
{
	MyLock lock;
	lock.Lock();
	printf("cur close num %d\n", m_nThreadCloseNum);
	--m_nThreadCloseNum;	
	lock.UnLock();
}

DWORD WINAPI WorkThread(void* pParam)
{
	IOCPNet* pThis = (IOCPNet*)pParam;
	DWORD nTransferredSize = 0;
	IOData* pData = NULL;
	LPOVERLAPPED lpOverlapped = NULL;
	HANDLE_Data* hAccept = NULL;

	while(true)
	{
		if (GetQueuedCompletionStatus(pThis->m_CompletePort, &nTransferredSize, (PULONG_PTR)&hAccept, &lpOverlapped, INFINITE))
		{
			if (nTransferredSize <= 0)
				continue;

			if (!hAccept)
			{
				pThis->ReduceThreadCloseNum();
				printf("Thread %d end\n", GetCurrentThreadId());
				return 0;
			}

			char out[1024];
			sprintf(out, "GetQueuedCompletionStatus  true  threadID = %d transferredSize = %d socket = %d \n", GetCurrentThreadId(), nTransferredSize, hAccept->acceptSocket);
			DebugOut(out);

			pData = CONTAINING_RECORD(lpOverlapped, IOData, overLapped);

			pThis->ProcessIOData(hAccept->acceptSocket, pData);			
		}
		else
		{
			if(!hAccept)
				continue;

			int nError = WSAGetLastError();

			printf("GetQueuedCompletionStatus  false %d \n", nError);

			if (ERROR_NETNAME_DELETED == nError)
			{
				//TCP 通道会非正常关闭. 操作：客户端异常关闭断开连接
				pThis->ClearClientSocket(hAccept->acceptSocket);
				closesocket(hAccept->acceptSocket);
			}
		}
	}

	return 0;
}

DWORD WINAPI AcceptThread( void* pParam )
{
	IOCPNet* pThis = (IOCPNet*)pParam;

	DWORD ret = pThis->WSAAcceptProcess();

	return 0;
}
