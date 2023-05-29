#pragma once

class CFileInfo
{
public:
	CFileInfo();
	~CFileInfo();

public:
	static CString Convert_RelativePath(const CString strPath);
};

