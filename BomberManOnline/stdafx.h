
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <iostream>
#include <string>


#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持


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
