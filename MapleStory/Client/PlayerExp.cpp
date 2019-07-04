#include "StdAfx.h"
#include "PlayerExp.h"

CPlayerExp::CPlayerExp(void)
{
}

CPlayerExp::~CPlayerExp(void)
{
}

void CPlayerExp::Initialize(void)
{
	m_tInfo.size.cx = 310.f;
	m_tInfo.size.cy = 14.f;
	m_tInfo.pt.x = 404.f;
	m_tInfo.pt.y = 585.f;

	m_iExp = 0;
	m_iExpMax = 3000;

	m_pImgName = L"PlayerEXPBar";

	m_eRenderType = RENDER_OBJ;

}

int CPlayerExp::Update(void)
{
	m_iExp = m_pPlayer->GetState().iExp;
	m_iExpMax = m_pPlayer->GetState().iMaxExp;

	UpdateRect();

	return 0;
}

void CPlayerExp::Render(HDC hDc)
{
	CMyBmp* pBit = CBitmapMgr::GetInstance()->FindImage(m_pImgName);

	if(NULL == pBit)  return;

	TransparentBlt(hDc,
		static_cast<int>(m_tRect.left),
		static_cast<int>(m_tRect.top), 
		static_cast<int>(m_tInfo.size.cx * ((float)g_iExp / (float)m_iExpMax)),
		static_cast<int>(m_tInfo.size.cy),
		pBit->GetMemDC(),
		0,
		0,
		static_cast<int>(m_tInfo.size.cx * ((float)g_iExp / (float)m_iExpMax)),
		static_cast<int>(m_tInfo.size.cy),
		RGB(0, 0, 255));

}

void CPlayerExp::FrameMove()
{

}

void CPlayerExp::Release(void)
{

}
