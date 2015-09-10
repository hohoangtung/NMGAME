
/*
	call __debugoutput to print the value, the string to output console
	usefull when debug and test.
	example:
	#if _DEBUG		// macro _DEBUG is created automatically if you chose debug on target build. 
	__debugoutput(11);
	__debugoutput("test string");
	__debugoutput(local_var);
	#endif
	__debugoutput should be contained by #if and #endif for no error and high performence in release version.
*/
#ifndef __DEBUG_H__
#define __DEBUG_H__
#include <string>
#include <debugapi.h>	// for OutputDebugString
using namespace std;

template <typename T>
// T type should in:
// int, unsigned int, long, unsigned long,
// _Longlong, _ULonglong, long double, double, float
void __debugoutput(T var)
{
#if UNICODE
	wstring str = to_wstring(var);
	OutputDebugStringW(str.c_str());
	OutputDebugStringW(L"\n");
	// reference to_wstring : http://www.cplusplus.com/reference/string/to_wstring/
#else
	string str = to_string(var);
	OutputDebugStringA(str);
	OutputDebugStringA("\n");
	// reference to_string : http://www.cplusplus.com/reference/string/to_string/
#endif // UNICOE
}

#endif // !__DEBUG_H__
