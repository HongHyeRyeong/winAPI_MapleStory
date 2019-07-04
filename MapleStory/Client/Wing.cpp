#include "StdAfx.h"
#include "Wing.h"

CWing::CWing(void)
{
}

CWing::~CWing(void)
{
}

void CWing::Initialize(void)
{
	m_tInfo.size.cx = 659.f;
	m_tInfo.size.cy = 454.f;


	m_dwFrameOldTime = GetTickCount();
	m_dwFrameCurTime = 0;

	m_pImgName = L"Wing";
	// 
	// 	if(DIR_LEFT == m_eDir )
	// 		m_pImgName = L"Fire_LEFT";
	// 	else
	// 		m_pImgName = L"Fire_RIGHT";

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 16;
	m_tFrame.iScene = 0;
	m_tFrame.dwFrameSpd = 50;

	m_eRenderType = RENDER_OBJ;
}


int CWing::Update(void)
{
	m_dwFrameCurTime = GetTickCount();

	if(m_dwFrameOldTime + m_tFrame.dwFrameSpd < m_dwFrameCurTime)
	{
		++(m_tFrame.iFrameStart);
		m_dwFrameOldTime = m_dwFrameCurTime;
	}

	if(m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
		return 1;

	UpdateCollRect();


	CObj::UpdateRect();

	return 0;

}

void CWing::Render(HDC hDc)
{
	CMyBmp* pBit = CBitmapMgr::GetInstance()->FindImage(m_pImgName);

	if(NULL == pBit)  return;



	// Èì..¤Ð¤Ð
	TransparentBlt(hDc,
		static_cast<int>(m_tCollRect.left + g_fScrollX),
		static_cast<int>(m_tCollRect.top + g_fScrollY), 
		static_cast<int>(m_tInfo.size.cx),
		static_cast<int>(m_tInfo.size.cy),
		pBit->GetMemDC(),
		static_cast<int>(m_tFrame.iFrameStart * m_tInfo.size.cx),
		static_cast<int>(m_tFrame.iScene * m_tInfo.size.cy),
		static_cast<int>(m_tInfo.size.cx),
		static_cast<int>(m_tInfo.size.cy),
		RGB(0, 255, 0));

	// È÷Æ®¹Ú½º
	if(GetAsyncKeyState('2'))
	{
		Rectangle(hDc, 
			static_cast<int>(m_tCollRect.left + g_fScrollX),
			static_cast<int>(m_tCollRect.top + g_fScrollY), 
			static_cast<int>(m_tCollRect.right + g_fScrollX),
			static_cast<int>(m_tCollRect.bottom + g_fScrollY));
	}


}

void CWing::FrameMove()
{

}

void CWing::UpdateCollRect()
{
// 	m_tCollRect.left = m_tRect.left + g_fScrollX;
// 	m_tCollRect.top = m_tRect.top + g_fScrollY;
// 	m_tCollRect.bottom = m_tRect.bottom + g_fScrollY;
// 	m_tCollRect.right = m_tRect.right + g_fScrollX;

	m_tCollRect.top = static_cast<LONG>(m_tRect.top - g_fScrollY);
	m_tCollRect.bottom = static_cast<LONG>(m_tRect.bottom- g_fScrollY);
	m_tCollRect.left = static_cast<LONG>(m_tRect.left - g_fScrollX);
	m_tCollRect.right = static_cast<LONG>(m_tRect.right - g_fScrollX);

	// 	if(DIR_RIGHT == m_eDir)
	// 	{
	// 		m_tCollRect.left = static_cast<LONG>(m_tInfo.pt.x);
	// 		m_tCollRect.top = m_tRect.top;
	// 		m_tCollRect.bottom = m_tRect.bottom;
	// 		m_tCollRect.right = m_tRect.right;
	// 	}
	// 	else
	// 	{
	// 		m_tCollRect.right = static_cast<LONG>(m_tInfo.pt.x);
	// 		m_tCollRect.top = m_tRect.top;
	// 		m_tCollRect.bottom = m_tRect.bottom;
	// 		m_tCollRect.left = m_tRect.left;
	// 	}


}

void CWing::Release(void)
{

}
