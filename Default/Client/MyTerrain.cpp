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
#pragma region ����
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

	//	// Ÿ�� �ε���
	//	swprintf_s(szBuf, L"%d", iIndex);

	//	CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(),  // ��������Ʈ �İ�ü
	//		szBuf, // ����� ���ڿ�
	//		lstrlen(szBuf), // ���ڿ� ����
	//		nullptr,	// ����� ��Ʈ �ּҰ�
	//		0,			// ���� �Ű��������� ������ �簢�� ���� �ȿ����� ���� �ɼ�, DT_CENTER, DT_LEFT
	//		D3DCOLOR_ARGB(255, 0, 0, 255)); // ����� ��Ʈ ����
	//	
	//	++iIndex;
	//}


#pragma endregion ����


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

			// Ÿ�� �ε���
			swprintf_s(szBuf, L"%d", iIndex);

			CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(),  // ��������Ʈ �İ�ü
				szBuf, // ����� ���ڿ�
				lstrlen(szBuf), // ���ڿ� ����
				nullptr,	// ����� ��Ʈ �ּҰ�
				0,			// ���� �Ű��������� ������ �簢�� ���� �ȿ����� ���� �ɼ�, DT_CENTER, DT_LEFT
				D3DCOLOR_ARGB(255, 0, 0, 255)); // ����� ��Ʈ ����

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

			// �»�� �̿��� ��Ģ

			// �� ������ �ƴ� �� && �� ���� ���� �ƴ� ��
			if ((0 != i) && (0 != (iIndex % (TILEX * 2))))
			{
				// Ȧ -> ¦ 20 ����

				if ((0 != i % 2) && (!m_vecTile[iIndex - TILEX]->byOption))
					m_vecAdj[iIndex].push_back(m_vecTile[iIndex - TILEX]);

				// ¦ -> Ȧ 21 ����
				else if ((0 == i % 2) && (!m_vecTile[iIndex - (TILEX + 1)]->byOption))
					m_vecAdj[iIndex].push_back(m_vecTile[iIndex - (TILEX + 1)]);
			}

			// ���� �̿��� ��Ģ

			// �� ������ �ƴ� �� && �� ������ ���� �ƴ� ��
			if ((0 != i) && ((TILEX * 2 - 1) != (iIndex % (TILEX * 2))))
			{
				// Ȧ -> ¦ 19 ����

				if ((0 != i % 2) && (!m_vecTile[iIndex - (TILEX - 1)]->byOption))
					m_vecAdj[iIndex].push_back(m_vecTile[iIndex - (TILEX - 1)]);

				// ¦ -> Ȧ 20 ����
				else if ((0 == i % 2) && (!m_vecTile[iIndex - TILEX]->byOption))
					m_vecAdj[iIndex].push_back(m_vecTile[iIndex - TILEX]);
			}

			// ���ϴ� �̿��� ��Ģ

			// �� �Ʒ����� �ƴ� �� && �� ���� ���� �ƴ� ��
			if ((TILEY - 1 != i) && (0 != (iIndex % (TILEX * 2))))
			{
				// Ȧ -> ¦ 20 ����

				if ((0 != i % 2) && (!m_vecTile[iIndex + TILEX]->byOption))
					m_vecAdj[iIndex].push_back(m_vecTile[iIndex + TILEX]);

				// ¦ -> Ȧ 19 ����
				else if ((0 == i % 2) && (!m_vecTile[iIndex + (TILEX - 1)]->byOption))
					m_vecAdj[iIndex].push_back(m_vecTile[iIndex + (TILEX - 1)]);
			}

			// ���ϴ� �̿��� ��Ģ

			// �� �Ʒ����� �ƴ� �� && �� ������ ���� �ƴ� ��
			if ((TILEY - 1 != i) && ((TILEX * 2 - 1) != (iIndex % (TILEX * 2))))
			{
				// Ȧ -> ¦ 21 ����

				if ((0 != i % 2) && (!m_vecTile[iIndex + (TILEX + 1)]->byOption))
					m_vecAdj[iIndex].push_back(m_vecTile[iIndex + (TILEX + 1)]);

				// ¦ -> Ȧ 20 ����
				else if ((0 == i % 2) && (!m_vecTile[iIndex + TILEX]->byOption))
					m_vecAdj[iIndex].push_back(m_vecTile[iIndex + TILEX]);
			}
		}
	}

	return S_OK;
}