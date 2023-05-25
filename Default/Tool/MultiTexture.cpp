#include "stdafx.h"
#include "MultiTexture.h"
#include "Device.h"


CMultiTexture::CMultiTexture()
{
}


CMultiTexture::~CMultiTexture()
{
	Release();
}

const TEXINFO * CMultiTexture::Get_Texture(const TCHAR * pStateKey, const int & iCount)
{
	auto	iter = find_if(m_mapMultiTex.begin(), m_mapMultiTex.end(), [&](auto& MyPair)->bool
	{
		if (pStateKey == MyPair.first)
			return true;

		return false;
	});

	if (iter == m_mapMultiTex.end())
		return nullptr;

	return iter->second[iCount];
}

HRESULT CMultiTexture::Insert_Texture(const TCHAR * pFilePath, const TCHAR * pStateKey , const int & iCount)
{

	TCHAR		szFullPath[MAX_PATH] = L"";

	for (int i = 0; i < iCount; ++i)
	{
		swprintf_s(szFullPath, pFilePath, i);

		TEXINFO* pTexInfo = new TEXINFO;
		ZeroMemory(pTexInfo, sizeof(TEXINFO));

		if (FAILED(D3DXGetImageInfoFromFile(szFullPath, &(pTexInfo->tImgInfo))))
		{
			Safe_Delete(pTexInfo);
			AfxMessageBox(szFullPath);
			return E_FAIL;
		}

		if (FAILED(D3DXCreateTextureFromFileEx(CDevice::Get_Instance()->Get_Device(),
			szFullPath,
			pTexInfo->tImgInfo.Width,
			pTexInfo->tImgInfo.Height,
			pTexInfo->tImgInfo.MipLevels,
			0,	// D3DUSAGE_RENDERTARGET : ����Ÿ�� �� �ؽ�ó		
			pTexInfo->tImgInfo.Format,
			D3DPOOL_MANAGED,
			D3DX_DEFAULT,  // �̹��� ���͸� ���, �̹��� Ȯ��, ��� �ȼ��� ���� ó���� ���� �ɼ�
			D3DX_DEFAULT,  // �Ӹ� �̹��� ���͸� ���
			0,				// ������ ����, 0�� ������ �÷�Ű ��ȿȭ
			nullptr,		// �̹��� ���� ���(m_pTexInfo->tImgInfo ���� ������ �̹� ä����)
			nullptr,		// �ȷ�Ʈ �̹��� ó��
			&(pTexInfo->pTexture))))
		{
			Safe_Delete(pTexInfo);
			AfxMessageBox(L"Multi Texture Failed");
			return E_FAIL;
		}

		m_mapMultiTex[pStateKey].push_back(pTexInfo);
	}
	
	return S_OK;
}

void CMultiTexture::Release(void)
{
	for_each(m_mapMultiTex.begin(), m_mapMultiTex.end(), [](auto& MyPair)
	{
		for_each(MyPair.second.begin(), MyPair.second.end(), [](auto& iter)
		{
			iter->pTexture->Release();
			Safe_Delete(iter);
		});
		MyPair.second.clear();
	});

	m_mapMultiTex.clear();



}
