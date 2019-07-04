#include "StdAfx.h"
#include "DragonEffect.h"

CDragonEffect::CDragonEffect(void)
{
}

CDragonEffect::~CDragonEffect(void)
{
}

void CDragonEffect::Initialize(void)
{
	m_tInfo.size.cx = 212.f;
	m_tInfo.size.cy = 205.f;

	m_dwFrameOldTime = GetTickCount();
	m_dwFrameCurTime = 0;

// 	if(DIR_LEFT == m_eDir )
// 	{
		m_pImgName = L"DragonEffect";
// 	}
// 	else
// 	{
// 		m_pImgName = L"SwingEffect_RIGHT";
// 	}

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 7;
	m_tFrame.iScene = 0;
	m_tFrame.dwFrameSpd = 150;

	m_eRenderType = RENDER_OBJ;

}

int CDragonEffect::Update(void)
{
	m_dwFrameCurTime = GetTickCount();

	if(m_dwFrameOldTime + m_tFrame.dwFrameSpd < m_dwFrameCurTime)
	{
		++(m_tFrame.iFrameStart);
		m_dwFrameOldTime = m_dwFrameCurTime;
	}

	if(m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
		return 1;

	//UpdateCollRect();

	CObj::UpdateRect();

	return 0;
}

void CDragonEffect::Render(HDC hDc)
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
}

void CDragonEffect::Release(void)
{

}

void CDragonEffect::FrameMove()
{

}

void CDragonEffect::UpdateCollRect()
{

}
