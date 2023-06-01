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
			0,	// D3DUSAGE_RENDERTARGET : 렌더타겟 용 텍스처		
			pTexInfo->tImgInfo.Format,
			D3DPOOL_MANAGED,
			D3DX_DEFAULT,  // 이미지 필터링 방식, 이미지 확대, 축소 픽셀에 대한 처리를 묻는 옵션
			D3DX_DEFAULT,  // 밉맵 이미지 필터링 방식
			0,				// 제거할 색상, 0을 넣으면 컬러키 무효화
			nullptr,		// 이미지 정보 기록(m_pTexInfo->tImgInfo 값을 위에서 이미 채웠음)
			nullptr,		// 팔레트 이미지 처리
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
			CDevice::Get_Instance()->Get_Device(), // 장치
			szFilePath,
			pInfo->tImgInfo.Width,	// 이미지 가로 Size
			pInfo->tImgInfo.Height, // 이미지 세로 Size
			pInfo->tImgInfo.MipLevels,
			0,	// D3DUSAGE_RENDERTARGET : 렌더타겟 용 텍스처		
			pInfo->tImgInfo.Format, // 이미지 픽셀 포멧
			D3DPOOL_MANAGED,		// 메모리 사용 방식
			D3DX_DEFAULT,  // 이미지 필터링 방식, 이미지 확대, 축소 픽셀에 대한 처리를 묻는 옵션
			D3DX_DEFAULT,  // 밉맵 이미지 필터링 방식
			0,				// 제거할 색상, 0을 넣으면 컬러키 무효화
			nullptr,		// 이미지 정보 기록(m_pTexInfo->tImgInfo 값을 위에서 이미 채웠음)
			nullptr,		// 팔레트 이미지 처리
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
