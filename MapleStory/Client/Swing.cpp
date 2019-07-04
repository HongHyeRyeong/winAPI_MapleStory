#include "StdAfx.h"
#include "Swing.h"

CSwing::CSwing(void)
{
}

CSwing::~CSwing(void)
{
}

void CSwing::Initialize(void)
{
	m_tInfo.size.cx = 206.f;
	m_tInfo.size.cy = 143.f;

	m_dwFrameOldTime = GetTickCount();
	m_dwFrameCurTime = 0;

	if(DIR_LEFT == m_eDir )
		m_pImgName = L"Swing_LEFT";
	else
		m_pImgName = L"Swing_RIGHT";

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 3;
	m_tFrame.iScene = 0;
	m_tFrame.dwFrameSpd = 150;

	m_eRenderType = RENDER_OBJ;
}

int CSwing::Update(void)
{
	this->SetPos(m_pPlayer->GetInfo().pt.x, m_pPlayer->GetInfo().size.cy);

	m_eDir = m_pPlayer->GetDir();

	if(DIR_LEFT == m_eDir )
		m_pImgName = L"Swing_LEFT";
	else
		m_pImgName = L"Swing_RIGHT";

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

void CSwing::Render(HDC hDc)
{
	CMyBmp* pBit = CBitmapMgr::GetInstance()->FindImage(m_pImgName);
	if (NULL == pBit)  return;

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
	if (GetAsyncKeyState('2'))
	{
		Rectangle(hDc,
			static_cast<int>(m_tCollRect.left + g_fScrollX),
			static_cast<int>(m_tCollRect.top + g_fScrollY),
			static_cast<int>(m_tCollRect.right + g_fScrollX),
			static_cast<int>(m_tCollRect.bottom + g_fScrollY));
	}
}

void CSwing::FrameMove()
{

}

void CSwing::Release(void)
{

}

void CSwing::UpdateCollRect()
{
	if(DIR_RIGHT == m_eDir)
	{
		m_tCollRect.left = static_cast<LONG>(m_tInfo.pt.x);
		m_tCollRect.top = m_tRect.top;
		m_tCollRect.bottom = m_tRect.bottom;
		m_tCollRect.right = m_tRect.right;
	}
	else
	{
		m_tCollRect.right = static_cast<LONG>(m_tInfo.pt.x);
		m_tCollRect.top = m_tRect.top;
		m_tCollRect.bottom = m_tRect.bottom;
		m_tCollRect.left = m_tRect.left;
	}
}
