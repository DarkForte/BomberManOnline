
// stdafx.cpp : 只包括标准包含文件的源文件
// BomberManOnline.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

void OutputDebugPrintf(const char* strOutputString,...)
{
	char strBuffer[4096]={0};
	va_list vlArgs;
	va_start(vlArgs,strOutputString);
	_vsnprintf_s(strBuffer,sizeof(strBuffer)-1,strOutputString,vlArgs);
	//vsprintf(strBuffer,strOutputString,vlArgs);
	va_end(vlArgs);
	OutputDebugString(CA2W(strBuffer));
}