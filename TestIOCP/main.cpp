#include <stdio.h>
#include <tchar.h>

#include "IOCPNet.h"
#include "AcceptExExample.h"

#pragma  comment(lib, "ws2_32.lib")

int main()
{
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
			if(MessageBox(NULL, _T("ÊÇ·ñ¹Ø±Õ"), _T("¹Ø±Õ"), MB_OKCANCEL) == IDCANCEL )
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