#include "StdAfx.h"
#include "Gold.h"

CGold::CGold(void)
: m_bIsXReverse(false)
{
}

CGold::~CGold(void)
{
}

void CGold::Initialize(void)
{
	m_tInfo.size.cx = 35.f;
	m_tInfo.size.cy = 35.f;

	m_fSpeed = 0.f;

	m_fAngle = 45.f;
	m_fXAngle = 30.f;

	m_fJumpAcc = 0.f;
	m_fJumpSpeed = 20.f;

	m_bIsJump = true;
	m_bIsJumpUp = true;

	m_pImgName = L"Gold";

	// 이미지
	m_dwFrameOldTime = GetTickCount();
	m_dwFrameCurTime = 0;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 3;
	m_tFrame.dwFrameSpd = 200;

	// 렌더 타입
	m_eRenderType = RENDER_OBJ;
}

int CGold::Update(void)
{
	if (m_bIsDead) return 1;

	CObj::UpdateRect();

	Jump();
	UpdateCollRect();	// 충돌박스 세팅	
	FrameMove();		// 프레임 돌리기

	return 0;
}

void CGold::Render(HDC hDc)
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

void CGold::FrameMove()
{
	m_dwFrameCurTime = GetTickCount();

	if (m_dwFrameOldTime + m_tFrame.dwFrameSpd < m_dwFrameCurTime)
	{
		++(m_tFrame.iFrameStart);
		m_dwFrameOldTime = m_dwFrameCurTime;
	}

	if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
		m_tFrame.iFrameStart = 0;
}

void CGold::Release(void)
{

}

void CGold::UpdateCollRect()
{
	m_tCollRect = m_tRect;
}

void CGold::Jump()
{
	if (m_bIsJump)
	{
		m_fJumpAcc += 0.25f;

		float fY = m_fJumpSpeed * m_fJumpAcc * sinf(m_fAngle * PI / 180.f)
			- GRAVITY * m_fJumpAcc * m_fJumpAcc * 0.5f;
		float fX = (m_fSpeed * cosf(m_fAngle * PI / 180.f));

		if (fY < 0)
			m_bIsJumpUp = false;
		else
			m_bIsJumpUp = true;

		m_tInfo.pt.y -= fY;
		m_tInfo.pt.x += fX;

		if (m_tInfo.pt.y > m_fOriginalY + 10.f && !m_bIsJumpUp)
		{
			m_fJumpAcc = 0.f;
			m_bIsJump = 0.f;
		}
	}
}

void CGold::SetGold(int iGold)
{
	m_iGold = iGold;
	m_tState.iGold = iGold;

	if (m_iGold > 0 && m_iGold <= 49)
		m_eID = GOLD_0;
	else if (m_iGold >= 50 && m_iGold <= 99)
		m_eID = GOLD_1;
	else if (m_iGold >= 100 && m_iGold <= 499)
		m_eID = GOLD_2;
	else if (m_iGold >= 500 && m_iGold <= 1000)
		m_eID = GOLD_3;

	switch (m_eID)
	{
	case GOLD_0:
		m_tFrame.iScene = 0;
		break;
	case GOLD_1:
		m_tFrame.iScene = 1;
		break;
	case GOLD_2:
		m_tFrame.iScene = 2;
		break;
	case GOLD_3:
		m_tFrame.iScene = 3;
		break;
	}
}
