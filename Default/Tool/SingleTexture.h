#pragma once
#include "Texture.h"

class CSingleTexture :
	public CTexture
{
public:
	CSingleTexture();
	virtual ~CSingleTexture();

public:
	virtual TEXINFO* Get_Texture(const TCHAR * pStateKey = L"", const int & iCount = 0) override
	{
		return m_pTexInfo;
	}

public:
	virtual HRESULT Insert_Texture(const TCHAR * pFilePath, const TCHAR * pStateKey = L"", const int & iCount = 0) override;
	virtual void Release(void) override;

public:
	virtual HRESULT Load_Texture(
		const wstring& wstrFilePath,
		const wstring& wstrStateKey = L"",
		const int& iCount = 0) override;

private:
	TEXINFO*			m_pTexInfo;
};

