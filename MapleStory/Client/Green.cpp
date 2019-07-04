#include "StdAfx.h"
#include "Green.h"
#include "Gold.h"


CGreen::CGreen(void)
{
}

CGreen::~CGreen(void)
{
}

void CGreen::Initialize( void )
{
	m_tInfo.size.cx = 60.f;
	m_tInfo.size.cy = 60.f;
	m_fSpeed = 4.f;

	m_tState.iAtt = 50;
	m_tState.iLevel = 15;
	m_tState.iExp = 100;
	m_tState.iMaxExp = 100;
	m_tState.iMaxHp = 300;

	m_eCurState = MONSTER_WALK;
	m_ePreState = m_eCurState;

	//
	m_dwCreateCurTime = GetTickCount();
	m_dwCreateOldTime = m_dwCreateCurTime;

	// 이미지
	m_dwFrameOldTime = GetTickCount();
	m_dwFrameCurTime = 0;

	if(m_eDir == DIR_LEFT)
		m_pImgName = L"GreenMush_LEFT";
	else if(m_eDir == DIR_RIGHT)
		m_pImgName = L"GreenMush_RIGHT";

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 4;
	m_tFrame.iScene = 1;
	m_tFrame.dwFrameSpd = 250;

	m_eRenderType = RENDER_OBJ;


	UpdateCollRect();
}

int CGreen::Update(void)
{
	if (m_bIsDead)
		return 0;

	if (m_tState.iHp <= 0)
	{
		m_bIsDead = true;
		m_eCurState = MONSTER_DEAD;

		if (true == m_bIsDead)
		{
			g_iTakedMob1++;
			g_iExp += m_tState.iExp;

			g_vecplayer[g_myid].attackAccValue += 100;

			CObjMgr::GetInstance()->AddObject(CreateGold<CGold>(), OBJ_ITEM);
			CSoundMgr::GetInstance()->PlaySound(L"MonsterDead.MP3", CSoundMgr::CHANNEL_EFFECT);

			return 1;
		}
	}

	UpdateCollRect();
	FrameMove();

	CObj::UpdateRect();

	return 0;
}

void CGreen::Render(HDC hDc)
{
	if (!m_bIsDead)
	{
		// 방향 설정
		if (m_eDir == DIR_LEFT)
			m_pImgName = L"GreenMush_LEFT";
		else if (m_eDir == DIR_RIGHT)
			m_pImgName = L"GreenMush_RIGHT";

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
			RGB(255, 0, 255));

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
}

void CGreen::FrameMove()
{
	if(m_eCurState != m_ePreState)
	{
		switch(m_eCurState)
		{
		case MONSTER_STAND:
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iScene = 3;
			break;
		case MONSTER_DAMAGED:
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iScene = 2;
			break;
		case MONSTER_WALK:
			m_tFrame.iFrameEnd = 4;
			m_tFrame.iScene = 1;
			break;
		case MONSTER_DEAD:
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iScene = 0;
			break;
		}

		m_ePreState = m_eCurState;
	}

	m_dwFrameCurTime = GetTickCount();

	if(m_dwFrameOldTime + m_tFrame.dwFrameSpd < m_dwFrameCurTime)
	{
		++(m_tFrame.iFrameStart);
		m_dwFrameOldTime = m_dwFrameCurTime;
	}

	if(m_eCurState == MONSTER_DEAD)
	{
		if(m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
			m_bIsDead = true;
	}

	if(m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
	{
		m_tFrame.iFrameStart = 0;
	}
}

void CGreen::Release( void )
{
}

void CGreen::LineCollision()
{
}

void CGreen::UpdateCollRect()
{
	m_tCollRect = m_tRect;
}