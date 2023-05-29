#pragma once

#include "Include.h"

class CTile
{
public:
	CTile();
	~CTile();

public:
	//INFO&		Get_Info()				{ return m_tInfo; }
	//void		Set_Info(INFO& _tInfo)  { m_tInfo = _tInfo; }

	D3DXVECTOR3	Get_Pos()					  { return m_tInfo.vPos; }
	void		Set_Pos(float _fX, float _fY) { m_tInfo.vPos.x = _fX; m_tInfo.vPos.y = _fY; }

	void		Render_MiniView();

public:
	void Initialize();
	int	 Update();
	void Render();
	void Release();

private:
	TILE m_tInfo;
};

