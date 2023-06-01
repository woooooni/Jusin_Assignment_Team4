#pragma once

#include "SingleTexture.h"
#include "MultiTexture.h"

class CTextureMgr
{
	DECLARE_SINGLETON(CTextureMgr)

private:
	CTextureMgr();
	~CTextureMgr();

public:
	TEXINFO*		Get_Texture(const TCHAR* pObjKey,
									const TCHAR* pStateKey = L"",
									const int& iCount = 0);

	HRESULT		Insert_Texture(const TCHAR* pFilePath,
								TEXTYPE eType,
								const TCHAR* pObjKey,
								const TCHAR* pStateKey = L"",
								const int& iCount = 0);

	 void		Release(void);

public: // Ãß°¡
	HRESULT		Load_Texture(
		TEXTYPE eTexType, const wstring& wstrFilePath,
		const wstring& wstrObjectKey, const wstring& wstrStateKey = L"",
		const int& iCount = 0);

private:
	map<wstring, CTexture*>		m_mapTexture;
};

