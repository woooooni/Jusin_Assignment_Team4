#pragma once

class CObj;
class CToolObjMgr
{
	SINGLETON(CToolObjMgr);

private:
	list<CObj*> m_objList;

};

