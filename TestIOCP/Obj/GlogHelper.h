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

//因为单线程使用，所以单例不考虑多线程情况
class GlogHelper
{
public:
	static GlogHelper& GetInstance();
	~GlogHelper();

	void Test();

private:
	GlogHelper();
	GlogHelper(const GlogHelper&);
	GlogHelper& operator = (const GlogHelper&);

private:
	void SetLogDir();

private:
	string m_strLogDir;
};
