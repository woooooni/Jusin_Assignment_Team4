#pragma once

enum TEXTYPE	{ TEX_SINGLE, TEX_MULTI, TEX_END };

enum  OBJID     {  OBJ_PLAYER, OBJ_MONSTER, OBJ_BULLET, OBJ_MOUSE, OBJ_SHIELD, OBJ_UI, OBJ_END };
const wstring STR_OBJID[(UINT)OBJID::OBJ_END] = { L"�÷��̾�", L"����", L"����", L"����", L"����", L"����" };

enum EDIT_MODE { EDIT_OBJ, EDIT_MAP, EDIT_TILE, EDIT_END};