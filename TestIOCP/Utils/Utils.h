#pragma once
#ifndef __UTILS__
#define __UTILS__

#include <string>
using std::string;

/** debug�����Ϣ */
void DebugOut(char* out);

/** ���ִ��Ŀ¼ */
const string& GetModuleDir();

/** ִ��Ŀ¼�´���Ŀ¼ */
string CreateDir(const char* szDirName);

#endif