#include <stdio.h>
#include <tchar.h>

#include "Iocp/IOCPNet.h"
#include "AcceptExExample.h"

#define GLOG_NO_ABBREVIATED_SEVERITIES  
#include "glog/logging.h"

#pragma  comment(lib, "ws2_32.lib")

#ifdef DEBUG
#pragma comment(lib, "./DebugLib/libglog.lib")
#else
#pragma comment(lib, "./ReleaseLib/libglog.lib")
#endif

int main()
{
	char szPath[MAX_PATH];
	GetModuleFileName(NULL, szPath, MAX_PATH);

	string path = szPath;

	string subPath = path.substr(0, path.find_last_of('\\'));
	subPath += '\\';

	// Initialize Google's logging library.  
	google::InitGoogleLogging("");  
	google::SetLogDestination(google::GLOG_INFO, subPath.c_str());  
	// ������־·��  INFO WARNING ERROR FATAL  
	// ...  
	char str[20] = "hello log!";  
	LOG(INFO) << "Found " << google::COUNTER <<endl;  
	LOG(INFO) << str ;//<< " cookies";  
	LOG(WARNING) << "warning test";  // �����һ��Warning��־  
	LOG(ERROR) << "error test";//�����һ��Error��־  



	IOCPNet net;
	if(net.Start(10086, "127.0.0.1") < 0)
	{
		return -1;
	}

	int n = 1;
	while(n)
	{
		scanf("%d", &n);

		if (n == 0)
		{
			if(MessageBox(NULL, _T("�Ƿ�ر�"), _T("�ر�"), MB_OKCANCEL) == IDCANCEL )
			{
				n = 1;
			}
			
			net.End();
		}
	}

	printf("%d \n", net.GetThreadCloseNum());

	//AcceptExExample();

	return 0;
}