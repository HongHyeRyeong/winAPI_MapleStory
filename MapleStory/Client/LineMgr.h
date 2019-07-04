#pragma once

class CLine;
class CLineMgr
{
	DECLARE_SINGLETON(CLineMgr)
private:
	CLineMgr(void);
	~CLineMgr(void);

public:
	void Initialize();
	void Render(HDC hDC);
	void Release();

public:
	bool LineCollision(CObj* pPlayer, float fX, float* pY);

private:
	void LoadData();

private:
	list<CLine*>	m_LineList[LINE_END];

	LINE_ID			m_eLineID;
};

