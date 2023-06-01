#pragma once

#include "Include.h"

class CTexture
{
public:
	CTexture();
	virtual ~CTexture();

public:
	virtual TEXINFO*		Get_Texture(const TCHAR* pStateKey = L"",
											const int& iCount = 0)PURE;

	virtual HRESULT		Insert_Texture(const TCHAR* pFilePath, 
										const TCHAR* pStateKey = L"", 
										const int& iCount = 0) PURE;

	virtual void		Release(void)PURE;

public: // Ãß°¡
	virtual HRESULT Load_Texture(
		const wstring& swtrFilePath,
		const wstring& wstrStateKey = L"",
		const int& iCount = 0) PURE;
};

