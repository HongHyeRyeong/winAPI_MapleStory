#include "StdAfx.h"
#include "Fire.h"

CFire::CFire(void)
{
}

CFire::~CFire(void)
{
}

void CFire::Initialize(void)
{

	m_tInfo.size.cx = 308.f;
	m_tInfo.size.cy = 291.f;

	m_dwFrameOldTime = GetTickCount();
	m_dwFrameCurTime = 0;

	m_pImgName = L"Fire";
 
 	//if(DIR_LEFT == m_eDir )
 	//	m_pImgName = L"Fire_LEFT";
 	//else
 	//	m_pImgName = L"Fire_RIGHT";

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 7;
	m_tFrame.iScene = 0;
	m_tFrame.dwFrameSpd = 100;

	m_eRenderType = RENDER_OBJ;
}

int CFire::Update(void)
{
 	//if(DIR_LEFT == m_eDir )
 	//	m_pImgName = L"Fire_LEFT";
 	//else
 	//	m_pImgName = L"Fire_RIGHT";

	m_dwFrameCurTime = GetTickCount();

	if(m_dwFrameOldTime + m_tFrame.dwFrameSpd < m_dwFrameCurTime)
	{
		++(m_tFrame.iFrameStart);
		m_dwFrameOldTime = m_dwFrameCurTime;
	}

	if(m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
		return 1;

	//cout << "m_tRect.left : " << m_tRect.left << endl;
	//cout << "m_tRect.top : " << m_tRect.top << endl;
	//cout << "m_tRect.right : " << m_tRect.right << endl;
	//cout << "m_tRect.bottom : " << m_tRect.bottom << endl;

//	CObj::UpdateRect();

	m_tRect.left = static_cast<LONG>(m_tInfo.pt.x - 208.f / 2);
	m_tRect.top = static_cast<LONG>(m_tInfo.pt.y - 251.f / 2);
	m_tRect.right = static_cast<LONG>(m_tInfo.pt.x + 208.f / 2);
	m_tRect.bottom = static_cast<LONG>(m_tInfo.pt.y + 251.f / 2);


	UpdateCollRect();


	return 0;

}

void CFire::Render(HDC hDc)
{
	CMyBmp* pBit = CBitmapMgr::GetInstance()->FindImage(m_pImgName);

	if(NULL == pBit)  return;


	TransparentBlt(hDc,
		static_cast<int>(m_tRect.left + g_fScrollX),
		static_cast<int>(m_tRect.top + g_fScrollY), 
		static_cast<int>(m_tInfo.size.cx),
		static_cast<int>(m_tInfo.size.cy),
		pBit->GetMemDC(),
		static_cast<int>(m_tFrame.iFrameStart * m_tInfo.size.cx),
		static_cast<int>(m_tFrame.iScene * m_tInfo.size.cy),
		static_cast<int>(m_tInfo.size.cx),
		static_cast<int>(m_tInfo.size.cy),
		RGB(0, 255, 0));

	// 히트박스
	if(GetAsyncKeyState('2'))
	{
		Rectangle(hDc, 
			static_cast<int>(m_tCollRect.left + g_fScrollX),
			static_cast<int>(m_tCollRect.top + g_fScrollY), 
			static_cast<int>(m_tCollRect.right + g_fScrollX),
			static_cast<int>(m_tCollRect.bottom + g_fScrollY));
	}

}

void CFire::FrameMove()
{

}

void CFire::UpdateCollRect()
{
	m_tCollRect = m_tRect;
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

void CFire::Release(void)
{

}
