#pragma once

//����debugģʽ  �Ƿ����debug log
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

//��Ϊ���߳�ʹ�ã����Ե��������Ƕ��߳����
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
