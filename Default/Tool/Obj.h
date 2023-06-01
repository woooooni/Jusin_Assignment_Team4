#pragma once

#include "Include.h"

class CObj
{
public:
	CObj(OBJID _eID);
	virtual ~CObj();

public:
	const wstring&	Get_ObjName() { return m_strName; }
	void			Set_ObjName(wstring _strName) { m_strName = _strName; }

	const OBJID		Get_ObjID() { return m_eID; }
	void			Set_ObjID(OBJID _eID) { m_eID = _eID; }

	const wstring&	Get_ObjKey()const { return m_wstrObjKey; }

	const INFO&		Get_Info(void)const { return m_tInfo; }

	void			Set_Pos(const D3DXVECTOR3& vPos) { m_tInfo.vPos = vPos; }
	void			Set_Dir(const D3DXVECTOR3& vDir) 
	{ 
		m_tInfo.vDir = vDir;
		D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
	}

	void			Set_Size(const D3DXVECTOR3& _vSize) { m_tInfo.vSize = _vSize; }
	void			Set_Scale(const D3DXVECTOR3& _vScale) { m_tInfo.vScale = _vScale; }

	void			Set_Angle(float _f) { m_fAngle = _f; }
	float			Get_Angle() { return m_fAngle; }
public:
	virtual HRESULT		Initialize(void)	PURE;
	virtual int			Update(void)		PURE;
	virtual void		Late_Update(void)	PURE;
	virtual void		Render(void)		PURE;
	virtual void		Release(void)		PURE;

public:
	static	void		SetScroll(const D3DXVECTOR3& _vScroll) { m_vScroll = _vScroll; }
protected:
	void				Move_Frame(void);

protected:
	static D3DXVECTOR3			m_vScroll;

	INFO						m_tInfo;
	float						m_fAngle;
	FRAME						m_tFrame;
	wstring						m_wstrObjKey = L"";
	wstring						m_wstrStateKey = L"";

	wstring						m_strName = L"";
		
	OBJID						m_eID;
};

