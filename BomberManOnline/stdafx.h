
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <iostream>
#include <string>


#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��


const int WINDOW_WIDTH = 1280, WINDOW_HEIGHT = 720;
const int GRID_WIDTH=30, GRID_HEIGHT=30;
const int GRIDNUM_WIDTH = 30, GRIDNUM_HEIGHT = 20;
const int SPRITE_WIDTH = 32, SPRITE_HEIGHT = 32;

const int MAP_WIDTH = GRID_WIDTH * GRIDNUM_WIDTH;
const int MAP_HEIGHT = GRID_HEIGHT *GRIDNUM_HEIGHT;

const int PADDING = 5;

const int MAX_PLAYER = 4;

const int STOP=-1, DOWN=0, LEFT=1, RIGHT=2, UP=3;
const CPoint DIRECT_VEC[]={CPoint(0,1), CPoint(-1,0), CPoint(1,0), CPoint(0,-1)};

const int TIMER_RENDER=1;
const int MAX_FPS = 60;

const float DEFAULT_BOMBTIME = 1000;
const int DEFAULT_BOMBPOWER = 5;
const int DEFAULT_FIRETIME = 300;

void OutputDebugPrintf(const char* strOutputString,...);
