#pragma once
#ifndef __UTILS__
#define __UTILS__

#include <string>
using std::string;

/** debug输出信息 */
void DebugOut(char* out);

/** 获得执行目录 */
const string& GetModuleDir();

/** 执行目录下创建目录 */
string CreateDir(const char* szDirName);

#endif