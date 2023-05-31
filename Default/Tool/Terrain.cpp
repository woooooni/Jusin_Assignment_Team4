#include "stdafx.h"
#include "Terrain.h"
#include "TextureMgr.h"
#include "Device.h"
#include "ToolView.h"
#include "InspectorFormView.h"
#include "ToolMgr.h"

CTerrain::CTerrain()
	: m_fMapScale(1.f)
	, m_strMyMap(L"Map0.png")
{
	//m_vecTile.reserve(TILEX * TILEY);
}

CTerrain::~CTerrain()
{
	Release();
}

HRESULT CTerrain::Initialize(void)
{
	// Ÿ�� Texture
	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Stage/Terrain/Tile/Tile%d.png", TEX_MULTI, L"Terrain", L"Tile", 36)))
	{
		AfxMessageBox(L"TileTexture Create Failed");
		return E_FAIL;
	}

	m_iTileX = m_iMapWidth / TILECX + 1;
	m_iTileY = m_iMapHeight / (TILECY / 2.f) + 1;
	
	for (int i = 0; i < m_iTileY; ++i)
	{
		for (int j = 0; j < m_iTileX; ++j)
		{
			TILE* pTile = new TILE;

			float	fX = (TILECX * j) + ((TILECX / 2.f) * (i % 2));
			float	fY = (TILECY / 2.f) * i;

			pTile->vPos = { fX, fY, 0.f };
			pTile->vSize = { TILECX, TILECY, 0.f };
			pTile->byOption = 0;
			pTile->byDrawID = 1;

			m_vecTile.push_back(pTile);
		}
	}
	
	// �� Texture
	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Stage/Terrain/Map/Map%d.png", TEX_MULTI, L"Map", L"Map", 5)))
	{
		AfxMessageBox(L"Map Texture Create Failed");
		return E_FAIL;
	}

	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Stage/Terrain/PreMap/Map%d.png", TEX_MULTI, L"PreMap", L"Map", 5)))
	{
		AfxMessageBox(L"PreMap Texture Create Failed");
		return E_FAIL;
	}

	return S_OK;

	Render();
}

HRESULT CTerrain::Init_Tile(void)
{
	Release();

	m_iTileX = m_iMapWidth / TILECX + 1;
	m_iTileY = m_iMapHeight / (TILECY / 2.f) + 1;

	for (int i = 0; i < m_iTileY; ++i)
	{
		for (int j = 0; j < m_iTileX; ++j)
		{
			TILE*		pTile = new TILE;

			float	fX = (j * TILECX) + ((i % 2) * (TILECX / 2.f));
			float	fY = i * (TILECY / 2.f);

			pTile->vPos = { fX, fY, 0.f };
			pTile->vSize = { TILECX, TILECY, 0.f };
			pTile->byOption = 0;
			pTile->byDrawID = 1;

			m_vecTile.push_back(pTile);
		}
	}

	return S_OK;
}

void CTerrain::Update(void)
{
}

void CTerrain::Render(void)
{
	Map_Render();

	CToolView* pToolView = CToolMgr::GetInst()->GetMainFrm()->GetToolView();

	if (pToolView->GetToolMode() == EDIT_MODE::EDIT_TILE)
		Tile_Render();
}

