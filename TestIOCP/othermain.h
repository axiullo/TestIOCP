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
//�Զ���ṹ��������ɼ���(���������)  
typedef struct tagPER_HANDLE_DATA  
{  
	SOCKET Socket;  
	SOCKADDR_STORAGE ClientAddr;  
}PER_HANDLE_DATA,*LPPER_HANDLE_DATA;  

//����I/O��������  
typedef struct tagPer_IO_DATA  
{  
	OVERLAPPED Overlapped;  
	WSABUF DataBuf;  
	char buffer[1024];  
	int BufferLen;  
	int OperationType;            //IO��������  

}PER_IO_DATA, *LPPER_IO_DATA;  

//�̷߳���  
DWORD WINAPI ServerWorkerThread(LPVOID lpParam);  


#endif  