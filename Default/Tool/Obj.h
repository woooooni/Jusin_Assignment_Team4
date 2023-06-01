#pragma once

#include "Include.h"

class CTexture;
class CObj
{
public:
	CObj(OBJID _eID);
	virtual ~CObj();

public:
	const wstring&	Get_ObjName() { return m_strName; }
	void			Set_ObjName(wstring _strName) { m_strName = _strName; }

	const wstring&	Get_ObjState() { return m_wstrStateKey; }
	void			Set_ObjState(const wstring& _strKey) { m_wstrStateKey = _strKey; }

	const OBJID		Get_ObjID()					{ return m_eID; }
	void			Set_ObjID(OBJID _eID)		{ m_eID = _eID; }

	const wstring&	Get_ObjKey() const					{ return m_wstrObjKey; }
	void			Set_ObjKey(const wstring& _strKey)	{ m_wstrObjKey = _strKey; }

	const INFO&		Get_Info(void)const					{ return m_tInfo; }

	void			Set_Pos(const D3DXVECTOR3& vPos) { m_tInfo.vPos = vPos; }
	void			Set_Dir(const D3DXVECTOR3& vDir) 
	{ 
		m_tInfo.vDir = vDir;
		D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
	}

	void				Set_Size(const D3DXVECTOR3& _vSize) { m_tInfo.vSize = _vSize; }
	void				Set_Scale(const D3DXVECTOR3& _vScale) { m_tInfo.vScale = _vScale; }

	void				Set_Angle(float _f) { m_fAngle = _f; }
	float				Get_Angle() { return m_fAngle; }

	int					Get_AnimIdx()			{ return m_iAnimIdx; }
	void				Set_AnimIdx(int _iIdx)	{ m_iAnimIdx = _iIdx; }

public:
	virtual HRESULT		Initialize(void)	PURE;
	virtual int			Update(void)		PURE;
	virtual void		Late_Update(void)	PURE;
	virtual void		Render(void)		PURE;
	virtual void		Release(void)		PURE;

public:
	static	void				SetScroll(const D3DXVECTOR3& _vScroll)		{ m_vScroll = _vScroll; }
	

	
	const int					GetAnimInfoMapSize()	{ return m_mapAnimInfo.size(); }
	const int					GetAnimTexMapSize()		{ return m_mapAnimTex.size(); }

	void						InsertAnimationInfo(const wstring& _strObjKey, const vector<ANIMINFO_KJM> _vecAnim);
	void						InsertAnimTexture(const wstring& _strObjKey, const vector<CTexture*>& _vecAnim);

	const vector<ANIMINFO_KJM>& GetCurAnimInfoVec() { return m_mapAnimInfo.find(m_wstrStateKey)->second; }
	const vector<CTexture*>&	GetCurAnimTexVec()	{ return m_mapAnimTex.find(m_wstrStateKey)->second; }

protected:
	static D3DXVECTOR3					m_vScroll;

	INFO								m_tInfo;
	float								m_fAngle;

	wstring								m_wstrObjKey = L"";
	wstring								m_wstrStateKey = L"";
	wstring								m_wstrPath= L"";
	wstring								m_strName = L"";
		
	OBJID								m_eID;

	int									m_iAnimIdx;

	map<wstring, vector<ANIMINFO_KJM>>	m_mapAnimInfo;
	map<wstring, vector<CTexture*>>		m_mapAnimTex;

};

