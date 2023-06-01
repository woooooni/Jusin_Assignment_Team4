#pragma once

#include "Include.h"
#include "MainFrm.h"

class CToolView;
class CTerrain
{
public:
	CTerrain();
	~CTerrain();

public:
	HRESULT		Initialize(void);
	HRESULT		Init_Tile(void);
	void		Update(void);
	void		Release(void);

public:
	// Render 함수
	void		Render(void);
	void		Tile_Render(void);
	void		Map_Render(void);

	void		Mini_Render(void);
	void		Mini_TileRender(void);
	void		Mini_MapRender(void);

	// Re-Render 함수
	void		Load_TileRender(void);

public:
	void		Set_MainView(CToolView* pMainView) { m_pMainView = pMainView; }
	void		Set_Ratio(D3DXMATRIX* pOut, float fRatioX, float fRatioY);
	void		Set_MyMap(CString _MyMap) { m_strMyMap = _MyMap; }
	void		Set_MapScale(float _fScale) { m_fMapScale = _fScale; }
	void		Set_StageInfo(int _iX, int _iY)
	{
		m_iTileX = _iX;
		m_iTileY = _iY;
	}

public:
	vector<TILE*>		Get_VecTileForSave(void) { return m_vecTile; }
	vector<TILE*>&		Get_VecTile(void) { return m_vecTile; }
	vector<MAP*>&		Get_VecMap(void) { return m_vecMap; }
	void				Get_MapScale(float _fScale) { _fScale = m_fMapScale; }
	int					Get_TileIndex(const D3DXVECTOR3& vPos);
	HRESULT				Save_TileData(const TCHAR* _pGetPath);
	HRESULT				Load_TileData(const TCHAR* _pGetPath);

public:
	void	Tile_Change(const D3DXVECTOR3& vPos, const int& iDrawID);
	bool	Picking(const D3DXVECTOR3& vPos, const int& iIndex);
	bool	Picking_Dot(const D3DXVECTOR3& vPos, const int& iIndex);

public:
	INFO	m_tInfo;

private:
	CToolView*			m_pMainView = nullptr;

public:
	// 타일 관련
	vector<TILE*>		m_vecTile;
	int					m_iTileX;
	int					m_iTileY;

public:
	// Map 관련
	vector<MAP*>		m_vecMap;
	CString				m_strMyMap;
	int					m_iMapWidth;
	int					m_iMapHeight;
	float				m_fMapScale;

};

