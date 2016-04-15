#include "ProcessNetData.h"


ProcessNetData::ProcessNetData(void)
{
}

ProcessNetData::~ProcessNetData(void)
{
}

ProcessNetData& ProcessNetData::ProcessNetDataIns()
{
	static ProcessNetData ins;
	return ins;
}
