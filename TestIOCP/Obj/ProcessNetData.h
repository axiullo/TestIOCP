#pragma once

//��Ϊ���߳�ʹ�ã����Ե��������Ƕ��߳����

class ProcessNetData
{
public:
	static ProcessNetData& ProcessNetDataIns();
	~ProcessNetData(void);

private:
	ProcessNetData(void);
	ProcessNetData(const ProcessNetData& );
	ProcessNetData& operator = (const ProcessNetData&);


private:
};
