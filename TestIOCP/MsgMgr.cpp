#include "MsgMgr.h"

MsgMgr::MsgMgr(void)
{
}

MsgMgr::~MsgMgr(void)
{
}

MsgMgr MsgMgr::Instance()
{
	static MsgMgr ins;
	return ins;
}

void MsgMgr::ProcessData( const char* msg )
{

}
