#include "StdAfx.h"
#include "LeafCommunication.h"

CLeafCommunication::CLeafCommunication(void)
{
}

CLeafCommunication::~CLeafCommunication(void)
{
}

void CLeafCommunication::Initialize(void)
{
	m_tInfo.size.cx = 517.f;
	m_tInfo.size.cy = 187.f;	

	// 버튼 렉트
	m_bIsNextButtonActivated = false;

	// 이미지
	m_dwFrameOldTime = GetTickCount();
	m_dwFrameCurTime = 0;

	m_pImgName = L"Communication_Leaf";

	m_iRenderNumber= 0;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 3;
	m_tFrame.iScene = 0;
	m_tFrame.dwFrameSpd = 250;

	m_tInfo.pt.x = WINCX * 0.5f;
	m_tInfo.pt.y = WINCY * 0.5f;

	SetNextButtonRect();

	m_tInfo.pt.x -= g_fScrollX;
	m_tInfo.pt.y -= g_fScrollY;	

	UpdateRect();

	m_eRenderType = RENDER_OBJ;
}

int CLeafCommunication::Update(void)
{
	if (m_bIsDead)
		return 1;

	FrameMove();
	CObj::UpdateRect();

	// 마우스가 들어오는지
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	if (PtInRect(&m_tNextButton, pt) && m_bIsNextButtonActivated == false)
	{
		if (CKeyMgr::GetInstance()->OnceKeyDown(VK_LBUTTON)
			&& m_bIsNextButtonActivated == false)
		{
			if (m_iRenderNumber >= 2)
			{
				m_iRenderNumber = -1;

				g_bIsSend = true;

				// 파티 대기 상태로 바꾸기
				g_vecplayer[g_myid].ready = true;
			}
			else
				m_iRenderNumber++;
			m_bIsNextButtonActivated = true;
		}

		if (!CKeyMgr::GetInstance()->OnceKeyDown(VK_LBUTTON)
			&& m_bIsNextButtonActivated == true)
			m_bIsNextButtonActivated = false;
	}

	return 0;
}

void CLeafCommunication::Render(HDC hDc)
{
	CMyBmp* pBit = CBitmapMgr::GetInstance()->FindImage(m_pImgName);
	if(NULL == pBit)  return;

	TransparentBlt(hDc,
		static_cast<int>(m_tRect.left),
		static_cast<int>(m_tRect.top), 
		static_cast<int>(m_tInfo.size.cx),
		static_cast<int>(m_tInfo.size.cy),
		pBit->GetMemDC(),
		static_cast<int>(m_tFrame.iFrameStart * m_tInfo.size.cx),
		static_cast<int>(m_tFrame.iScene * m_tInfo.size.cy),
		static_cast<int>(m_tInfo.size.cx),
		static_cast<int>(m_tInfo.size.cy),
		RGB(255, 0, 255));
}

void CLeafCommunication::Release(void)
{

}

void CLeafCommunication::FrameMove()
{
	switch(m_iRenderNumber)
	{
	case 0:
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 0;
		break;
	case 1:
		m_tFrame.iFrameStart = 1;
		m_tFrame.iFrameEnd = 1;
		break;
	case 2:
		m_tFrame.iFrameStart = 2;
		m_tFrame.iFrameEnd = 2;
		break;
	case -1:
		m_bIsDead = 1;
		break;
	case 4:
		m_bIsDead = 1;
		break;
	}
}
