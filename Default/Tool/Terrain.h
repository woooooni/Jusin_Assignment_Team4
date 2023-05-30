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
	void		Update(void);
	void		Render(void);
	void		Index_Render(void);
	void		Mini_Render(void);
	void		Release(void);


public:
	void		Set_MainView(CToolView* pMainView) { m_pMainView = pMainView; }
	void		Set_Ratio(D3DXMATRIX* pOut, float fRatioX, float fRatioY);

public:
	vector<TILE*>&		Get_VecTile(void) { return m_vecTile; }
	int					Get_TileIndex(const D3DXVECTOR3& vPos);

	void	Tile_Change(const D3DXVECTOR3& vPos, const int& iDrawID);
	bool	Picking(const D3DXVECTOR3& vPos, const int& iIndex);
	bool	Picking_Dot(const D3DXVECTOR3& vPos, const int& iIndex);

public: // Ãß°¡
	INFO	m_tInfo;
	void	Set_Pos(float _fX, float _fY)
	{ 
		m_tInfo.vPos.x = _fX;
		m_tInfo.vPos.y = _fY;
	}

private:
	vector<TILE*>		m_vecTile;
	CToolView*			m_pMainView = nullptr;

};

