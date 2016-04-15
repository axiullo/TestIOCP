#pragma once

//因为单线程使用，所以单例不考虑多线程情况

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
