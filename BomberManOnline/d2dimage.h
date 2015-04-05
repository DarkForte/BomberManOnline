#pragma once
#include "stdafx.h"
#include "D2D1Header.h"

//image
class _declspec(dllexport) CD2DImage 
{
	//加载标志
private:
	bool					m_bLoad;				//是否加载
	ID2D1Bitmap	*			m_pID2D1Bitmap;			//位图标志

	//函数定义
public:
	//构造函数
	CD2DImage();
	//析构函数
	virtual ~CD2DImage();

	//状态函数
public:
	//是否加载
	bool IsValid();
	//获取宽度
	float GetWidth();
	//获取高度
	float GetHeight();

	//管理函数
public:
	//销毁图片
	void Release();
	//加载图片
	HRESULT Load(IWICImagingFactory * pIWICFactory,ID2D1RenderTarget* pd2dDevice,LPCTSTR pszResource);

	//绘画函数
public:
	//绘画图像
	HRESULT DrawImage(ID2D1RenderTarget * pd2dDevice, float fXPos, float fYPos,D2D_SIZE_F WindowSize,float fOpacity=1.0f);
	//绘画图像
	HRESULT DrawImage(ID2D1RenderTarget * pd2dDevice, float fXPos, float fYPos,float fXScale=1.0f,float fYScale=1.0f,float fOpacity=1.0f);
	//绘画图像
	HRESULT DrawImage(ID2D1RenderTarget * pd2dDevice, float fXDest, float fYDest, float fDestWidth, float fDestHeight, float fXScr, float fYSrc,float fXScale=1.0f,float fYScale=1.0f,float fOpacity=1.0f);

	//操作重载
public:
	//指针重载
	//ID2D1Bitmap * operator->() { return GetImage(); }
	//获取对象
	ID2D1Bitmap * GetImage() { return m_pID2D1Bitmap; }

};