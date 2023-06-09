#pragma once
#include "Texture.h"

class CMultiTexture :
	public CTexture
{
public:
	CMultiTexture();
	virtual ~CMultiTexture();

public:
	virtual TEXINFO * Get_Texture(const TCHAR * pStateKey = L"", const int & iCount = 0) override;
	virtual HRESULT Insert_Texture(const TCHAR * pFilePath, const TCHAR * pStateKey = L"", const int & iCount = 0) override;
	virtual void Release(void) override;

public:
	virtual HRESULT Load_Texture(
		const wstring& wstrFilePath,
		const wstring& wstrStateKey = L"",
		const int& iCount = 0) override;

private:
	map<wstring, vector<TEXINFO*>>		m_mapMultiTex;
};

