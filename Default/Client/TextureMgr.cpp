#include "stdafx.h"
#include "TextureMgr.h"

IMPLEMENT_SINGLETON(CTextureMgr)

CTextureMgr::CTextureMgr()
{
}


CTextureMgr::~CTextureMgr()
{
	Release();
}

TEXINFO* CTextureMgr::Get_Texture(const TCHAR * pObjKey, const TCHAR * pStateKey, const int & iCount)
{
	auto	iter = find_if(m_mapTexture.begin(), m_mapTexture.end(), [&](auto& MyPair)->bool
	{
		if (pObjKey == MyPair.first)
			return true;

		return false;
	});

	if (iter == m_mapTexture.end())
		return nullptr;
	
	return iter->second->Get_Texture(pStateKey, iCount);
}

HRESULT CTextureMgr::Insert_Texture(const TCHAR * pFilePath, TEXTYPE eType, const TCHAR * pObjKey, const TCHAR * pStateKey, const int & iCount)
{
	auto	iter = find_if(m_mapTexture.begin(), m_mapTexture.end(), [&](auto& MyPair)->bool
	{
		if (pObjKey == MyPair.first)
			return true;

		return false;
	});

	if (iter == m_mapTexture.end())
	{
		CTexture*		pTexture = nullptr;

		switch (eType)
		{
		case TEX_SINGLE:
			pTexture = new CSingleTexture;
			break;

		case TEX_MULTI:
			pTexture = new CMultiTexture;
			break;
		}

		if (FAILED(pTexture->Insert_Texture(pFilePath, pStateKey, iCount)))
		{
			AfxMessageBox(pFilePath);
			return E_FAIL;
		}

		m_mapTexture.insert({ pObjKey, pTexture });

	}


	return S_OK;
}

HRESULT CTextureMgr::Read_ImgPath(const wstring & wstrPath)
{
	wifstream		fin;
	fin.open(L"../Data/ImgPath.txt", ios::in);

	if (!fin.fail())
	{
		TCHAR		szObjKey[MAX_STR] = L"";
		TCHAR		szStateKey[MAX_STR] = L"";
		TCHAR		szCnt[MAX_STR] = L"";
		TCHAR		szPath[MAX_PATH] = L"";

		wstring		wstrCombined = L"";

		while (true)
		{
			fin.getline(szObjKey, MAX_STR, '|');
			fin.getline(szStateKey, MAX_STR, '|');
			fin.getline(szCnt, MAX_STR, '|');
			fin.getline(szPath, MAX_PATH);

			if (fin.eof())
				break;

			int iCount = _ttoi(szCnt);

			if (FAILED(Insert_Texture(szPath, TEX_MULTI, szObjKey, szStateKey, iCount)))
			{
				ERR_MSG(szPath);
				return E_FAIL;
			}

		}

		fin.close();
	}


	return S_OK;
}

void CTextureMgr::Release(void)
{
	for_each(m_mapTexture.begin(), m_mapTexture.end(), [](auto& MyPair)
	{
		Safe_Delete(MyPair.second);
	});
	m_mapTexture.clear();

}



//Ãß°¡

HRESULT CTextureMgr::Load_Texture(TEXTYPE eTexType, const wstring & wstrFilePath, const wstring & wstrObjectKey, const wstring & wstrStateKey, const int & iCount)
{
	CTexture* pTexture = nullptr;

	auto iter_find = m_mapTexture.find(wstrObjectKey);

	switch (eTexType)
	{
	case TEX_SINGLE:
		if (m_mapTexture.end() != iter_find)
			return E_FAIL;

		pTexture = new CSingleTexture;
		m_mapTexture[wstrObjectKey] = pTexture;
		break;

	case TEX_MULTI:
		if (m_mapTexture.end() == iter_find)
		{
			pTexture = new CMultiTexture;
			m_mapTexture[wstrObjectKey] = pTexture;
		}
		break;
	}

	m_mapTexture.insert({ wstrObjectKey, pTexture });

	//HRESULT hr = m_mapTexture[wstrObjectKey]->Insert_Texture(L"../Texture/Stage/Terrain/Tile/Tile%d.png", TEX_MULTI, L"Tile", 36);
	//(wstrFilePath, wstrStateKey, iCount);

	//if (FAILED(hr))										
	//{													
	//	ERR_MSG(L"Load Failed");									
	//	return E_FAIL;								
	//}

	return S_OK;
}
