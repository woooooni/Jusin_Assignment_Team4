#pragma once
#include "Obj.h"
class CPlayer :
	public CObj
{
public:
	explicit	CPlayer();
	virtual		~CPlayer();

public:
	// CObj��(��) ���� ��ӵ�
	virtual HRESULT		Initialize(void)	override;
	virtual int			Update(void)		override;
	virtual void		Late_Update(void)	override;
	virtual void		Render(void)		override;
	virtual void		Release(void)		override;


	
};

