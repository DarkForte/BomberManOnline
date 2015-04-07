#pragma once
#include "stdafx.h"
#include "D2D1Header.h"

//image
class _declspec(dllexport) CD2DImage 
{
	//���ر�־
private:
	bool					m_bLoad;				//�Ƿ����
	ID2D1Bitmap	*			m_pID2D1Bitmap;			//λͼ��־

	//��������
public:
	//���캯��
	CD2DImage();
	//��������
	virtual ~CD2DImage();

	//״̬����
public:
	//�Ƿ����
	bool IsValid();
	//��ȡ���
	float GetWidth();
	//��ȡ�߶�
	float GetHeight();

	//������
public:
	//����ͼƬ
	void Release();
	//����ͼƬ
	HRESULT Load(IWICImagingFactory * pIWICFactory,ID2D1RenderTarget* pd2dDevice,LPCTSTR pszResource);

	//�滭����
public:
	//�滭ͼ��
	HRESULT DrawImage(ID2D1RenderTarget * pd2dDevice, float fXPos, float fYPos,D2D_SIZE_F WindowSize,float fOpacity=1.0f);
	//�滭ͼ��
	HRESULT DrawImage(ID2D1RenderTarget * pd2dDevice, float fXPos, float fYPos,float fXScale=1.0f,float fYScale=1.0f,float fOpacity=1.0f);
	//�滭ͼ��
	HRESULT DrawImage(ID2D1RenderTarget * pd2dDevice, float fXDest, float fYDest, float fDestWidth, float fDestHeight, float fXScr, float fYSrc,float fXScale=1.0f,float fYScale=1.0f,float fOpacity=1.0f);

	//��������
public:
	//ָ������
	//ID2D1Bitmap * operator->() { return GetImage(); }
	//��ȡ����
	ID2D1Bitmap * GetImage() { return m_pID2D1Bitmap; }

};