
// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// BomberManOnline.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

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