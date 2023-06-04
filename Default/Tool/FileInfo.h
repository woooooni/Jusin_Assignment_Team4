#pragma once

#include "Include.h"

class CFileInfo
{
public:
	CFileInfo();
	~CFileInfo();

public:
	static CString		ConvertRelativePath(CString strFullPath);

	static int			DirFileCnt(const wstring& wstrPath);
	static void			DirInfoExtraction(const wstring& wstrPath, list<IMGPATH*>& rPathList);
};

