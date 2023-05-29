#pragma once
#include "Include.h"

class CRenderMgr_JWA
{
	DECLARE_SINGLETON(CRenderMgr_JWA)

public:
	enum TOOLID { TILE, MAP, ID_END };

private:
	CRenderMgr_JWA();
	~CRenderMgr_JWA();

public:
	void Initialize();
	void Render();
	void Release();

	void Render_Tile();
	void Render_MapTool();
	void Render_MapTool_MiniMap();

public:
	void Set_ToolID(TOOLID _eID) { m_eID = _eID; }
	void Set_TileIndex(int _iIndex) { m_iTileIndex = _iIndex; }
	void Set_MapObjInfo(map<CString, vector<OBJPOS_JWA*>>* pObjInfo) { m_pMapObjInfo = pObjInfo; }

public:
	TOOLID m_eID;
	int m_iTileIndex;
	
private:
	map<CString, vector<OBJPOS_JWA*>>* m_pMapObjInfo;
};

