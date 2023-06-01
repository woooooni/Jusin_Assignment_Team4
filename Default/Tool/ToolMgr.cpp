#include "stdafx.h"
#include "ToolMgr.h"
#include "Functor.h"
#include "Obj.h"
#include "MainFrm.h"
#include "Device.h"
#include "ToolView.h"
#include "TextureMgr.h"
#include "Texture.h"

CToolMgr::CToolMgr()
	: m_pTargetObj(nullptr)
	, m_pMainFrm(nullptr)
{
}


CToolMgr::~CToolMgr()
{
	for (UINT i = 0; i < m_vecObj.size(); ++i)
		Safe_Delete<CObj*>(m_vecObj[i]);

}

void CToolMgr::SwapObj(CObj * _pSrc, CObj * _pDest)
{

}

void CToolMgr::SaveObjData()
{
	// TODO :: SAVE.
	for (auto& obj : m_vecObj)
	{
		if (obj->GetAnimInfoMapSize() != 4)
		{
			wstring strErr = obj->Get_ObjName() + L"애니메이션을 모두 세팅해야 합니다.";
			AfxMessageBox(strErr.c_str());
			return;
		}
	}



}

void CToolMgr::LoadObjData()
{
	// TODO :: LoadObjData
}

void CToolMgr::SetTargetObj(int _iIdx)
{
	if (m_vecObj.size() < _iIdx)
	{
		AfxMessageBox(L"SetTarget 안되요.");
		return;
	}
	if (_iIdx < 0)
		m_pTargetObj = nullptr;

	m_pTargetObj = m_vecObj[_iIdx];
}

void CToolMgr::DeleteObj(int _iIdx)
{
	if (m_vecObj.size() < _iIdx)
	{
		AfxMessageBox(L"Delete 안되요.");
		return;
	}

	auto& iter = m_vecObj.begin();
	for (; iter != m_vecObj.end(); ++iter)
	{
		if ((*iter) == m_vecObj[_iIdx])
		{
			Safe_Delete<CObj*>((*iter));
			m_vecObj.erase(iter);
			m_pTargetObj = nullptr;
			return;
		}
	}
}

void CToolMgr::UpdateAllView()
{
	m_pMainFrm->Update_AllView();
}

void CToolMgr::RenderObj()
{
	CToolView*	pToolView = CToolMgr::GetInst()->GetMainFrm()->GetToolView();
	CTerrain*	pTerrain = pToolView->GetTerrain();
	float fMapScale = pTerrain->Get_MapScale();

	// 개수 지정시 사용하던 코드
	RECT	rc{};

	GetClientRect(pToolView->m_hWnd, &rc);

	float	fX = WINCX / float(rc.right - rc.left);
	float	fY = WINCY / float(rc.bottom - rc.top);

	D3DXMATRIX	matWorld, matScale, matTrans;

	for (auto& obj : m_vecObj)
	{
		TEXINFO* pTexInfo = nullptr;

		if (obj->GetAnimTexMapSize() > 0)
		{
			const	vector<ANIMINFO_KJM>& vecAnimInfo = obj->GetCurAnimInfoVec();
			const	vector<CTexture*>& vecAnimTex = obj->GetCurAnimTexVec();

			pTexInfo = vecAnimTex[obj->Get_AnimIdx()]->Get_Texture(obj->Get_ObjState().c_str());
		}
		else
		{
			pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(obj->Get_ObjKey().c_str(), L"Dash", obj->Get_AnimIdx());
		}


		int iCX = pTexInfo->tImgInfo.Width;
		int iCY = pTexInfo->tImgInfo.Height;
		
		obj->Set_Size({ float(iCX), float(iCY), 0.f });

		D3DXMatrixIdentity(&matWorld);

		D3DXMatrixScaling(&matScale, fMapScale, fMapScale, 0.f);

		D3DXMatrixTranslation(&matTrans,
			obj->Get_Info().vPos.x * fMapScale - pToolView->GetScrollPos(0), // 0일 경우 x 스크롤 값 얻어옴
			obj->Get_Info().vPos.y * fMapScale - pToolView->GetScrollPos(1), // 1일 경우 y 스크롤 값 얻어옴
			0.f);

		matWorld = matScale * matTrans;
		pTerrain->Set_Ratio(&matWorld, fX, fY);

		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
			nullptr,
			&D3DXVECTOR3(iCX / 2.f, iCY / 2.f, 0.f),
			nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}
		




}

bool CToolMgr::ObjPicking_Dot(const D3DXVECTOR3 & vPos, const int & iIndex)
{
	CTerrain* pTerrain = CToolMgr::GetInst()->GetMainFrm()->GetToolView()->GetTerrain();
	float fMapScale = pTerrain->Get_MapScale();

	const D3DXVECTOR3& vObjPos	= m_vecObj[iIndex]->Get_Info().vPos;
	const D3DXVECTOR3& vObjSize = m_vecObj[iIndex]->Get_Info().vSize;

	D3DXVECTOR3		vPoint[4]
	{
		{ vObjPos.x, vObjPos.y + (vObjSize.y / 2.f), 0.f },
		{ vObjPos.x + (vObjSize.x / 2.f), vObjPos.y, 0.f },
		{ vObjPos.x, vObjPos.y - (vObjSize.y / 2.f), 0.f },
		{ vObjPos.x - (vObjSize.x / 2.f), vObjPos.y, 0.f }
	};

	D3DXVECTOR3			vDir[4]
	{
		vPoint[1] - vPoint[0],
		vPoint[2] - vPoint[1],
		vPoint[3] - vPoint[2],
		vPoint[0] - vPoint[3]
	};

	D3DXVECTOR3			vNormal[4]
	{
		{ -vDir[0].y, vDir[0].x, 0.f },
		{ -vDir[1].y, vDir[1].x, 0.f },
		{ -vDir[2].y, vDir[2].x, 0.f },
		{ -vDir[3].y, vDir[3].x, 0.f }
	};

	D3DXVECTOR3			vMouseDir[4]{
		vPos - vPoint[0],
		vPos - vPoint[1],
		vPos - vPoint[2],
		vPos - vPoint[3]
	};

	for (int i = 0; i < 4; ++i)
	{
		D3DXVec3Normalize(&vNormal[i], &vNormal[i]);
		D3DXVec3Normalize(&vMouseDir[i], &vMouseDir[i]);
	}

	for (int i = 0; i < 4; ++i)
	{
		if (0.f < D3DXVec3Dot(&vNormal[i], &vMouseDir[i]))
			return false;
	}

	return true;
}

bool CToolMgr::PickObj(const D3DXVECTOR3 & vPos)
{
	m_pTargetObj = nullptr;

	for (size_t index = 0; index < m_vecObj.size(); ++index)
	{
		if (ObjPicking_Dot(vPos, index))
		{
			SetTargetObj(index);
			UpdateAllView();
			return true;
		}
	}

	UpdateAllView();
	return false;
}
