#include "stdafx.h"
#include "BomberManOnlineView.h"
using namespace std;

const wchar_t TAR_TITLE[]=L"BomberManOnline";
CBomberManOnlineView *application;


LRESULT CALLBACK WindProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	if(message == WM_PAINT)
	{
		application->OnRender();
	}
	else if(message == WM_KEYDOWN)
	{
		application->OnKeyDown(wParam);
	}
	else if(message == WM_KEYUP)
	{
		application->OnKeyUp(wParam);
	}

	else if(message == WM_LBUTTONDOWN)
	{
		int xPos = GET_X_LPARAM(lParam); 
		int yPos = GET_Y_LPARAM(lParam); 
		application->OnLButtonDown(CPoint(xPos, yPos));
	}
	else if (message == WM_LBUTTONUP)
	{
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);
		application->OnLButtonUp(CPoint(xPos, yPos));
	}
	else if (message == WM_MOUSEMOVE)
	{
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);
		application->OnLButtonMove(CPoint(xPos, yPos));
	}
	
	else if(message == WM_DESTROY)
	{
		PostQuitMessage(0);
	}
	else
	{
		return DefWindowProc(hwnd,message,wParam,lParam);
	}
		
}

int WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	WNDCLASSEX wndclass={0};
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW|CS_VREDRAW;
	wndclass.lpfnWndProc = WindProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = NULL;
	wndclass.hCursor = LoadCursor(NULL,IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = L"wndclass";

	//注册窗口

	if(!RegisterClassEx(&wndclass))
		return -1;

	HWND hwnd = CreateWindow(L"wndclass", TAR_TITLE, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH + MENU_WIDTH, WINDOW_HEIGHT + MENU_HEIGHT, NULL, NULL, hInstance, NULL);//第三个参数控制了窗口的样式，合集为WS_OVERLAPPEDWINDOW

	application = new CBomberManOnlineView();
	application->SetHwnd(hwnd);
	application->OnCreate();

	MoveWindow(hwnd, 250, 80, WINDOW_WIDTH + MENU_WIDTH, WINDOW_HEIGHT + MENU_HEIGHT, true);
	ShowWindow(hwnd,nShowCmd);
	UpdateWindow(hwnd);

	const int MAX_FPS = 60;
	
	MSG msg={0};
	float now_time;
	float last_time = timeGetTime();

	while(msg.message!=WM_QUIT)
	{
		if(PeekMessage(&msg,0,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		now_time = timeGetTime();
		if(now_time - last_time >= 1000/MAX_FPS)
		{
			//OutputDebugPrintf("%lf\n", now_time - last_time);
			application->Update(now_time - last_time);
			application->OnRender();
			last_time = now_time;
			
		}
	}

	application->OnDestroy();

	UnregisterClass(L"wndclass",wndclass.hInstance);

	//delete application;
	return 0;
}