// ��� Rendering
void CTerrain::Map_Render(void)
{
	if (L"" != m_strMyMap)
	{
		CString cstrObjKey = L"";
		CString cstrStateKey = L"";
		CString cstrTemp = L"";

		int		i = 0;

		for (i = 0; i < m_strMyMap.GetLength(); ++i)
		{
			if (0 != isdigit(m_strMyMap[i]))
				break;

			else
				cstrStateKey += m_strMyMap[i];
		}
		cstrObjKey = cstrStateKey;
		cstrTemp = m_strMyMap;
		cstrTemp.Delete(0, i);
		int iCount = _tstoi(cstrTemp);


		CInspectorFormView*		pInspectorForm = CToolMgr::GetInst()->GetMainFrm()->GetInspectorView();
		CDlgTab3*				pTool = pInspectorForm->dlg3;
		CMapToolMap*			pMapTool = pTool->m_pMapForm;
		CToolView*				pToolView = CToolMgr::GetInst()->GetMainFrm()->GetToolView();


		const TEXINFO*		pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(
			cstrObjKey, cstrStateKey, iCount);

		m_iMapWidth = pTexInfo->tImgInfo.Width;
		m_iMapHeight = pTexInfo->tImgInfo.Height;

		pMapTool->Set_Text_MapInfo(pTexInfo->tImgInfo.Width, pTexInfo->tImgInfo.Height);
		pToolView->SetScrollSizes(MM_TEXT, CSize(m_iMapWidth, m_iMapHeight));

		D3DXMATRIX		matWorld, matScale, matTrans;
		RECT			rc{};

		GetClientRect(pToolView->m_hWnd, &rc);

		float    fX = WINCX / float(rc.right - rc.left);
		float    fY = WINCY / float(rc.bottom - rc.top);

		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matScale, m_fMapScale, m_fMapScale, 1.f);
		D3DXMatrixTranslation(&matTrans,
			-pToolView->GetScrollPos(0),
			-pToolView->GetScrollPos(1),
			0.f);

		matWorld = matScale * matTrans;
		Set_Ratio(&matWorld, fX, fY);

		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
			nullptr,                            // ����� �̹��� ������ ���� Rect �ּ�, null�� ��� �̹����� 0, 0 �������� ���
			&D3DXVECTOR3(fX, fY, 0.f),            // ����� �̹����� �߽��࿡ ���� vector3 �ּ�, null�� ��� �̹����� 0, 0�� �߽� ��ǥ
			nullptr,                            // ��ġ ��ǥ�� ���� vector3 �ּ�, null�� ��� ��ũ�� ���� 0, 0��ǥ ���
			D3DCOLOR_ARGB(255, 255, 255, 255)); // ����� �̹����� ���� ���� ��, 0xffffffff�� �Ѱ��ָ� ���� ���� ����
	}
}

void CTerrain::Tile_Render(void)
{

	CToolView*	pToolView = CToolMgr::GetInst()->GetMainFrm()->GetToolView();
	TCHAR		szBuf[MIN_STR] = L"";
	int			iIndex = 0;

	
	// ���� ������ ����ϴ� �ڵ�
	RECT	rc{};

	GetClientRect(pToolView->m_hWnd, &rc);

	float	fX = WINCX / float(rc.right - rc.left);
	float	fY = WINCY / float(rc.bottom - rc.top);
	

	CString strMapName = L"";

	for (int i = 0; i < m_strMyMap.GetLength(); ++i)
	{
		if (0 != isdigit(m_strMyMap[i]))
			break;
	}


	D3DXMATRIX	matWorld, matScale, matTrans;
	if (!m_vecTile.empty()) // �� ����� Tile vec clear�ϱ� ����
	{
		for (int i = 0; i < m_iTileY; ++i)
		{
			for (int j = 0; j < m_iTileX; ++j)
			{
				int iIndex = (i * m_iTileX) + j;

				const TEXINFO* pTexInfo
					= CTextureMgr::Get_Instance()->Get_Texture(L"Terrain", L"Tile", m_vecTile[iIndex]->byDrawID);

				D3DXMatrixIdentity(&matWorld);

				D3DXMatrixScaling(&matScale, m_fMapScale, m_fMapScale, 0.f);

				D3DXMatrixTranslation(&matTrans,
					m_vecTile[iIndex]->vPos.x * m_fMapScale - pToolView->GetScrollPos(0), // 0�� ��� x ��ũ�� �� ����
					m_vecTile[iIndex]->vPos.y * m_fMapScale - pToolView->GetScrollPos(1), // 1�� ��� y ��ũ�� �� ����
					0.f);

				matWorld = matScale * matTrans;
				Set_Ratio(&matWorld, fX, fY);

				int iCX = pTexInfo->tImgInfo.Width;
				int iCY = pTexInfo->tImgInfo.Height;

				CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
				CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
					nullptr,							
					&D3DXVECTOR3(iCX / 2.f, iCY / 2.f, 0.f),
					nullptr,
					D3DCOLOR_ARGB(255, 255, 255, 255));

				if (g_bIndex)
				{
					swprintf_s(szBuf, L"%d", iIndex);

					CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(),
						szBuf,
						lstrlen(szBuf),
						nullptr,
						DT_CENTER,
						D3DCOLOR_ARGB(255, 255, 255, 255));

					++iIndex;
				}
			}
		}
	}
}

