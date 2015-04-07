#include "stdafx.h"
#include "Direct2DMFCBase.h"


CDirect2DMFCBase::CDirect2DMFCBase(void)
{
	m_hwnd= NULL;
	m_pDirect2dFactory = NULL;
	m_pRenderTarget = NULL;
	m_pWICImagingFactory = NULL;
	m_pWriteFactory = NULL;
}


CDirect2DMFCBase::~CDirect2DMFCBase(void)
{
	SafeRelease(&m_pDirect2dFactory);
	SafeRelease(&m_pRenderTarget);
	SafeRelease(&m_pWriteFactory);
	SafeRelease(&m_pWICImagingFactory);
}

HRESULT CDirect2DMFCBase::InitializeD2D()
{
	HRESULT hr;
	CoInitialize(NULL);
	// Initialize device-independent resources, such
	// as the Direct2D factory.
	hr = CreateDeviceIndependentResources();

	if (SUCCEEDED(hr))
	{

		// Because the CreateWindow function takes its size in pixels,
		// obtain the system DPI and use it to scale the window size.
		FLOAT dpiX, dpiY;

		// The factory returns the current system DPI. This is also the value it will use
		// to create its own windows.
		m_pDirect2dFactory->GetDesktopDpi(&dpiX, &dpiY);

	}

	return hr;
}

HRESULT CDirect2DMFCBase::CreateDeviceIndependentResources()
{
	HRESULT hr = S_OK;

	// Create a Direct2D factory.
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);

	hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IWICImagingFactory,
		reinterpret_cast<void **>(&m_pWICImagingFactory)
		);

	hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED, 
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&m_pWriteFactory)
		);
	return hr;
}

HRESULT CDirect2DMFCBase::CreateDeviceResources()
{
	HRESULT hr = S_OK;

	if (!m_pRenderTarget)
	{
		RECT rc;
		GetClientRect(m_hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

		// Create a Direct2D render target.
		hr = m_pDirect2dFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hwnd, size),
			&m_pRenderTarget
			);

	}

	return hr;
}

void CDirect2DMFCBase::DiscardDeviceResources()
{
	SafeRelease(&m_pRenderTarget);
}

HRESULT CDirect2DMFCBase::OnRender()
{
	HRESULT hr = S_OK;

	hr = CreateDeviceResources();
	if (SUCCEEDED(hr))
	{
		m_pRenderTarget->BeginDraw();

		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

		m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

		D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();

		hr = m_pRenderTarget->EndDraw();
	}

	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardDeviceResources();
	}

	return hr;
}

void CDirect2DMFCBase::OnResize(UINT width, UINT height)
{
	if (m_pRenderTarget)
	{
		// Note: This method can fail, but it's okay to ignore the
		// error here, because the error will be returned again
		// the next time EndDraw is called.
		m_pRenderTarget->Resize(D2D1::SizeU(width, height));
	}
}

IWICImagingFactory* CDirect2DMFCBase::GetWICImagingFactory()
{
	return m_pWICImagingFactory;
}

ID2D1HwndRenderTarget* CDirect2DMFCBase::GetRenderTarget()
{
	return m_pRenderTarget;
}


