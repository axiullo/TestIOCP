#include "GlogHelper.h"
#include "Utils.h"

/****************************
FLAGS_log_dir         日志输出目录 
FLAGS_v               自定义VLOG(m)时，m值小于此处设置值的语句才有输出 
FLAGS_max_log_size    每个日志文件最大大小（MB级别） 
FLAGS_minloglevel     输出日志的最小级别，即高于等于该级别的日志都将输出。
****************************/

GlogHelper::GlogHelper()
{
	m_strLogDir = "";

	m_strLogDir = CreateDir("log");

	// Initialize Google's logging library.
	//### 设置日志文件名中的"文件名"字段, 但是设置没有看出有用，所以这里先设为空
	google::InitGoogleLogging("");
	
	string tem = m_strLogDir + "/info_";
	// 设置日志路径  INFO WARNING ERROR FATAL  
	google::SetLogDestination(google::GLOG_INFO, tem.c_str());

	tem = m_strLogDir + "/warning_";
	google::SetLogDestination(google::GLOG_WARNING, tem.c_str());

	//设置文件名扩展部分
	google::SetLogFilenameExtension("IOCP_");

	//FLAGS_log_prefix = false;

	//将某级及以上级别的记录同时发送到stderr和日志文件. 严重等级INFO, WARNING, ERROR, FATAL 分别对应 0, 1, 2, 3.
	//FLAGS_stderrthreshold = 0; 	

	//google::InstallFailureFunction(&FatalFunc);

	//Test();
}

GlogHelper& GlogHelper::GetInstance()
{
	static GlogHelper ins;
	return ins;
}

void GlogHelper::Test()
{
	char str[20] = "hello log!";  
	LOG(INFO) << "Found " << google::COUNTER << endl;  
	LOG(INFO) << str;//<< " cookies";  
	LOG(WARNING) << "warning test";  // 会输出一个Warning日志  
	LOG(ERROR) << "error test";//会输出一个Error日志 
	LOG(INFO) << "sdklfjskldfjklsdjflksdjf";

	//test fatal and fatal func
	//LOG(FATAL) << "fatal func log ";

	DLOG(INFO) << "DEBUG -->Found cookies";
}

GlogHelper::~GlogHelper()
{
	google::ShutdownGoogleLogging();
}

void FatalFunc()
{
	printf("sdfsdfsdf");
}
