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
	const wstring&		Get_String() { return m_wstrFullPath; } // client
	TEXINFO*			Get_Texture(const TCHAR* pObjKey,
									const TCHAR* pStateKey = L"",
									const int& iCount = 0);
	void				Set_String(wstring wstrFullPath) { m_wstrFullPath = wstrFullPath; }

	HRESULT				Insert_Texture(const TCHAR* pFilePath,
								TEXTYPE eType,
								const TCHAR* pObjKey,
								const TCHAR* pStateKey = L"",
								const int& iCount = 0);

	HRESULT				Read_ImgPath(const wstring& wstrPath);
	void				Release(void);

public:
	HRESULT				Load_Texture(
		TEXTYPE eTexType, const wstring& wstrFilePath,
		const wstring& wstrObjectKey, const wstring& wstrStateKey = L"",
		const int& iCount = 0);

private:
	map<wstring, CTexture*>		m_mapTexture;
	wstring						m_wstrFullPath;
};

