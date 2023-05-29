#include "stdafx.h"
#include "Device.h"
#include "TileMgr.h"
#include "ToolView.h"
#include "TextureMgr.h"
#include "RenderMgr_JWA.h"

IMPLEMENT_SINGLETON(CRenderMgr_JWA)

CRenderMgr_JWA::CRenderMgr_JWA()
{
}

CRenderMgr_JWA::~CRenderMgr_JWA()
{
	Release();
}

void CRenderMgr_JWA::Initialize()
{
	m_iTileIndex = -1;
}

void CRenderMgr_JWA::Render()
{
	CDevice::Get_Instance()->Render_Begin();

	if (m_eID == TILE)
		Render_Tile();

	else if (m_eID == MAP)
	{
		CTileMgr::Get_Instance()->Render();
		Render_MapTool();
	}

	CDevice::Get_Instance()->Render_End();
}

void CRenderMgr_JWA::Release()
{
}

void CRenderMgr_JWA::Render_Tile()
{
	if (m_iTileIndex == -1)
		return;
}

void CRenderMgr_JWA::Render_MapTool()
{
	int iScrollX = CTileMgr::Get_Instance()->Get_ToolView()->GetScrollPos(0);
	int iScrollY = CTileMgr::Get_Instance()->Get_ToolView()->GetScrollPos(1);

	for (auto& iter : *m_pMapObjInfo)
	{
		for (size_t i = 0; i < iter.second.size(); ++i)
		{
			wstring strName = iter.first.GetString();

			// Obj 텍스쳐 불러오기
			// const TEXINFO* pTexInfo = TextureMgr::Get_Instance()-> ~

			D3DXMATRIX matScale, matWorld, matTrans;
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
			D3DXMatrixTranslation(&matTrans, iter.second[i]->vPos.x - iScrollX,
				iter.second[i]->vPos.y - iScrollY, 0.f);
			matWorld = matScale * matTrans;

			CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
			// Obj 텍스쳐 Draw
			//CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr,
				//&D3DXVECTOR3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}

}

void CRenderMgr_JWA::Render_MapTool_MiniMap()
{
	for (auto& iter : *m_pMapObjInfo)
	{
		for (size_t i = 0; i < iter.second.size(); ++i)
		{
			wstring strName = iter.first.GetString();

			// Obj 텍스쳐 불러오기
			// const TEXINFO* pTexInfo = TextureMgr::Get_Instance()-> ~

			D3DXMATRIX matScale, matWorld, matTrans;
			D3DXMatrixScaling(&matScale, 1.f / 2.f, 1.f / 1.4f, 1.f);
			D3DXMatrixTranslation(&matTrans, iter.second[i]->vPos.x / 2.f,
				iter.second[i]->vPos.y / 1.4f, 0.f);
			matWorld = matScale * matTrans;

			CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		}
	}
}