void CTerrain::Mini_Render(void)
{
	Mini_MapRender();
	Mini_TileRender();
}

void CTerrain::Mini_TileRender(void)
{
	D3DXMATRIX	matWorld, matScale, matTrans;

	if (!m_vecTile.empty()) // �� ����� Tile vec clear�ϱ� ����
	{
		for (int i = 0; i < m_iTileY; ++i)
		{
			for (int j = 0; j < m_iTileX; ++j)
			{
				int iIndex = (i * m_iTileX) + j;

				const TEXINFO* pTexInfo
					= CTextureMgr::Get_Instance()->Get_Texture(L"Terrain", L"Tile", m_vecTile[iIndex]->byDrawID);

				D3DXMatrixIdentity(&matWorld);
				D3DXMatrixScaling(&matScale, m_fMapScale, m_fMapScale, 0.f);
				D3DXMatrixTranslation(&matTrans,
					m_vecTile[iIndex]->vPos.x * m_fMapScale,
					m_vecTile[iIndex]->vPos.y * m_fMapScale,
					0.f);

				matWorld = matScale * matTrans;

				Set_Ratio(&matWorld, 0.3f, 0.5f);

				int iCX = pTexInfo->tImgInfo.Width;
				int iCY = pTexInfo->tImgInfo.Height;

				CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
				CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
					nullptr,
					&D3DXVECTOR3(iCX / 2.f, iCY / 2.f, 0.f),
					nullptr,
					D3DCOLOR_ARGB(255, 255, 255, 255));
			}
		}
	}
}

