#include <WinBase.h>

void DebugOut(char* out)
{
#if DEBUG
	OutputDebugString(out);
#endif	
}