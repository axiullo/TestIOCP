//// IocpModel.cpp : 定义控制台应用程序的入口点。  
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
//        根据传进来的state，来进行改变下一步的IO操作 
//    Parameter： 
//        lpPerIoContext:上一个IO操作的结果 
//        state           上一个IO操作的状态 
//    Return Value: 
//        返回值为空 
//**/  
//    if(OP_READ == state)  
//    {  
//        //完成了读取客户端数据，并且返回数据  
//        lpPerIoContext->OperationType = OP_WRITE;  
//        ZeroMemory(&(lpPerIoContext->Overlapped),sizeof(OVERLAPPED));  
//        //返回数据给客户端  
//        char *sendClientData = "sendClientData";  
//        lpPerIoContext->DataBuf.buf = sendClientData;  
//        lpPerIoContext->DataBuf.len = BUFFER_SIZE;  
//    }  
//    if(OP_WRITE == state)  
//    {      
//        //上一个IO数据发送客户端完成，结束就用 OP_END  
//        //继续接受，叫类型设置为OP_READ,注意清空下 buff  
//        //继续发送，将数据设置为OP_WRITE  
//        lpPerIoContext->OperationType = OP_READ;  
//        lpPerIoContext->DataBuf.buf = lpPerIoContext->buffer;  
//        lpPerIoContext->DataBuf.len = BUFFER_SIZE;  
//    }  
//    if(OP_ACCEPT == state)  
//    {  
//        //立起来的连接，把句柄设为读类型  
//        lpPerIoContext->OperationType = OP_READ;  
//        ZeroMemory(&(lpPerIoContext->Overlapped),sizeof(OVERLAPPED));  
//        ZeroMemory(&(lpPerIoContext->DataBuf),sizeof(lpPerIoContext->DataBuf));  
//        lpPerIoContext->DataBuf.buf = lpPerIoContext->buffer;  
//        lpPerIoContext->DataBuf.len = BUFFER_SIZE;  
//        cout << "数据包操作类型被改为 : OP_READ"<<endl;  
//    }  
//}  
//  
//void SendHandleData(LPPER_IO_DATA lpPerIoContext,LPPER_HANDLE_DATA perHandData)  
//{  
//    /** 
//        根据lpPerIoContext对象OperationType来进行下一步的操作 
//    **/  
//    DWORD dwIosize = 0;  
//    int nResult  =0;  
//    DWORD RecvBytes = 0;  
//    DWORD nFlag = 0;  
//    if(lpPerIoContext->OperationType  == OP_WRITE)  
//    {  
//        /*发送数据到客户端*/  
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
//        //清空，准备下个I/O数据  
//        ZeroMemory(&(lpPerIoContext->Overlapped),sizeof(OVERLAPPED));  
//        //注意：这里清空下buffer，要不然数据会跟上一条粘在一起  
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
//    //创建一个IO完成端口  
//    CompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,0);  
//    if ( CompletionPort == INVALID_HANDLE_VALUE )   
//    {  
//        cout<<"完成端口创建失败!!!!"<<endl;  
//        return 0;  
//    }  
//  
//    //创建IOCP工作线程(根据CPU个数 * 2)  
//    for(int i = 0;i< Systeminfo.dwNumberOfProcessors * 2 ; ++ i)  
//    {  
//        //创建线程  
//        ThreadHanle = CreateThread(NULL,0,ServerWorkerThread,CompletionPort,0,NULL);  
//        CloseHandle(ThreadHanle);  
//    }  
//  
//    //4. 创建一个监听套接字，  
//    ListenSock = WSASocket(AF_INET,SOCK_STREAM,0,NULL,0,WSA_FLAG_OVERLAPPED);  
//    if(ListenSock == INVALID_SOCKET)  
//    {  
//        cout<<"套接字创建 失败!!"<<endl;  
//        return 0;  
//    }  
//  
//    InternetAddr.sin_family = AF_INET;  
//    InternetAddr.sin_port = htons(10086);  
//    InternetAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  
//    if(SOCKET_ERROR == bind(ListenSock,(SOCKADDR*)&InternetAddr,sizeof(InternetAddr)))  
//    {  
//        cout<<"服务器绑定地址信息 失败!! errid:"<<GetLastError()<<endl;  
//        closesocket(ListenSock);  
//        return 0;  
//    }  
//    int ret = listen(ListenSock, 5);  
//    if (ret == SOCKET_ERROR)  
//    {  
//        cout<<"监听套接字失败!! errid:"<<GetLastError()<<endl;  
//        closesocket(ListenSock);  
//        return 0;  
//    }  
//  
//    while(TRUE)  
//    {  
//        //接受连接  
//        RemoteLen = sizeof(saRemote);  
//        //accpSocket = WSAAccept(ListenSock,NULL,NULL,NULL,0);  
//        accpSocket = accept(ListenSock, (SOCKADDR*)&saRemote, &RemoteLen);  
//        if(SOCKET_ERROR == accpSocket)  
//        {  
//            cout<<"接受套接字错误!! errid:"<<GetLastError()<<endl;  
//            closesocket(accpSocket);  
//            return 0;  
//        }  
//  
//        //创建用来和套接字关联句柄信息结构  
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
//        //将接受的套接字关联到 完成端口  
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
////创建线程函数  
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
//                                         &ByresTransferred,                    //的I/O操作所传送数据的字节数  
//                                         (PULONG_PTR)&PerHandleData,        //用于存放与之关联的Completion键  
//                                         (LPOVERLAPPED*)&lpOverlapped,  
//                                          INFINITE);  
//  
//        if(!bRet)  
//        {  
//            closesocket(PerHandleData->Socket);  
//            ::GlobalFree(PerHandleData);  
//            ::GlobalFree(PerIoData);  
//            ::GlobalFree(lpOverlapped);  
//            cout<< "失去客户端连接！！！！" << bRet <<endl;  
//            return 0;  
//        }  
//        PerIoData = (LPPER_IO_DATA)lpOverlapped;  
//  
//        //先检查一下,看看是否套接字有错误发生  
//        if(0 == ByresTransferred)  
//        {  
//            if(SOCKET_ERROR  == closesocket(PerHandleData->Socket)){  
//                cout<< "ByresTransferred SOCKET_ERROR err:" << GetLastError() << endl;  
//            }  
//            ::GlobalFree(PerHandleData);  
//            ::GlobalFree(PerIoData);  
//            cout<< "发生错误！ error" <<endl;  
//            continue;  
//        }  
//  
//          
//        //处理操作  
//        switch(PerIoData->OperationType)  
//        {  
//        case OP_ACCEPT:  
//            if(ByresTransferred)  
//            {  
//                //第一次连接接受到的数据  
//                ChangeHandleData(PerIoData,OP_ACCEPT);  
//                SendHandleData(PerIoData,PerHandleData);  
//            }  
//            else  
//            {  
//                //连接成功，数据没接受到，继续接受  
//                ChangeHandleData(PerIoData,OP_ACCEPT);  
//                SendHandleData(PerIoData,PerHandleData);  
//            }  
//            break;  
//        case OP_READ:  
//            cout<<"-----------------reader----------------------"<< endl;  
//            cout<<"传送数据的字节数:"<< ByresTransferred  <<endl;  
//            cout<<"接受数据为："<<PerIoData->DataBuf.buf<< endl;  
//            cout<<"---------------------------------------------"<< endl;  
//            ChangeHandleData(PerIoData,OP_READ);  
//            SendHandleData(PerIoData,PerHandleData);  
//            break;  
//        case OP_WRITE:  
//            ChangeHandleData(PerIoData,OP_WRITE);  
//            SendHandleData(PerIoData,PerHandleData);  
//            break;  
//        default:  
//            //其他操作  
//            break;  
//              
//        }            
//    }  
//  
//    return 0 ;  
//}  