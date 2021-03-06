
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#define _WIN32_WINNT 0x0502

//#include <afxwin.h>
#include <winsock2.h>
#include <windows.h>
#include <windowsx.h>
#include <atlimage.h>
#include <iostream>
#include <string>
#include <SDKDDKVer.h>
#include "mmsystem.h"
#include <cassert>
#include <atlconv.h>
#include <fstream>
#include <sstream>

#include <stdlib.h>


#pragma comment(lib, "winmm.lib")
#pragma comment(lib,"ws2_32.lib")


#define ASSERT(x) {if(!(x)) _asm{int 0x03}}
const int INF = 1<<30;

const float WINDOW_WIDTH = 1280, WINDOW_HEIGHT = 720;
const float GRID_WIDTH=30, GRID_HEIGHT=30;
const int GRIDNUM_WIDTH = 30, GRIDNUM_HEIGHT = 20;
const float SPRITE_WIDTH = 32, SPRITE_HEIGHT = 32;
const float ITEM_WIDTH = 30, ITEM_HEIGHT = 30;

const float MAP_WIDTH = GRID_WIDTH * GRIDNUM_WIDTH;
const float MAP_HEIGHT = GRID_HEIGHT *GRIDNUM_HEIGHT;

const int PADDING = 0;

const int MAX_PLAYER = 4;
const int MAX_ITEMS = 6;

const int STOP=-1, DOWN=0, LEFT=1, RIGHT=2, UP=3;
const CPoint DIRECT_VEC[]={CPoint(0,1), CPoint(-1,0), CPoint(1,0), CPoint(0,-1)};

const float DEFAULT_WRAPTIME = 3000;
const float DEFAULT_BOMBTIME = 2000;
const int DEFAULT_BOMBPOWER = 5;
const int DEFAULT_FIRETIME = 300;
const float DEFAULT_SPEED = 0.2f;
const float DELTA_SPEED = 0.02f;
const float DART_SPEED = 0.4;
const float SLOW_SPEED = 0.1;
const float DEFAULT_TRANSTIME=10000;
const float TRANS_BEGINTIME=1000;
const float TRANS_ENDTIME = 1000;

const float QUICK_SPEED = 0.35;
const int KICK_DISTANCE=10;
const float KICK_SINGLEFLYTIME=50;

const int LOGIN_MAX_BUTTON = 6;
const int LOGIN_MAX_ICON = 3;

const int LOBBY_MAX_BUTTON = 13;
const int LOBBY_MAX_ICON = 3;

const int ROOM_MAX_BUTTON = 17;
const int ROOM_MAX_ICON = 3;

const int GAME_MAX_BUTTON = 0;
const int GAME_MAX_ICON = 0;

const int MAX_BUTTON = 20;
const int MAX_SCENE = 4;

const int MENU_HEIGHT = 37;
const int MENU_WIDTH = 17;

const int MAIN_VERSION = 1;
const int MINOR_VERSION = 0;

const int MAX_CHAT_HISTORY = 100;

void OutputDebugPrintf(const char* strOutputString,...);
