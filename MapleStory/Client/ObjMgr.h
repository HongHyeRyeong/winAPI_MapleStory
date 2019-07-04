#pragma once

class CObj;
class CObjMgr
{
	DECLARE_SINGLETON(CObjMgr)
private:
	CObjMgr(void);
	~CObjMgr(void);

public:
	void AddObject(CObj* pObj, OBJECT_TYPE eID);
	void UpdateObj();
	void RenderObj(HDC hDC);
	void ReleaseObj(OBJECT_TYPE eID);
	void ReleaseAll();

public:
	 CObj* GetTarget( CObj* pObj, OBJECT_TYPE eTargetID);

	 // 몬스터 좌표 얻기 위해서 만든 함수 
	 OBJLIST GetMonsterOBJList();


public:
	static bool CompareY(CObj* pDest, CObj* pSour);

private:
	OBJLIST			m_ObjList[OBJ_END];  
	vector<CObj*>	m_vecRender[RENDER_END];
	//list<CLine*>	m_LineList[LINE_END];


	// static 변수는 전역에다 초기화 (최초 한번 할당)

};
