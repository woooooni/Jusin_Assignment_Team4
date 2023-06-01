#include "stdafx.h"
#include "MyTerrain.h"
#include "Device.h"
#include "TextureMgr.h"
#include "TimeMgr.h"

CMyTerrain::CMyTerrain()
{
	m_vecTile.reserve(TILEX * TILEY);
}

CMyTerrain::~CMyTerrain()
{
	Release();
}

HRESULT CMyTerrain::Initialize(void)
{
	if (FAILED(LoadTile(L"../Data/Map.dat")))
		return E_FAIL;

	if (FAILED(Ready_Adjacency()))
		return E_FAIL;

	m_wstrObjKey = L"Terrain";
	m_wstrStateKey = L"Tile";

	return S_OK;
}

int CMyTerrain::Update(void)
{
	Move_Frame();

	if (0.f > ::Get_Mouse().x)
		__super::m_vScroll.x += 300.f * CTimeMgr::Get_Instance()->Get_TimeDelta();

	if (WINCX < ::Get_Mouse().x)
		__super::m_vScroll.x -= 300.f * CTimeMgr::Get_Instance()->Get_TimeDelta();

	if (0.f > ::Get_Mouse().y)
		__super::m_vScroll.y += 300.f * CTimeMgr::Get_Instance()->Get_TimeDelta();

	if (WINCY < ::Get_Mouse().y)
		__super::m_vScroll.y -= 300.f * CTimeMgr::Get_Instance()->Get_TimeDelta();

	return 0;
}

void CMyTerrain::Late_Update(void)
{
}

void CMyTerrain::Render(void)
{
#pragma region 생략
	//D3DXMATRIX		matWorld, matScale, matTrans;

	//TCHAR		szBuf[MIN_STR] = L"";
	//int			iIndex = 0;
	//
	//for (auto iter : m_vecTile)
	//{
	//	D3DXMatrixIdentity(&matWorld);
	//	D3DXMatrixTranslation(&matTrans, iter->vPos.x + m_vScroll.x, iter->vPos.y + m_vScroll.y, iter->vPos.z);

	//	matWorld = matTrans;

	//	const TEXINFO*		pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(m_wstrObjKey.c_str(), m_wstrStateKey.c_str(), m_vecTile[iIndex]->byDrawID);
	//	if (nullptr == pTexInfo)
	//		return;

	//	float		fX = pTexInfo->tImgInfo.Width / 2.f;
	//	float		fY = pTexInfo->tImgInfo.Height / 2.f;

	//	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

	//	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
	//		nullptr,
	//		&D3DXVECTOR3(fX, fY, 0.f),
	//		nullptr,
	//		D3DCOLOR_ARGB(255, 255, 255, 255));

	//	// 타일 인덱스
	//	swprintf_s(szBuf, L"%d", iIndex);

	//	CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(),  // 스프라이트 컴객체
	//		szBuf, // 출력할 문자열
	//		lstrlen(szBuf), // 문자열 길이
	//		nullptr,	// 출력할 렉트 주소값
	//		0,			// 위의 매개변수에서 지정한 사각형 범위 안에서의 정렬 옵션, DT_CENTER, DT_LEFT
	//		D3DCOLOR_ARGB(255, 0, 0, 255)); // 출력할 폰트 색상
	//	
	//	++iIndex;
	//}


#pragma endregion 생략


	D3DXMATRIX		matWorld, matTrans;

	TCHAR		szBuf[MIN_STR] = L"";
	int			iIndex = 0;

	int		iCullX = int(-m_vScroll.x) / TILECX;
	int		iCullY = int(-m_vScroll.y) / (TILECY / 2);

	int	iCullEndX = WINCX / TILECX;
	int	iCullEndY = WINCY / (TILECY / 2);

	for (int i = iCullY; i < iCullY + iCullEndY; ++i)
	{
		for (int j = iCullX; j < iCullX + iCullEndX; ++j)
		{
			int		iIndex = i * TILEX + j;

			if (0 > iIndex || (size_t)iIndex >= m_vecTile.size())
				continue;

			D3DXMatrixIdentity(&matWorld);
			D3DXMatrixTranslation(&matTrans,
				m_vecTile[iIndex]->vPos.x + m_vScroll.x,
				m_vecTile[iIndex]->vPos.y + m_vScroll.y,
				m_vecTile[iIndex]->vPos.z);

			matWorld = matTrans;

			const TEXINFO*		pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(m_wstrObjKey.c_str(), m_wstrStateKey.c_str(), m_vecTile[iIndex]->byDrawID);
			if (nullptr == pTexInfo)
				return;

			float		fX = pTexInfo->tImgInfo.Width / 2.f;
			float		fY = pTexInfo->tImgInfo.Height / 2.f;

			CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

			CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
				nullptr,
				&D3DXVECTOR3(fX, fY, 0.f),
				nullptr,
				D3DCOLOR_ARGB(255, 255, 255, 255));

			// 타일 인덱스
			swprintf_s(szBuf, L"%d", iIndex);

			CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(),  // 스프라이트 컴객체
				szBuf, // 출력할 문자열
				lstrlen(szBuf), // 문자열 길이
				nullptr,	// 출력할 렉트 주소값
				0,			// 위의 매개변수에서 지정한 사각형 범위 안에서의 정렬 옵션, DT_CENTER, DT_LEFT
				D3DCOLOR_ARGB(255, 0, 0, 255)); // 출력할 폰트 색상

			iIndex++;
		}
	}
}

