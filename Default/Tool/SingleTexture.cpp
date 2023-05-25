#include "stdafx.h"
#include "SingleTexture.h"
#include "Device.h"

CSingleTexture::CSingleTexture()
	: m_pTexInfo(nullptr)
{
}


CSingleTexture::~CSingleTexture()
{
	Release();
}



HRESULT CSingleTexture::Insert_Texture(const TCHAR * pFilePath, const TCHAR * pStateKey , const int & iCount)
{
	m_pTexInfo = new TEXINFO;
	ZeroMemory(m_pTexInfo, sizeof(TEXINFO));

	// D3DXGetImageInfoFromFile : ������ �̹��� ���Ͽ� ���� ������ ������ �Լ�
	if (FAILED(D3DXGetImageInfoFromFile(pFilePath, &(m_pTexInfo->tImgInfo))))
	{
		Safe_Delete(m_pTexInfo);
		AfxMessageBox(L"D3DXGetImageInfoFromFile Failed");
		return E_FAIL;
	}

	// D3DPOOL_DEFAULT : ���� ������ �޸𸮿� ����
	// D3DPOOL_MANAGED : dx�� ���� �ڿ��� �����ϸ� �׷��� �޸𸮸� ��������� ���� ���
	// D3DPOOL_SYSTEMMEM : ���� ����
	// D3DPOOL_SCRATCH : �ý��� �޸𸮸� ��������� dx ��ġ ���� ����

	if (FAILED(D3DXCreateTextureFromFileEx(CDevice::Get_Instance()->Get_Device(),
		pFilePath,
		m_pTexInfo->tImgInfo.Width,
		m_pTexInfo->tImgInfo.Height,
		m_pTexInfo->tImgInfo.MipLevels,
		0,	// D3DUSAGE_RENDERTARGET : ����Ÿ�� �� �ؽ�ó		
		m_pTexInfo->tImgInfo.Format,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,  // �̹��� ���͸� ���, �̹��� Ȯ��, ��� �ȼ��� ���� ó���� ���� �ɼ�
		D3DX_DEFAULT,  // �Ӹ� �̹��� ���͸� ���
		0,				// ������ ����, 0�� ������ �÷�Ű ��ȿȭ
		nullptr,		// �̹��� ���� ���(m_pTexInfo->tImgInfo ���� ������ �̹� ä����)
		nullptr,		// �ȷ�Ʈ �̹��� ó��
		&(m_pTexInfo->pTexture))))
	{
		Safe_Delete(m_pTexInfo);
		AfxMessageBox(L"Single Texture Failed");
		return E_FAIL;
	}

	return S_OK;
}

void CSingleTexture::Release(void)
{
	Safe_Release(m_pTexInfo->pTexture);
	Safe_Delete(m_pTexInfo);
}
