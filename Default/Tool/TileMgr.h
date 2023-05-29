#pragma once

class CTile;
class CToolView;

class CTileMgr
{

	DECLARE_SINGLETON(CTileMgr)

private:
	CTileMgr();
	~CTileMgr();

public:
	void		Change_Tile(POINT tMousePos, int _TexID,
							bool _b = FALSE, TCHAR _str[MIN_STR] = L"");
	CToolView*	Get_ToolView()					{ return m_pToolView; }
	void		Set_ToolView(CToolView* pView)	{ m_pToolView = pView; }

public:
	void	Initialize();
	void	Render();
	void	Mini_Render();
	void	Release();

private:
	vector<CTile*>		m_vecTile;
	CToolView* m_pToolView;
};

