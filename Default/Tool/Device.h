#pragma once

#include "Include.h"

class CDevice
{
	DECLARE_SINGLETON(CDevice)

private:
	CDevice();
	~CDevice();

	// 1. ��ġ�� ������ ��ü ����
	// 2. ��ġ ����(���� ���� �ľ�)
	// 3. ��ġ�� ������ ��ü ����(����̽� ��ü)

public:
	LPDIRECT3DDEVICE9		Get_Device(void) { return m_pDevice; }
	LPD3DXSPRITE			Get_Sprite(void) { return m_pSprite; }
	LPD3DXFONT				Get_Font(void) { return m_pFont; }

public:
	HRESULT		Initialize(void);

	void		Render_Begin(void);
	void		Render_End(HWND hWnd = nullptr);

	void		Release(void);

private:
	void		Set_Parameters(D3DPRESENT_PARAMETERS& d3dpp);

private:
	LPDIRECT3D9			m_pSDK;		// ��ġ�� ������ ��ü ����
	LPDIRECT3DDEVICE9	m_pDevice;	// ��ġ�� ������ ��ü ����(����̽� ��ü), �׷��� ��ġ�� �����ϴ� ��ü ������
	LPD3DXSPRITE		m_pSprite;	// dx �󿡼� 2D �̹����� ������ִ� ��ü
	LPD3DXFONT			m_pFont;	

};

