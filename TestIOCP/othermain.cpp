//// IocpModel.cpp : �������̨Ӧ�ó������ڵ㡣  
////  
// 
//#include "othermain.h"  
//  
//#define Op_READ 1  
//#define Op_WRITE 2  
//  
//WSADATA wsa;  
//HANDLE CompletionPort;  
//SYSTEM_INFO Systeminfo;  
//SOCKADDR_IN InternetAddr;  
//  
//HANDLE ThreadHanle;  
//  
//  
//void ChangeHandleData(LPPER_IO_DATA lpPerIoContext,int state)  
//{  
///** 
//    Fucntion Description: 
//        ���ݴ�������state�������иı���һ����IO���� 
//    Parameter�� 
//        lpPerIoContext:��һ��IO�����Ľ�� 
//        state           ��һ��IO������״̬ 
//    Return Value: 
//        ����ֵΪ�� 
//**/  
//    if(OP_READ == state)  
//    {  
//        //����˶�ȡ�ͻ������ݣ����ҷ�������  
//        lpPerIoContext->OperationType = OP_WRITE;  
//        ZeroMemory(&(lpPerIoContext->Overlapped),sizeof(OVERLAPPED));  
//        //�������ݸ��ͻ���  
//        char *sendClientData = "sendClientData";  
//        lpPerIoContext->DataBuf.buf = sendClientData;  
//        lpPerIoContext->DataBuf.len = BUFFER_SIZE;  
//    }  
//    if(OP_WRITE == state)  
//    {      
//        //��һ��IO���ݷ��Ϳͻ�����ɣ��������� OP_END  
//        //�������ܣ�����������ΪOP_READ,ע������� buff  
//        //�������ͣ�����������ΪOP_WRITE  
//        lpPerIoContext->OperationType = OP_READ;  
//        lpPerIoContext->DataBuf.buf = lpPerIoContext->buffer;  
//        lpPerIoContext->DataBuf.len = BUFFER_SIZE;  
//    }  
//    if(OP_ACCEPT == state)  
//    {  
//        //�����������ӣ��Ѿ����Ϊ������  
//        lpPerIoContext->OperationType = OP_READ;  
//        ZeroMemory(&(lpPerIoContext->Overlapped),sizeof(OVERLAPPED));  
//        ZeroMemory(&(lpPerIoContext->DataBuf),sizeof(lpPerIoContext->DataBuf));  
//        lpPerIoContext->DataBuf.buf = lpPerIoContext->buffer;  
//        lpPerIoContext->DataBuf.len = BUFFER_SIZE;  
//        cout << "���ݰ��������ͱ���Ϊ : OP_READ"<<endl;  
//    }  
//}  
//  
//void SendHandleData(LPPER_IO_DATA lpPerIoContext,LPPER_HANDLE_DATA perHandData)  
//{  
//    /** 
//        ����lpPerIoContext����OperationType��������һ���Ĳ��� 
//    **/  
//    DWORD dwIosize = 0;  
//    int nResult  =0;  
//    DWORD RecvBytes = 0;  
//    DWORD nFlag = 0;  
//    if(lpPerIoContext->OperationType  == OP_WRITE)  
//    {  
//        /*�������ݵ��ͻ���*/  
//        nResult = WSASend(perHandData->Socket,&(lpPerIoContext->DataBuf),1,&dwIosize,0,&(lpPerIoContext->Overlapped),NULL);  
//        if((nResult == SOCKET_ERROR) && WSAGetLastError() != ERROR_IO_PENDING )  
//        {  
//            cout<< "WSASend error :"<<WSAGetLastError()<<endl;  
//            ::closesocket(perHandData->Socket);  
//            return;  
//        }  
//  
//    }  
//    else if(lpPerIoContext->OperationType == OP_READ)  
//    {  
//        //��գ�׼���¸�I/O����  
//        ZeroMemory(&(lpPerIoContext->Overlapped),sizeof(OVERLAPPED));  
//        //ע�⣺���������buffer��Ҫ��Ȼ���ݻ����һ��ճ��һ��  
//        ZeroMemory(&(lpPerIoContext->buffer),sizeof(lpPerIoContext->buffer));  
//        lpPerIoContext->DataBuf.len = BUFFER_SIZE;  
//        if(SOCKET_ERROR == WSARecv(perHandData->Socket, &(lpPerIoContext->DataBuf), 1,&RecvBytes,&nFlag,&(lpPerIoContext->Overlapped),NULL)){  
//            cout << "WSARecv() failed: " << WSAGetLastError() << endl;  
//            return;  
//        }  
//    }  
//    else if(lpPerIoContext->OperationType == OP_END)  
//    {  
//        ::closesocket(perHandData->Socket);  
//    }  
//}  
//  
//int main()  
//{  
//    SOCKET ListenSock;  
//    SOCKET accpSocket;  
//    PER_HANDLE_DATA *PerHandleData = NULL;  
//    SOCKADDR_IN saRemote;  
//    int RemoteLen;  
//  
//    cout<<"Server start......"<<endl;  
//    WSAStartup(MAKEWORD(2,2),&wsa);  
//    GetSystemInfo(&Systeminfo);  
//  
//    //����һ��IO��ɶ˿�  
//    CompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,0);  
//    if ( CompletionPort == INVALID_HANDLE_VALUE )   
//    {  
//        cout<<"��ɶ˿ڴ���ʧ��!!!!"<<endl;  
//        return 0;  
//    }  
//  
//    //����IOCP�����߳�(����CPU���� * 2)  
//    for(int i = 0;i< Systeminfo.dwNumberOfProcessors * 2 ; ++ i)  
//    {  
//        //�����߳�  
//        ThreadHanle = CreateThread(NULL,0,ServerWorkerThread,CompletionPort,0,NULL);  
//        CloseHandle(ThreadHanle);  
//    }  
//  
//    //4. ����һ�������׽��֣�  
//    ListenSock = WSASocket(AF_INET,SOCK_STREAM,0,NULL,0,WSA_FLAG_OVERLAPPED);  
//    if(ListenSock == INVALID_SOCKET)  
//    {  
//        cout<<"�׽��ִ��� ʧ��!!"<<endl;  
//        return 0;  
//    }  
//  
//    InternetAddr.sin_family = AF_INET;  
//    InternetAddr.sin_port = htons(10086);  
//    InternetAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  
//    if(SOCKET_ERROR == bind(ListenSock,(SOCKADDR*)&InternetAddr,sizeof(InternetAddr)))  
//    {  
//        cout<<"�������󶨵�ַ��Ϣ ʧ��!! errid:"<<GetLastError()<<endl;  
//        closesocket(ListenSock);  
//        return 0;  
//    }  
//    int ret = listen(ListenSock, 5);  
//    if (ret == SOCKET_ERROR)  
//    {  
//        cout<<"�����׽���ʧ��!! errid:"<<GetLastError()<<endl;  
//        closesocket(ListenSock);  
//        return 0;  
//    }  
//  
//    while(TRUE)  
//    {  
//        //��������  
//        RemoteLen = sizeof(saRemote);  
//        //accpSocket = WSAAccept(ListenSock,NULL,NULL,NULL,0);  
//        accpSocket = accept(ListenSock, (SOCKADDR*)&saRemote, &RemoteLen);  
//        if(SOCKET_ERROR == accpSocket)  
//        {  
//            cout<<"�����׽��ִ���!! errid:"<<GetLastError()<<endl;  
//            closesocket(accpSocket);  
//            return 0;  
//        }  
//  
//        //�����������׽��ֹ��������Ϣ�ṹ  
//        PerHandleData =(LPPER_HANDLE_DATA)GlobalAlloc(GPTR,sizeof(PER_HANDLE_DATA));  
//        if(PerHandleData == NULL)  
//        {  
//            cout<<"PerHandleData = null!! errid:"<<GetLastError()<<endl;  
//            closesocket(accpSocket);  
//            return 0;  
//        }  
//        cout<< "Socker number: "<< accpSocket << " connected" << endl;  
//        PerHandleData->Socket = accpSocket;  
//        memcpy(&PerHandleData->ClientAddr,&saRemote,RemoteLen);  
//  
//  
//        //�����ܵ��׽��ֹ����� ��ɶ˿�  
//        if(CreateIoCompletionPort((HANDLE)accpSocket,CompletionPort,(DWORD)PerHandleData,0) == NULL)  
//        {  
//            cout<<"create complertion port error! errid:"<<GetLastError()<<endl;  
//            return 0;  
//        }  
//  
//        LPPER_IO_DATA PerIoData = (LPPER_IO_DATA)GlobalAlloc(GPTR,sizeof(PER_IO_DATA));;  
//        ChangeHandleData(PerIoData,OP_ACCEPT);  
//        SendHandleData(PerIoData,PerHandleData);  
//    }  
//    return 0;  
//}  
//  
//  
////�����̺߳���  
//DWORD WINAPI ServerWorkerThread(LPVOID lpParam)  
//{  
//    HANDLE CompletionPort = (HANDLE)lpParam;  
//    DWORD ByresTransferred;  
//    LPOVERLAPPED lpOverlapped = NULL;  
//    LPPER_HANDLE_DATA PerHandleData = NULL;  
//    //LPPER_IO_DATA PerHandleData = NULL;  
//    LPPER_IO_DATA PerIoData = NULL;  
//  
//    DWORD SendBytes;  
//    DWORD RecvBytes = 0;  
//    DWORD Flags = 0;  
//    BOOL bRet = FALSE;  
//  
//    while (TRUE)  
//    {  
//        bRet = GetQueuedCompletionStatus(CompletionPort,  
//                                         &ByresTransferred,                    //��I/O�������������ݵ��ֽ���  
//                                         (PULONG_PTR)&PerHandleData,        //���ڴ����֮������Completion��  
//                                         (LPOVERLAPPED*)&lpOverlapped,  
//                                          INFINITE);  
//  
//        if(!bRet)  
//        {  
//            closesocket(PerHandleData->Socket);  
//            ::GlobalFree(PerHandleData);  
//            ::GlobalFree(PerIoData);  
//            ::GlobalFree(lpOverlapped);  
//            cout<< "ʧȥ�ͻ������ӣ�������" << bRet <<endl;  
//            return 0;  
//        }  
//        PerIoData = (LPPER_IO_DATA)lpOverlapped;  
//  
//        //�ȼ��һ��,�����Ƿ��׽����д�����  
//        if(0 == ByresTransferred)  
//        {  
//            if(SOCKET_ERROR  == closesocket(PerHandleData->Socket)){  
//                cout<< "ByresTransferred SOCKET_ERROR err:" << GetLastError() << endl;  
//            }  
//            ::GlobalFree(PerHandleData);  
//            ::GlobalFree(PerIoData);  
//            cout<< "�������� error" <<endl;  
//            continue;  
//        }  
//  
//          
//        //�������  
//        switch(PerIoData->OperationType)  
//        {  
//        case OP_ACCEPT:  
//            if(ByresTransferred)  
//            {  
//                //��һ�����ӽ��ܵ�������  
//                ChangeHandleData(PerIoData,OP_ACCEPT);  
//                SendHandleData(PerIoData,PerHandleData);  
//            }  
//            else  
//            {  
//                //���ӳɹ�������û���ܵ�����������  
//                ChangeHandleData(PerIoData,OP_ACCEPT);  
//                SendHandleData(PerIoData,PerHandleData);  
//            }  
//            break;  
//        case OP_READ:  
//            cout<<"-----------------reader----------------------"<< endl;  
//            cout<<"�������ݵ��ֽ���:"<< ByresTransferred  <<endl;  
//            cout<<"��������Ϊ��"<<PerIoData->DataBuf.buf<< endl;  
//            cout<<"---------------------------------------------"<< endl;  
//            ChangeHandleData(PerIoData,OP_READ);  
//            SendHandleData(PerIoData,PerHandleData);  
//            break;  
//        case OP_WRITE:  
//            ChangeHandleData(PerIoData,OP_WRITE);  
//            SendHandleData(PerIoData,PerHandleData);  
//            break;  
//        default:  
//            //��������  
//            break;  
//              
//        }            
//    }  
//  
//    return 0 ;  
//}  