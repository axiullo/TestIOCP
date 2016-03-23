#ifndef __IOCPMODEL_H  
#define __IOCPMODEL_H  

#include <iostream>  
#include <WinSock2.h>  

#pragma comment(lib,"ws2_32.lib")  


#define OP_READ 1  
#define OP_WRITE 2  
#define OP_ACCEPT 3  
#define OP_END -1  

#define BUFFER_SIZE 1024  

using namespace std;  
//自定义结构，即“完成键”(单句柄数据)  
typedef struct tagPER_HANDLE_DATA  
{  
	SOCKET Socket;  
	SOCKADDR_STORAGE ClientAddr;  
}PER_HANDLE_DATA,*LPPER_HANDLE_DATA;  

//单个I/O操作数据  
typedef struct tagPer_IO_DATA  
{  
	OVERLAPPED Overlapped;  
	WSABUF DataBuf;  
	char buffer[1024];  
	int BufferLen;  
	int OperationType;            //IO操作类型  

}PER_IO_DATA, *LPPER_IO_DATA;  

//线程方法  
DWORD WINAPI ServerWorkerThread(LPVOID lpParam);  


#endif  