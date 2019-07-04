#include "StdAfx.h"
#include "Mouse.h"

CMouse::CMouse(void)
{
}

CMouse::~CMouse(void)
{
}

void CMouse::Initialize(void)
{
	m_tInfo.size.cx = 26.f;
	m_tInfo.size.cy = 30.f;

	m_fSpeed = 0.f;

	m_eCurState = MOUSE_BASIC;
	m_ePreState = m_eCurState;

	m_pImgName = L"Mouse";

	m_dwFrameOldTime = GetTickCount();
	m_dwFrameCurTime = 0;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 1;
	m_tFrame.iScene = 0;
	m_tFrame.dwFrameSpd = 10;

	m_eRenderType = RENDER_OBJ;
}

int CMouse::Update(void)
{
	GetCursorPos(&m_ptMouse);
	ScreenToClient(g_hWnd, &m_ptMouse);

	if(m_eCurState == MOUSE_BASIC)
	{
		m_tInfo.pt.x = static_cast<float>(m_ptMouse.x);
		m_tInfo.pt.y = static_cast<float>(m_ptMouse.y);
	}
	else if(m_eCurState == MOUSE_GRAB)
	{
		m_tInfo.pt.x = static_cast<float>(m_ptMouse.x) - WINCX;
		m_tInfo.pt.y = static_cast<float>(m_ptMouse.y);
	}
	

	//m_eCurState = MOUSE_BASIC;

// 	if(CKeyMgr::GetInstance()->OnceKeyDown(VK_LBUTTON))
// 		m_eCurState = MOUSE_GRAB;
// 
// 	if(CKeyMgr::GetInstance()->OnceKeyUp(VK_LBUTTON))
// 		m_eCurState = MOUSE_BASIC;


	FrameMove();

	CObj::UpdateRect();
	return 0;
}

void CMouse::Render(HDC hDc)
{
	CMyBmp* pBit = 
		CBitmapMgr::GetInstance()->FindImage(m_pImgName);

	if(NULL == pBit)  return;

	// ** 付快胶 胶农费 贸府 林狼且巴
	TransparentBlt(hDc, static_cast<int>(m_tRect.left),
		static_cast<int>(m_tRect.top),
		static_cast<int>(m_tInfo.size.cx),
		static_cast<int>(m_tInfo.size.cy),
		pBit->GetMemDC(),
		static_cast<int>(m_tFrame.iFrameStart * m_tInfo.size.cx),
		static_cast<int>(m_tFrame.iScene * m_tInfo.size.cy),
		static_cast<int>(m_tInfo.size.cx),
		static_cast<int>(m_tInfo.size.cy),
		RGB(0, 255, 0));
}

void CMouse::Release(void)
{

}

void CMouse::FrameMove()
{
// 	if(m_eCurState != m_ePreState)
// 	{
		switch(m_eCurState)
		{
		case MOUSE_BASIC:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			//m_tFrame.iScene = 0;
			break;
		case MOUSE_GRAB:
			m_tFrame.iFrameStart = 1;
			m_tFrame.iFrameEnd = 1;
			//m_tFrame.iScene = 0;
			break;
		}
// 
// 		m_ePreState = m_eCurState;
// 	}

// 	m_dwFrameCurTime = GetTickCount();
// 
// 	if(m_dwFrameOldTime + m_tFrame.dwFrameSpd < m_dwFrameCurTime)
// 	{
// 		++(m_tFrame.iFrameStart);
// 		m_dwFrameOldTime = m_dwFrameCurTime;
// 	}
// 
// 	if(m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
// 		m_tFrame.iFrameStart = 0;
}
