#include "stdafx.h"
#include "Monster.h"


CMonster::CMonster()
	: CObj(OBJID::OBJ_MONSTER)
{

}


CMonster::~CMonster()
{
}

HRESULT CMonster::Initialize(void)
{
	return E_NOTIMPL;
}

int CMonster::Update(void)
{
	return 0;
}

void CMonster::Late_Update(void)
{
}

void CMonster::Render(void)
{
}

void CMonster::Release(void)
{
}