void CMyTerrain::Release(void)
{
	for_each(m_vecTile.begin(), m_vecTile.end(), CDeleteObj());
	m_vecTile.clear();
	m_vecTile.shrink_to_fit();
}

HRESULT CMyTerrain::LoadTile(const TCHAR* pTilePath)
{
	HANDLE	hFile = CreateFile(pTilePath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD	dwByte = 0;
	TILE*	pTile = nullptr;

	while (true)
	{
		pTile = new TILE;

		ReadFile(hFile, pTile, sizeof(TILE), &dwByte, nullptr);

		if (0 == dwByte)
		{
			Safe_Delete(pTile);
			break;
		}

		m_vecTile.push_back(pTile);
	}

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CMyTerrain::Ready_Adjacency(void)
{
	m_vecAdj.resize(m_vecTile.size());

	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			int		iIndex = i * TILEX + j;

			// 좌상단 이웃의 규칙

			// 맨 윗줄이 아닐 때 && 맨 왼쪽 줄이 아닐 때
			if ((0 != i) && (0 != (iIndex % (TILEX * 2))))
			{
				// 홀 -> 짝 20 감소

				if ((0 != i % 2) && (!m_vecTile[iIndex - TILEX]->byOption))
					m_vecAdj[iIndex].push_back(m_vecTile[iIndex - TILEX]);

				// 짝 -> 홀 21 감소
				else if ((0 == i % 2) && (!m_vecTile[iIndex - (TILEX + 1)]->byOption))
					m_vecAdj[iIndex].push_back(m_vecTile[iIndex - (TILEX + 1)]);
			}

			// 우상단 이웃의 규칙

			// 맨 윗줄이 아닐 때 && 맨 오른쪽 줄이 아닐 때
			if ((0 != i) && ((TILEX * 2 - 1) != (iIndex % (TILEX * 2))))
			{
				// 홀 -> 짝 19 감소

				if ((0 != i % 2) && (!m_vecTile[iIndex - (TILEX - 1)]->byOption))
					m_vecAdj[iIndex].push_back(m_vecTile[iIndex - (TILEX - 1)]);

				// 짝 -> 홀 20 감소
				else if ((0 == i % 2) && (!m_vecTile[iIndex - TILEX]->byOption))
					m_vecAdj[iIndex].push_back(m_vecTile[iIndex - TILEX]);
			}

			// 좌하단 이웃의 규칙

			// 맨 아래줄이 아닐 때 && 맨 왼쪽 줄이 아닐 때
			if ((TILEY - 1 != i) && (0 != (iIndex % (TILEX * 2))))
			{
				// 홀 -> 짝 20 증가

				if ((0 != i % 2) && (!m_vecTile[iIndex + TILEX]->byOption))
					m_vecAdj[iIndex].push_back(m_vecTile[iIndex + TILEX]);

				// 짝 -> 홀 19 증가
				else if ((0 == i % 2) && (!m_vecTile[iIndex + (TILEX - 1)]->byOption))
					m_vecAdj[iIndex].push_back(m_vecTile[iIndex + (TILEX - 1)]);
			}

			// 우하단 이웃의 규칙

			// 맨 아래줄이 아닐 때 && 맨 오른쪽 줄이 아닐 때
			if ((TILEY - 1 != i) && ((TILEX * 2 - 1) != (iIndex % (TILEX * 2))))
			{
				// 홀 -> 짝 21 증가

				if ((0 != i % 2) && (!m_vecTile[iIndex + (TILEX + 1)]->byOption))
					m_vecAdj[iIndex].push_back(m_vecTile[iIndex + (TILEX + 1)]);

				// 짝 -> 홀 20 증가
				else if ((0 == i % 2) && (!m_vecTile[iIndex + TILEX]->byOption))
					m_vecAdj[iIndex].push_back(m_vecTile[iIndex + TILEX]);
			}
		}
	}

	return S_OK;
}