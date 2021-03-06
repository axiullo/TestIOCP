#include <stdio.h>
#include <tchar.h>

#include "Iocp/IOCPNet.h"
#include "AcceptExExample.h"

#include "Utils.h"
#include "GlogHelper.h"
#include "ProcessNetData.h"

#pragma  comment(lib, "ws2_32.lib")

int main()
{
	GlogHelper::GetInstance();
	ProcessNetData::ProcessNetDataIns();

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