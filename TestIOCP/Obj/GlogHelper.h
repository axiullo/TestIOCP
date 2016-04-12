#pragma once

//设置debug模式  是否输出debug log
#ifdef _DEBUG
#pragma comment(lib, "./DebugLib/libglog.lib")
#else
#define NDEBUG
#pragma comment(lib, "./ReleaseLib/libglog.lib")
#endif

#define GLOG_NO_ABBREVIATED_SEVERITIES  
#include "glog/logging.h"

#include <Windows.h>
#include <iostream>
#include <string>
using std::string;
using std::endl;

void FatalFunc();

class GlogHelper
{
public:
	static GlogHelper* GetInstance();

	void Test();

private:
	GlogHelper();
	~GlogHelper();

private:
	void SetLogDir();

private:
	string m_strLogDir;
};
