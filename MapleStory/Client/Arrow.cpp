#include "StdAfx.h"
#include "Arrow.h"
#include "DragonEffect.h"

CArrow::CArrow(void)
:m_eID(ARROW_BASIC)
{
}

CArrow::~CArrow(void)
{
}

void CArrow::Initialize(void)
{
	m_tInfo.size.cx = 86.f;
	m_tInfo.size.cy = 23.f;

	m_fSpeed = 17.f;

	m_bCollMode = true;
	
	m_dwFrameCurTime = GetTickCount();
	m_dwFrameOldTime = m_dwFrameCurTime;

	if(DIR_LEFT == m_eDir )
		m_pImgName = L"Arrow_LEFT";
	else
		m_pImgName = L"Arrow_RIGHT";

	

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iScene = 0;
	m_tFrame.dwFrameSpd = 1500;

	m_eRenderType = RENDER_OBJ;


}

int CArrow::Update(void)
{
	
	/*this->SetPos(m_pPlayer->GetInfo().pt.x, m_pPlayer->GetInfo().size.cy);*/

	//m_eDir = m_pPlayer->GetDir();

	UpdateCollRect();

	if(DIR_LEFT == m_eDir )
		m_pImgName = L"Arrow_LEFT";
	else
		m_pImgName = L"Arrow_RIGHT";

	m_dwFrameCurTime = GetTickCount();


	if(DIR_LEFT == m_eDir)
		m_tInfo.pt.x -= m_fSpeed;
	else
		m_tInfo.pt.x += m_fSpeed;

	CObj::UpdateRect();

	if(m_dwFrameOldTime + 1000 < m_dwFrameCurTime)
	{
		return 1;
	}
	if(m_bIsDead)
	{
		if(ARROW_DRAGON == m_eID)
		{
			CObjMgr::GetInstance()->AddObject(CreateEffect<CDragonEffect>(),
				OBJ_EFFECT);
		}
		return 1;
	}

	return 0;
}

void CArrow::Render(HDC hDc)
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

	if(GetAsyncKeyState('2'))
	{
		Rectangle(hDc, 
			static_cast<int>(m_tCollRect.left + g_fScrollX),
			static_cast<int>(m_tCollRect.top + g_fScrollY), 
			static_cast<int>(m_tCollRect.right + g_fScrollX),
			static_cast<int>(m_tCollRect.bottom + g_fScrollY));
	}
}

void CArrow::Release(void)
{

}

void CArrow::FrameMove()
{

}

void CArrow::UpdateCollRect()
{
	m_tCollRect.top = m_tRect.top;
	m_tCollRect.bottom = m_tRect.bottom;
	m_tCollRect.left = m_tRect.left;
	m_tCollRect.right = m_tRect.right;
}
