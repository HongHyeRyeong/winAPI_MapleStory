#include "StdAfx.h"
#include "UI.h"

CUI::CUI(void)
{
}

CUI::~CUI(void)
{
}

void CUI::Initialize(void)
{
	m_tInfo.size.cx = WINCX;
	m_tInfo.size.cy = WINCY;
	m_tInfo.pt.x = WINCX * 0.5f;
	m_tInfo.pt.y = WINCY * 0.5f;

	m_pImgName = L"UI";

	m_eRenderType = RENDER_OBJ;

	// 레벨 (십의자리)
	m_tTensLvInfo.pt.x = 42.f;
	m_tTensLvInfo.pt.y = 575.f;
	m_tTensLvInfo.size.cx = 10.f;
	m_tTensLvInfo.size.cy = 12.f;

	m_tTensLvRect.top = static_cast<LONG>(m_tTensLvInfo.pt.y - m_tTensLvInfo.size.cy * 0.5f);
	m_tTensLvRect.bottom = static_cast<LONG>(m_tTensLvInfo.pt.y + m_tTensLvInfo.size.cy * 0.5f);
	m_tTensLvRect.left = static_cast<LONG>(m_tTensLvInfo.pt.x - m_tTensLvInfo.size.cx * 0.5f);
	m_tTensLvRect.right = static_cast<LONG>(m_tTensLvInfo.pt.x + m_tTensLvInfo.size.cx * 0.5f);

	m_tTensFrame.iFrameStart = 8;
	m_tTensFrame.iFrameEnd = 8;

	// 레벨 (일의 자리)
	m_tUnitLvInfo.pt.x = 54.f;
	m_tUnitLvInfo.pt.y = 575.f;
	m_tUnitLvInfo.size.cx = 10.f;
	m_tUnitLvInfo.size.cy = 12.f;

	m_tUnitLvRect.top = static_cast<LONG>(m_tUnitLvInfo.pt.y - m_tUnitLvInfo.size.cy * 0.5f);
	m_tUnitLvRect.bottom = static_cast<LONG>(m_tUnitLvInfo.pt.y + m_tUnitLvInfo.size.cy * 0.5f);
	m_tUnitLvRect.left = static_cast<LONG>(m_tUnitLvInfo.pt.x - m_tUnitLvInfo.size.cx * 0.5f);
	m_tUnitLvRect.right = static_cast<LONG>(m_tUnitLvInfo.pt.x + m_tUnitLvInfo.size.cx * 0.5f);

	m_tUnitFrame.iFrameStart = 8;
	m_tUnitFrame.iFrameEnd = 8;

	m_iTensLevel = static_cast<int>(g_iLevel * 0.1);
	int iTensNumber = static_cast<int>((g_iLevel * 0.1));
	m_iUnitLevel = g_iLevel - (iTensNumber) * 10;
}

int CUI::Update(void)
{
	// 레벨
	m_iTensLevel = static_cast<int>(g_iLevel * 0.1);
	int iTensNumber = static_cast<int>((g_iLevel * 0.1));
	m_iUnitLevel = g_iLevel - (iTensNumber) * 10;

	m_tUnitFrame.iFrameStart = m_iUnitLevel;
	m_tTensFrame.iFrameStart = m_iTensLevel;

	UpdateRect();
	return 0;
}

void CUI::Render(HDC hDc)
{
	// 메인 UI
	CMyBmp* pBit = CBitmapMgr::GetInstance()->FindImage(m_pImgName);
	if (NULL == pBit)  return;

	TransparentBlt(hDc,
		static_cast<int>(m_tRect.left),
		static_cast<int>(m_tRect.top),
		static_cast<int>(m_tInfo.size.cx),
		static_cast<int>(m_tInfo.size.cy),
		pBit->GetMemDC(),
		0,
		0,
		static_cast<int>(m_tInfo.size.cx),
		static_cast<int>(m_tInfo.size.cy),
		RGB(0, 0, 255));

	// 닉네임
	if (g_myid != -1)
	{
		wchar_t name[512];
		mbstowcs(name, g_vecplayer[g_myid].nickname, strlen(g_vecplayer[g_myid].nickname) + 1);

		SetBkColor(hDc, RGB(0, 0, 0));
		SetTextColor(hDc, RGB(255, 255, 255));
		TextOut(hDc, 80, 567, name, strlen(g_vecplayer[g_myid].nickname));
		SetBkColor(hDc, RGB(255, 255, 255));
		SetTextColor(hDc, RGB(0, 0, 0));
	}
	else
	{
		SetBkColor(hDc, RGB(0, 0, 0));
		SetTextColor(hDc, RGB(255, 255, 255));
		TextOut(hDc, 80, 567, L"None", 4);
		SetBkColor(hDc, RGB(255, 255, 255));
		SetTextColor(hDc, RGB(0, 0, 0));
	}

	// 레벨
	CMyBmp* pTensBit = CBitmapMgr::GetInstance()->FindImage(L"LvNum");
	if (NULL == pTensBit)  return;
	TransparentBlt(hDc,
		static_cast<int>(m_tTensLvRect.left),
		static_cast<int>(m_tTensLvRect.top),
		static_cast<int>(m_tTensLvInfo.size.cx),
		static_cast<int>(m_tTensLvInfo.size.cy),
		pTensBit->GetMemDC(),
		static_cast<int>(m_tTensLvInfo.size.cx * m_tTensFrame.iFrameStart),
		0,
		static_cast<int>(m_tTensLvInfo.size.cx),
		static_cast<int>(m_tTensLvInfo.size.cy),
		RGB(0, 0, 0));

	CMyBmp* pUnitBit = CBitmapMgr::GetInstance()->FindImage(L"LvNum");
	if (NULL == pUnitBit)  return;
	TransparentBlt(hDc,
		static_cast<int>(m_tUnitLvRect.left),
		static_cast<int>(m_tUnitLvRect.top),
		static_cast<int>(m_tUnitLvInfo.size.cx),
		static_cast<int>(m_tUnitLvInfo.size.cy),
		pUnitBit->GetMemDC(),
		static_cast<int>(m_tUnitLvInfo.size.cx * m_tUnitFrame.iFrameStart),
		0,
		static_cast<int>(m_tUnitLvInfo.size.cx),
		static_cast<int>(m_tUnitLvInfo.size.cy),
		RGB(0, 0, 0));
}

void CUI::FrameMove()
{

}

void CUI::Release(void)
{

}
