#include "stdafx.h"
#include "Device.h"

IMPLEMENT_SINGLETON(CDevice)

CDevice::CDevice()
	: m_pSDK(nullptr), m_pDevice(nullptr), m_pSprite(nullptr)
{
}


CDevice::~CDevice()
{
	Release();
}

HRESULT CDevice::Initialize(void)
{
	// 1��
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);

	// 2��
	D3DCAPS9		DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	// GetDeviceCaps : ��ġ�� ���� ����(���� ����)�� ������ �Լ�
	// D3DADAPTER_DEFAULT : ������ �������� �⺻ �׷��� ī�带 �ǹ�
	
	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
	{
		AfxMessageBox(L"GetDeviceCaps Failed");
		return E_FAIL;
	}

	DWORD	vp = 0;	// ���ؽ� ���μ��� = ���� ��ȯ + ���� ����

	if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING;

	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	D3DPRESENT_PARAMETERS		d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	Set_Parameters(d3dpp);

	// 3��
	// CreateDevice : ��ġ�� ������ ��ü ����
	// CreateDevice(� �׷��� ī�带 ������ ���ΰ�, � ������ ��ġ�� ������ ���ΰ�, ��ġ�� ����� ������, ���� ���, ��� ȯ��, ��ġ ���� ��ü ����)
	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd, vp, &d3dpp, &m_pDevice)))
	{
		AfxMessageBox(L"CreateDevice Failed");
		return E_FAIL;
	}

	if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
	{
		AfxMessageBox(L"Create Sprite Failed");
		return E_FAIL;
	}

	D3DXFONT_DESCW		tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));

	tFontInfo.Height = 20;
	tFontInfo.Width = 10;
	tFontInfo.Weight = FW_HEAVY;
	tFontInfo.CharSet = HANGEUL_CHARSET;
	lstrcpy(tFontInfo.FaceName, L"�ü�");
	
	if (FAILED(D3DXCreateFontIndirect(m_pDevice, &tFontInfo, &m_pFont)))
	{
		AfxMessageBox(L"Create Font Failed");
		return E_FAIL;
	}


	return S_OK;
}

void CDevice::Render_Begin(void)
{
	m_pDevice->Clear(0,			// Rect �迭�� ���� ����
					nullptr,	// Rect�� ������ �迭�� ù �ּ�, nullptr�ΰ�� ��ü ������ ����
					D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, // ���ٽ�,����, Ÿ�� ���� ��� ���ڴٴ� �ǹ�
					D3DCOLOR_ARGB(255, 125, 125, 125), // �ĸ� ���۸� ���� ä�� ����
					1.f, // ���� ���� �ʱ�ȭ ��
					0); // ���ٽ� ���� �ʱ�ȭ ��

	// ���⼭���� �ĸ� ���ۿ� �׸��� ����
	m_pDevice->BeginScene();

	// 2D �̹����� �׸� �� �ֵ��� ��ġ�� �غ�(������ �ɼ�)
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

}

void CDevice::Render_End(HWND hWnd)
{
	m_pSprite->End();
	m_pDevice->EndScene();

	m_pDevice->Present(NULL, NULL, hWnd, NULL);
	// 1, 2�� : ���� ü���� �ɼ��� D3DSWAPEFFECT_COPY�� ���� ����, �� �� ��Ʈ�� �ּҸ� ���� 
	// 3 : ��� ��� ������ �ڵ� 
	// 4�� : D3DSWAPEFFECT_COPY�� ���� ����

}

void CDevice::Release(void)
{
	Safe_Release(m_pFont);
	Safe_Release(m_pSprite);
	Safe_Release(m_pDevice);
	Safe_Release(m_pSDK);
}

void CDevice::Set_Parameters(D3DPRESENT_PARAMETERS & d3dpp)
{
	d3dpp.BackBufferWidth = WINCX;
	d3dpp.BackBufferHeight = WINCY;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	// D3DSWAPEFFECT_DISCARD : ����ü�� ���
	// D3DSWAPEFFECT_FLIP    : ���� �ϳ��� �������鼭 ����ϴ� ���
	// D3DSWAPEFFECT_COPY    : ���� ���۸��� ������ ���� ���
	
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	
	d3dpp.hDeviceWindow = g_hWnd;
	
	// â ���� ������ ���ΰ� ��üȭ�� ���� ������ ���ΰ�
	d3dpp.Windowed = TRUE;	// â ��� ����
		
	// DX�� �ڵ������� ���� / ���ٽ� ���۸� ����� �����ϱ� ���ϴ� ����
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	// ��üȭ�� ����� ��츸 ����ϴ� ��� ���� �ɼ�

	// ��üȭ�� ��� �ÿ� ���� ����� ������� ���� ������ ����
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

	// D3DPRESENT_INTERVAL_IMMEDIATE : ������� �ÿ��� ������ ������� ���� �� ��� �ÿ�
	// D3DPRESENT_INTERVAL_DEFAULT : ������ ������ DX�� �˾Ƽ� ����, ���� ����� ������� ����

	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}
