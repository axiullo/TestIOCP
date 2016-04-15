#include "GlogHelper.h"
#include "Utils.h"

/****************************
FLAGS_log_dir         ��־���Ŀ¼ 
FLAGS_v               �Զ���VLOG(m)ʱ��mֵС�ڴ˴�����ֵ����������� 
FLAGS_max_log_size    ÿ����־�ļ�����С��MB���� 
FLAGS_minloglevel     �����־����С���𣬼����ڵ��ڸü������־���������
****************************/

GlogHelper::GlogHelper()
{
	m_strLogDir = "";

	m_strLogDir = CreateDir("log");

	// Initialize Google's logging library.
	//### ������־�ļ����е�"�ļ���"�ֶ�, ��������û�п������ã�������������Ϊ��
	google::InitGoogleLogging("");
	
	string tem = m_strLogDir + "/info_";
	// ������־·��  INFO WARNING ERROR FATAL  
	google::SetLogDestination(google::GLOG_INFO, tem.c_str());

	tem = m_strLogDir + "/warning_";
	google::SetLogDestination(google::GLOG_WARNING, tem.c_str());

	//�����ļ�����չ����
	google::SetLogFilenameExtension("IOCP_");

	//FLAGS_log_prefix = false;

	//��ĳ�������ϼ���ļ�¼ͬʱ���͵�stderr����־�ļ�. ���صȼ�INFO, WARNING, ERROR, FATAL �ֱ��Ӧ 0, 1, 2, 3.
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
	LOG(WARNING) << "warning test";  // �����һ��Warning��־  
	LOG(ERROR) << "error test";//�����һ��Error��־ 
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
