#include "StdAfx.h"
#include "PlayerMP.h"

CPlayerMP::CPlayerMP(void)
{
}

CPlayerMP::~CPlayerMP(void)
{
}

void CPlayerMP::Initialize(void)
{
	m_tInfo.size.cx = 141.f;
	m_tInfo.size.cy = 14.f;
	m_tInfo.pt.x = 490.3f;
	m_tInfo.pt.y = 567.5f;

	m_iMp = 8000;
	m_iMpMax = 8000;

	m_pImgName = L"PlayerMPBar";

	m_eRenderType = RENDER_OBJ;
}

int CPlayerMP::Update(void)
{
	m_iMp = m_pPlayer->GetState().iMp;
	m_iMpMax = m_pPlayer->GetState().iMaxMp;

	UpdateRect();
	return 0;

}

void CPlayerMP::Render(HDC hDc)
{
	CMyBmp* pBit = CBitmapMgr::GetInstance()->FindImage(m_pImgName);

	if(NULL == pBit)  return;

	TransparentBlt(hDc,
		static_cast<int>(m_tRect.left),
		static_cast<int>(m_tRect.top), 
		static_cast<int>(m_tInfo.size.cx * ( (float)m_iMp / (float)m_iMpMax)),
		static_cast<int>(m_tInfo.size.cy),
		pBit->GetMemDC(),
		0,
		0,
		static_cast<int>(m_tInfo.size.cx * ( (float)m_iMp / (float)m_iMpMax)),
		static_cast<int>(m_tInfo.size.cy),
		RGB(0, 0, 255));
}

void CPlayerMP::FrameMove()
{

}

void CPlayerMP::Release(void)
{

}
