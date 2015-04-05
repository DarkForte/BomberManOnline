#pragma once
#include "stdafx.h"
#include "D2D1Header.h"

class CDirect2DMFCBase
{

private:
	HWND m_hwnd;
	ID2D1Factory* m_pDirect2dFactory;
	ID2D1HwndRenderTarget* m_pRenderTarget;
	IWICImagingFactory* m_pWICImagingFactory;
	IDWriteFactory* m_pWriteFactory;

public:
	CDirect2DMFCBase(void);
	~CDirect2DMFCBase(void);

	// call methods for instantiating drawing resources
	HRESULT InitializeD2D();

	ID2D1HwndRenderTarget* GetRenderTarget();
	IWICImagingFactory* GetWICImagingFactory();
	IDWriteFactory* GetWriteFactory(){return m_pWriteFactory;}
	void SetHwnd(HWND v){m_hwnd = v;}

protected:
	// Initialize device-independent resources.
	HRESULT CreateDeviceIndependentResources();

	// Initialize device-dependent resources.
	virtual HRESULT CreateDeviceResources();

	// Release device-dependent resource.
	void DiscardDeviceResources();

	// Draw content.
	virtual HRESULT OnRender();

	// Resize the render target.
	void OnResize(UINT width,UINT height);
};

