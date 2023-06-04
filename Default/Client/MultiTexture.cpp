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

TEXINFO * CMultiTexture::Get_Texture(const TCHAR * pStateKey, const int & iCount)
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

HRESULT CMultiTexture::Load_Texture(const wstring & wstrFilePath, const wstring & wstrStateKey, const int & iCount)
{
	auto iter_find = m_mapMultiTex.find(wstrStateKey);

	if (m_mapMultiTex.end() != iter_find)
		return E_FAIL;

	TCHAR szFilePath[MAX_STR] = L"";

	HRESULT hr  = 0;

	for (int i = 0; i < iCount; ++i)
	{
		TEXINFO* pInfo = new TEXINFO;
		ZeroMemory(pInfo, sizeof(TEXINFO));

		swprintf_s(szFilePath, wstrFilePath.c_str(), i);

		hr = D3DXGetImageInfoFromFile(szFilePath, &pInfo->tImgInfo);
		//if (FAILED(hr))
		//{
		//	ERR_MSG(L"Failed");
		//	return E_FAIL;
		//}

		hr = D3DXCreateTextureFromFileEx(
			CDevice::Get_Instance()->Get_Device(), // ��ġ
			szFilePath,
			pInfo->tImgInfo.Width,	// �̹��� ���� Size
			pInfo->tImgInfo.Height, // �̹��� ���� Size
			pInfo->tImgInfo.MipLevels,
			0,	// D3DUSAGE_RENDERTARGET : ����Ÿ�� �� �ؽ�ó		
			pInfo->tImgInfo.Format, // �̹��� �ȼ� ����
			D3DPOOL_MANAGED,		// �޸� ��� ���
			D3DX_DEFAULT,  // �̹��� ���͸� ���, �̹��� Ȯ��, ��� �ȼ��� ���� ó���� ���� �ɼ�
			D3DX_DEFAULT,  // �Ӹ� �̹��� ���͸� ���
			0,				// ������ ����, 0�� ������ �÷�Ű ��ȿȭ
			nullptr,		// �̹��� ���� ���(m_pTexInfo->tImgInfo ���� ������ �̹� ä����)
			nullptr,		// �ȷ�Ʈ �̹��� ó��
			&(pInfo->pTexture));
		//if (FAILED(hr))
		//{
		//	ERR_MSG(L"Failed");
		//	return E_FAIL;
		//}
		
		m_mapMultiTex[wstrStateKey].push_back(pInfo);
	}

	return S_OK;
}
