#include "stdafx.h"
#include "Terrain.h"
#include "TextureMgr.h"
#include "Device.h"
#include "ToolView.h"
#include "InspectorFormView.h"

CTerrain::CTerrain()
	: m_fMapScale(1.f), m_strMyMap(L"Map0.png")
{
	//m_vecTile.reserve(TILEX * TILEY);
}

CTerrain::~CTerrain()
{
	Release();
}

HRESULT CTerrain::Initialize(void)
{
	// 타일 Texture
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
	
	// 맵 Texture
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
	CDevice::Get_Instance()->Render_Begin();
	Map_Render();
	Tile_Render();
	CDevice::Get_Instance()->Render_End();
}

void CTerrain::Tile_Render(void)
{
	// 맵 사이즈에 맞춰서 타일 깔리게끔 수정할 것

	D3DXMATRIX	matWorld, matScale, matTrans;

	TCHAR		szBuf[MIN_STR] = L"";
	int			iIndex = 0;

	/*
	// 개수 지정시 사용하던 코드
	RECT	rc{};

	// GetClientRect : 현재 클라이언트 영역의 rect 정보를 얻어옴
	GetClientRect(m_pMainView->m_hWnd, &rc);

	float	fX = WINCX / float(rc.right - rc.left);
	float	fY = WINCY / float(rc.bottom - rc.top);
	*/

	CString strMapName = L"";

	int i = 0;
	for (; i < m_strMyMap.GetLength(); ++i)
	{
		if (0 != isdigit(m_strMyMap[i]))
			break;
	}

	//m_iTileX = m_iMapWidth / TILECX + 1;
	//m_iTileY = m_iMapHeight / (TILECY / 2.f) + 1;

	if (!m_vecTile.empty()) // 맵 변경시 Tile vec clear하기 때문
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
					m_vecTile[iIndex]->vPos.x * m_fMapScale - m_pMainView->GetScrollPos(0), // 0일 경우 x 스크롤 값 얻어옴
					m_vecTile[iIndex]->vPos.y * m_fMapScale - m_pMainView->GetScrollPos(1), // 1일 경우 y 스크롤 값 얻어옴
					0.f);

				matWorld = matScale * matTrans;

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
	/*
	for (auto iter : m_vecTile)
	{
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans, 
			iter->vPos.x - m_pMainView->GetScrollPos(0), // 0일 경우 x 스크롤 값 얻어옴
			iter->vPos.y - m_pMainView->GetScrollPos(1), // 1일 경우 y 스크롤 값 얻어옴
			0.f);

		matWorld = matScale * matTrans;

		Set_Ratio(&matWorld, fX, fY);

		const TEXINFO*	pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Terrain", L"Tile", iter->byDrawID);
		
		float	fX = pTexInfo->tImgInfo.Width / 2.f;
		float	fY = pTexInfo->tImgInfo.Height / 2.f;
			
		// 이미지에 행렬을 반영
		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
			nullptr,							// 출력할 이미지 영역에 대한 Rect 주소, null인 경우 이미지의 0, 0 기준으로 출력
			&D3DXVECTOR3(fX, fY, 0.f),			// 출력할 이미지의 중심축에 대한 vector3 주소, null인 경우 이미지의 0, 0이 중심 좌표
			nullptr,							// 위치 좌표에 대한 vector3 주소, null인 경우 스크린 상의 0, 0좌표 출력
			D3DCOLOR_ARGB(255, 255, 255, 255)); // 출력할 이미지와 섞을 색상 값, 0xffffffff를 넘겨주면 원본 색상 유지
		

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
	*/
}

