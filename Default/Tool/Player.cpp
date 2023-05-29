#include "stdafx.h"
#include "Player.h"


CPlayer::CPlayer()
	: CObj(OBJID::OBJ_PLAYER)
{
}


CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Initialize(void)
{
	return E_NOTIMPL;
}

int CPlayer::Update(void)
{
	return 0;
}

void CPlayer::Late_Update(void)
{
}

void CPlayer::Render(void)
{
}

void CPlayer::Release(void)
{
}
