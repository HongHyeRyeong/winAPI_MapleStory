#pragma once
#include "obj.h"

class CMouse :
	public CObj
{
public:
	CMouse(void);
	virtual ~CMouse(void);

public:
	virtual void Initialize(void);
	virtual int Update(void);
	virtual void Render(HDC hDc);
	virtual void Release(void);
	virtual void FrameMove();

public:
	static POINT GetMousePos()
	{
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		return pt;
	}

private:
	POINT		m_ptMouse;
	MOUSE_STATE m_eCurState;
	MOUSE_STATE m_ePreState;
};
