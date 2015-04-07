#include "stdafx.h"
#include "D2D1Header.h"
#include "d2dimage.h"


//���캯��
CD2DImage::CD2DImage()
{
	m_bLoad=false;
	m_pID2D1Bitmap=NULL;
}

//��������
CD2DImage::~CD2DImage()
{
	Release();
}

//�Ƿ����
bool CD2DImage::IsValid()
{
	return m_bLoad&&(m_pID2D1Bitmap);
}

//��ȡ���
float CD2DImage::GetWidth()
{
	//�����ж�
	if (!IsValid())
		return 0;

	//��ȡ���
	return m_pID2D1Bitmap->GetSize().width;
}

//��ȡ�߶�
float CD2DImage::GetHeight()
{
	//�����ж�
	if (!IsValid())
		return 0;

	//��ȡ���
	return m_pID2D1Bitmap->GetSize().height;
}

//����ͼƬ
void CD2DImage::Release()
{
	m_bLoad=false;
	SafeRelease(&m_pID2D1Bitmap);
}

//����ͼƬ
HRESULT CD2DImage::Load(IWICImagingFactory * pIWICFactory,ID2D1RenderTarget * pd2dDevice,LPCTSTR pszResource)
{
	//�ظ�����
	Release();

    HRESULT hr=S_OK;
	IWICStream *pStream=NULL;
	IWICBitmapScaler *pScaler=NULL;
    IWICBitmapDecoder *pDecoder=NULL;
    IWICBitmapFrameDecode *pSource=NULL;
    IWICFormatConverter *pConverter=NULL;
    
    hr = pIWICFactory->CreateDecoderFromFilename(
        pszResource,
        NULL,
        GENERIC_READ,
        WICDecodeMetadataCacheOnLoad,
        &pDecoder
        );

    if (SUCCEEDED(hr))
    {
        // Create the initial frame.
        hr = pDecoder->GetFrame(0, &pSource);
    }

    if (SUCCEEDED(hr))
    {
        hr = pIWICFactory->CreateFormatConverter(&pConverter);
    }

    if (SUCCEEDED(hr))
    {
        hr = pConverter->Initialize(
            pSource,
            GUID_WICPixelFormat32bppPBGRA,
            WICBitmapDitherTypeNone,
            NULL,
            0.f,
            WICBitmapPaletteTypeMedianCut
            );
    }

    if (SUCCEEDED(hr))
    {
        // Create a Direct2D bitmap from the WIC bitmap.
        hr = pd2dDevice->CreateBitmapFromWicBitmap(
            pConverter,
            NULL,
            &m_pID2D1Bitmap
            );
    }

    SafeRelease(&pDecoder);
    SafeRelease(&pSource);
    SafeRelease(&pStream);
    SafeRelease(&pConverter);
    SafeRelease(&pScaler);

	if(SUCCEEDED(hr)) 
		m_bLoad=true;

	return hr;
}

//�滭ͼ��
HRESULT CD2DImage::DrawImage(ID2D1RenderTarget * pd2dDevice, float fXPos, float fYPos,D2D_SIZE_F WindowSize,float fOpacity)
{
	//״̬�ж�
	if(IsValid()==false) return E_FAIL;
	if(pd2dDevice==NULL) return E_FAIL;

	float fWidth=WindowSize.width;
	float fHeight=WindowSize.height;
	pd2dDevice->DrawBitmap( m_pID2D1Bitmap,D2D1::RectF(fXPos,fYPos,fXPos+fWidth,fYPos+fHeight) ,fOpacity);
	
	return S_OK;
}

//�滭ͼ��
HRESULT CD2DImage::DrawImage(ID2D1RenderTarget * pd2dDevice, float fXPos, float fYPos,float fXScale,float fYScale,float fOpacity)
{
	//״̬�ж�
	if(IsValid()==false) return E_FAIL;
	if(pd2dDevice==NULL) return E_FAIL;

	float fWidth=GetWidth()*fXScale;
	float fHeight=GetHeight()*fYScale;
	pd2dDevice->DrawBitmap( m_pID2D1Bitmap,D2D1::RectF(fXPos,fYPos,fXPos+fWidth,fYPos+fHeight) ,fOpacity);
	
	return S_OK;
}

//�滭ͼ��
HRESULT CD2DImage::DrawImage(ID2D1RenderTarget * pd2dDevice, float fXPos, float fYPos, float fDestWidth, float fDestHeight, float fXScr, float fYSrc,float fXScale,float fYScale,float fOpacity)
{
	//״̬�ж�
	if(IsValid()==false) return E_FAIL;
	if(pd2dDevice==NULL) return E_FAIL;

	pd2dDevice->DrawBitmap( 
								m_pID2D1Bitmap, D2D1::RectF(fXPos,fYPos,fDestWidth+fXPos,fDestHeight+fYPos) ,
								fOpacity,D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
								D2D1::RectF(fXScr,fYSrc,fXScr+fDestWidth,fYSrc+fDestHeight)
							);
	
	return S_OK;
}