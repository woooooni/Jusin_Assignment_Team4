#include "stdafx.h"
#include "TextureMgr.h"
#include "Tile.h"
#include "TileMgr.h"
#include "ToolView.h"

IMPLEMENT_SINGLETON(CTileMgr)

CTileMgr::CTileMgr()
{
	/*
	HRESULT hr = CTextureMgr::Get_Instance()->Load_Texture(
		TEX_MULTI, L"../Texture/Stage/Terrain/Tile/Tile%d.png",
		L"Terrian", L"Tile", TILE_TEX);
	
	if (FAILED(hr))
		return;
	*/
}

CTileMgr::~CTileMgr()
{
	Release();
}

void CTileMgr::Initialize()
{
	m_vecTile.reserve(TILEX * TILEY);
	CTile* pTile = nullptr;

	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			pTile = new CTile;

			float	fX = (TILECX * j) + ((TILECX / 2.f) * (i % 2));
			float	fY = (TILECY / 2.f) * i;

			pTile->Set_Pos((float)fX, (float)fY);

			m_vecTile.push_back(pTile);
		}
	}
};

void CTileMgr::Render()
{
	TCHAR szIndex[MIN_STR] = L"";

	for (size_t i = 0; i < m_vecTile.size(); ++i)
	{
		m_vecTile[i]->Render();

		swprintf_s(szIndex, L"%d", i);
	}
}

void CTileMgr::Mini_Render()
{
	for (size_t i = 0; i < m_vecTile.size(); ++i)
		m_vecTile[i]->Render_MiniView();
}

void CTileMgr::Change_Tile(POINT tMousePos, int _TexID, bool _b, TCHAR _str[MIN_STR])
{
}

void CTileMgr::Release()
{
	for_each(m_vecTile.begin(), m_vecTile.end(), CDeleteObj());
	m_vecTile.clear();
	m_vecTile.shrink_to_fit();
}