// 통맵 Rendering
void CTerrain::Map_Render(void)
{
	D3DXMATRIX	matTrans, matScale, matWorld;

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
		
		CMainFrame*				pMainFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
		CInspectorFormView*		pInspectorForm = dynamic_cast<CInspectorFormView*>(pMainFrm->m_MainSplitter.GetPane(0, 2));
		CDlgTab3*		pTool = pInspectorForm->dlg3;
		CMapToolMap*	pMapTool = pTool->m_pMapForm;
		
		if (pMapTool)
		{
			pMapTool->Set_Text_MapInfo(pTexture->tImgInfo.Width, pTexture->tImgInfo.Height);
			m_iMapHeight = pTexture->tImgInfo.Height;
			m_iMapWidth = pTexture->tImgInfo.Width;
			m_pMainView->SetScrollSizes(MM_TEXT,
				CSize(m_iMapWidth * m_fMapScale, m_iMapHeight * m_fMapScale));
		}
		
		D3DXMatrixScaling(&matScale, m_fMapScale, m_fMapScale, 0.f);

		D3DXMatrixTranslation(&matTrans,
			-m_pMainView->GetScrollPos(0),
			-m_pMainView->GetScrollPos(1),
			0.f);

		matWorld = matScale * matTrans;

		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexture->pTexture,
			NULL,
			NULL,
			NULL,
			D3DCOLOR_ARGB(255, 255, 255, 255));
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
	/*
	for (auto iter : m_vecTile)
	{
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans,
			iter->vPos.x,
			iter->vPos.y,
			0.f);

		matWorld = matScale * matTrans;

		Set_Ratio(&matWorld, 0.3f, 0.5f);

		const TEXINFO*	pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Terrain", L"Tile", iter->byDrawID);

		float	fX = pTexInfo->tImgInfo.Width / 2.f;
		float	fY = pTexInfo->tImgInfo.Height / 2.f;

		// 이미지에 행렬을 반영
		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
			nullptr,
			&D3DXVECTOR3(fX, fY, 0.f),
			nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	*/

	if (!m_vecTile.empty()) // 맵 변경시 Tile vec clear하기 때문
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
	/*
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

		D3DXMatrixScaling(&matScale, m_fMapScale, m_fMapScale, 1.f);

		D3DXMatrixTranslation(&matTrans,
			-m_pMainView->GetScrollPos(0),
			-m_pMainView->GetScrollPos(1),
			0.f);

		matWorld = matScale * matTrans;

		Set_Ratio(&matWorld, 0.5f, 0.8f);

		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexture->pTexture,
			nullptr,
			nullptr,
			nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	*/

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

		D3DXMatrixScaling(&matScale, m_fMapScale, m_fMapScale, 0.f);

		D3DXMatrixTranslation(&matTrans,
			-m_pMainView->GetScrollPos(0),
			-m_pMainView->GetScrollPos(1),
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
	// 새로 맵을 불러올 때에도 사용할 수 있다.
	for_each(m_vecTile.begin(), m_vecTile.end(), CDeleteObj());
	m_vecTile.clear();
	m_vecTile.shrink_to_fit();

	//m_vecTile.clear();
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
		{
			return index;
		}
	}

	return -1;
}

bool CTerrain::Picking(const D3DXVECTOR3 & vPos, const int & iIndex)
{

	float	fGradient[4] {

		((TILECY / 2.f) / (TILECX / 2.f)) * -1.f,
		((TILECY / 2.f) / (TILECX / 2.f)),
		((TILECY / 2.f) / (TILECX / 2.f)) * -1.f,
		((TILECY / 2.f) / (TILECX / 2.f))
	};

	// 12, 3, 6, 9 시 순서로 점의 위치를 저장

	D3DXVECTOR3		vPoint[4] {

		{ m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y + (TILECY / 2.f), 0.f },
		{ m_vecTile[iIndex]->vPos.x + (TILECX / 2.f),m_vecTile[iIndex]->vPos.y, 0.f },
		{ m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y - (TILECY / 2.f), 0.f },
		{ m_vecTile[iIndex]->vPos.x - (TILECX / 2.f),m_vecTile[iIndex]->vPos.y , 0.f }
	};
	
	// Y = aX + b-> b = y - ax;

	float	fB[4] {

		vPoint[0].y - fGradient[0] * vPoint[0].x, 
		vPoint[1].y - fGradient[1] * vPoint[1].x,
		vPoint[2].y - fGradient[2] * vPoint[2].x,
		vPoint[3].y - fGradient[3] * vPoint[3].x	
	};


	// 0 == ax + b - y		: 직선 상에 있는 상태
	// 0 > ax + b - y		: 직선 보다 위쪽에 있는 상태
	// 0 < ax + b - y		: 직선 보다 아래쪽에 있는 상태

	bool		bCheck[4] { false };

	if (0 < fGradient[0] * vPos.x + fB[0] - vPos.y)
		bCheck[0] = true;

	if (0 >= fGradient[1] * vPos.x + fB[1] - vPos.y)
		bCheck[1] = true;

	if (0 > fGradient[2] * vPos.x + fB[2] - vPos.y)
		bCheck[2] = true;

	if (0 < fGradient[3] * vPos.x + fB[3] - vPos.y)
		bCheck[3] = true;
	
	return bCheck[0] && bCheck[1] && bCheck[2] && bCheck[3];
}

bool CTerrain::Picking_Dot(const D3DXVECTOR3 & vPos, const int & iIndex)
{
	D3DXVECTOR3		vPoint[4]{

		{ m_vecTile[iIndex]->vPos.x * m_fMapScale,m_vecTile[iIndex]->vPos.y * m_fMapScale + (TILECY * m_fMapScale / 2.f), 0.f },
		{ m_vecTile[iIndex]->vPos.x * m_fMapScale + (TILECX * m_fMapScale / 2.f),m_vecTile[iIndex]->vPos.y * m_fMapScale, 0.f },
		{ m_vecTile[iIndex]->vPos.x * m_fMapScale,m_vecTile[iIndex]->vPos.y * m_fMapScale - (TILECY * m_fMapScale / 2.f), 0.f },
		{ m_vecTile[iIndex]->vPos.x * m_fMapScale - (TILECX * m_fMapScale / 2.f),m_vecTile[iIndex]->vPos.y * m_fMapScale , 0.f }
	};

	D3DXVECTOR3			vDir[4] {
		
		vPoint[1] - vPoint[0],
		vPoint[2] - vPoint[1], 
		vPoint[3] - vPoint[2],
		vPoint[0] - vPoint[3],
	};

	D3DXVECTOR3			vNormal[4]{
		{ -vDir[0].y, vDir[0].x, 0.f},
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
