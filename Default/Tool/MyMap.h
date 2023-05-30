#pragma once

#include "Include.h"
#include "MainFrm.h"

class CToolView;
class CMyMap
{
public:
	CMyMap();
	~CMyMap();

public:
	HRESULT		Initialize(void);

private:
	vector<MAP*>		m_vecMAP;
};

