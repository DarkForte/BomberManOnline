
// ChildView.cpp : CChildView 类的实现
//

#include "stdafx.h"
#include "BomberManOnline.h"
#include "BomberManOnlineView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CImage map_back;

void TransparentPNG(CImage *png)
{
	for(int i = 0; i <png->GetWidth(); i++)
	{
		for(int j = 0; j <png->GetHeight(); j++)
		{
			unsigned char* pucColor = reinterpret_cast<unsigned char*>(png->GetPixelAddress(i , j));
			pucColor[0] = pucColor[0] *pucColor[3] / 255;
			pucColor[1] = pucColor[1] *pucColor[3] / 255;
			pucColor[2] = pucColor[2] *pucColor[3] / 255;
		}
	}
}

void CBomberManOnlineView::Init()
{
	game_state = LOBBY;

	map_back.Load(L"pic\\meadow.png");

	return;
}

CBomberManOnlineView::CBomberManOnlineView()
{
	
}

CBomberManOnlineView::~CBomberManOnlineView()
{
}


BEGIN_MESSAGE_MAP(CBomberManOnlineView, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



// CChildView 消息处理程序

BOOL CBomberManOnlineView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);


	Init();
	return TRUE;
}

void CBomberManOnlineView::LobbyRender(CDC *pDC)
{
	pDC->TextOut(100,100,L"点击开始");

	return;
}

void CBomberManOnlineView::GameRender(CDC *pDC)
{
	map_back.Draw(*pDC, game.map_area);

	return;
}

void CBomberManOnlineView::OnPaint() 
{
	CDC *pDC = GetDC();
	
	// TODO: 在此处添加消息处理程序代码
	GetClientRect(&client_rect);
	cacheDC.CreateCompatibleDC(NULL);
	cache_bitmap.CreateCompatibleBitmap(pDC, client_rect.Width(), client_rect.Height());
	cacheDC.SelectObject(&cache_bitmap);
	
	if(game_state == LOBBY)
	{
		LobbyRender(&cacheDC);
	}
	else if(game_state == INGAME)
	{
		GameRender(&cacheDC);
	}


	pDC->BitBlt(0, 0, client_rect.Width(), client_rect.Height(), &cacheDC,0,0,SRCCOPY);  

	ValidateRect(client_rect);
	cache_bitmap.DeleteObject();
	cacheDC.DeleteDC();

	// 不要为绘制消息而调用 CWnd::OnPaint()
}



void CBomberManOnlineView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(game_state == LOBBY)
	{
		game_state = INGAME;
		Invalidate();
	}


	CWnd::OnLButtonDown(nFlags, point);
}
