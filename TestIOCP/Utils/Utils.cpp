#include "Utils.h"

#include <Windows.h>

void DebugOut( char* out )
{
#if DEBUG
		OutputDebugString(out);
#endif	
}

const string& GetModuleDir()
{
	static string strModuleDir = "";

	if (strModuleDir.length() <= 0)
	{
		char szPath[MAX_PATH];
		//获得执行路径
		GetModuleFileName(NULL, szPath, MAX_PATH);

		strModuleDir = szPath;

		strModuleDir = strModuleDir.substr(0, strModuleDir.find_last_of('\\'));
		strModuleDir += '\\';
	}

	return strModuleDir;
}

string CreateDir( const char* szDirName )
{
	const string rootDir = GetModuleDir();
	string strTarget = rootDir + szDirName;

	if (CreateDirectory(strTarget.c_str(), NULL) == 0)
	{
		if(ERROR_ALREADY_EXISTS != GetLastError())
		{
			exit(-1);
		}
	}

	//string commend = "md " + strTarget;
	//system(commend.c_str());

	strTarget += "\\";

	return strTarget;
}