void CTerrain::Mini_MapRender(void)
{
	D3DXMATRIX	matWorld, matScale, matTrans;

	if (L"" != m_strMyMap)
	{
		CString cstrObjKey = L"";
		CString cstrStateKey = L"";
		CString cstrTemp = L"";

		int		i = 0;

		for (i = 0; i < m_strMyMap.GetLength(); ++i)
		{
			if (0 != isdigit(m_strMyMap[i]))
				break;

			else
				cstrStateKey += m_strMyMap[i];
		}

		cstrObjKey = cstrStateKey;
		cstrTemp = m_strMyMap;
		cstrTemp.Delete(0, i);
		int iCount = _tstoi(cstrTemp);

		const TEXINFO*		pTexture = CTextureMgr::Get_Instance()->Get_Texture(
			cstrObjKey, cstrStateKey, iCount);


		m_iMapHeight = pTexture->tImgInfo.Height;
		m_iMapWidth = pTexture->tImgInfo.Width;

		CToolView*				pToolView = CToolMgr::GetInst()->GetMainFrm()->GetToolView();

		D3DXMatrixScaling(&matScale, m_fMapScale, m_fMapScale, 0.f);
		D3DXMatrixTranslation(&matTrans,
			-pToolView->GetScrollPos(0),
			-pToolView->GetScrollPos(1),
			0.f);

		matWorld = matScale * matTrans;

		Set_Ratio(&matWorld, 0.3f, 0.5f);

		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexture->pTexture,
			NULL,
			NULL,
			NULL,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CTerrain::Release(void)
{
	for_each(m_vecTile.begin(), m_vecTile.end(), CDeleteObj());
	m_vecTile.clear();
	m_vecTile.shrink_to_fit();
}

void CTerrain::Tile_Change(const D3DXVECTOR3 & vPos, const int & iDrawID)
{
	int		iIndex = Get_TileIndex(vPos);

	if (-1 == iIndex)
		return;

	m_vecTile[iIndex]->byDrawID = (BYTE)iDrawID;
	m_vecTile[iIndex]->byOption = 1;
}

void CTerrain::Set_Ratio(D3DXMATRIX * pOut, float fRatioX, float fRatioY)
{
	pOut->_11 *= fRatioX;
	pOut->_21 *= fRatioX;
	pOut->_31 *= fRatioX;
	pOut->_41 *= fRatioX;

	pOut->_12 *= fRatioY;
	pOut->_22 *= fRatioY;
	pOut->_32 *= fRatioY;
	pOut->_42 *= fRatioY;
}

int CTerrain::Get_TileIndex(const D3DXVECTOR3 & vPos)
{
	for (size_t index = 0; index < m_vecTile.size(); ++index)
	{
		if (Picking_Dot(vPos, index))
			return index;
	}

	return -1;
}


bool CTerrain::Picking_Dot(const D3DXVECTOR3 & vPos, const int & iIndex)
{
	D3DXVECTOR3		vPoint[4]
	{
		{ m_vecTile[iIndex]->vPos.x * m_fMapScale,m_vecTile[iIndex]->vPos.y * m_fMapScale + (TILECY * m_fMapScale / 2.f), 0.f },
		{ m_vecTile[iIndex]->vPos.x * m_fMapScale + (TILECX * m_fMapScale / 2.f),m_vecTile[iIndex]->vPos.y * m_fMapScale, 0.f },
		{ m_vecTile[iIndex]->vPos.x * m_fMapScale, m_vecTile[iIndex]->vPos.y * m_fMapScale - (TILECY * m_fMapScale / 2.f), 0.f },
		{ m_vecTile[iIndex]->vPos.x * m_fMapScale - (TILECX * m_fMapScale / 2.f),m_vecTile[iIndex]->vPos.y * m_fMapScale , 0.f }
	};

	D3DXVECTOR3			vDir[4]
	{
		vPoint[1] - vPoint[0],
		vPoint[2] - vPoint[1],
		vPoint[3] - vPoint[2],
		vPoint[0] - vPoint[3],
	};

	D3DXVECTOR3			vNormal[4]
	{
		{ -vDir[0].y, vDir[0].x, 0.f },
		{ -vDir[1].y, vDir[1].x, 0.f },
		{ -vDir[2].y, vDir[2].x, 0.f },
		{ -vDir[3].y, vDir[3].x, 0.f },
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
//
//bool CTerrain::Picking(const D3DXVECTOR3 & vPos, const int & iIndex)
//{
//
//	float	fGradient[4] {
//		((TILECY / 2.f) / (TILECX / 2.f)) * -1.f,
//		((TILECY / 2.f) / (TILECX / 2.f)),
//		((TILECY / 2.f) / (TILECX / 2.f)) * -1.f,
//		((TILECY / 2.f) / (TILECX / 2.f))
//	};
//
//	// 12, 3, 6, 9 �� ������ ���� ��ġ�� ����
//
//	D3DXVECTOR3		vPoint[4] {
//
//		{ m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y + (TILECY / 2.f), 0.f },
//		{ m_vecTile[iIndex]->vPos.x + (TILECX / 2.f),m_vecTile[iIndex]->vPos.y, 0.f },
//		{ m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y - (TILECY / 2.f), 0.f },
//		{ m_vecTile[iIndex]->vPos.x - (TILECX / 2.f),m_vecTile[iIndex]->vPos.y , 0.f }
//	};
//	
//	// Y = aX + b-> b = y - ax;
//
//	float	fB[4] {
//		vPoint[0].y - fGradient[0] * vPoint[0].x, 
//		vPoint[1].y - fGradient[1] * vPoint[1].x,
//		vPoint[2].y - fGradient[2] * vPoint[2].x,
//		vPoint[3].y - fGradient[3] * vPoint[3].x	
//	};
//
//
//	// 0 == ax + b - y		: ���� �� �ִ� ����
//	// 0 > ax + b - y		: ���� ���� ���ʿ� �ִ� ����
//	// 0 < ax + b - y		: ���� ���� �Ʒ��ʿ� �ִ� ����
//
//	bool		bCheck[4] { false };
//
//	if (0 < fGradient[0] * vPos.x + fB[0] - vPos.y)
//		bCheck[0] = true;
//
//	if (0 >= fGradient[1] * vPos.x + fB[1] - vPos.y)
//		bCheck[1] = true;
//
//	if (0 > fGradient[2] * vPos.x + fB[2] - vPos.y)
//		bCheck[2] = true;
//
//	if (0 < fGradient[3] * vPos.x + fB[3] - vPos.y)
//		bCheck[3] = true;
//	
//	return bCheck[0] && bCheck[1] && bCheck[2] && bCheck[3];
//}

