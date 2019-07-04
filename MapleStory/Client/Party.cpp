#include "stdafx.h"
#include "Party.h"

CParty::CParty()
	: m_bIsShowed(false)
{
}

CParty::~CParty()
{
}

void CParty::Initialize(void)
{
	m_tInfo.pt.x = WINCX * 0.8f;
	m_tInfo.pt.y = WINCY * 0.3f;
	m_tInfo.size.cx = 236.f;
	m_tInfo.size.cy = 342.f;

	UpdateRect();

	// 이미지
	m_pImgName = L"Party";

	m_dwFrameOldTime = GetTickCount();
	m_dwFrameCurTime = 0;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iScene = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.dwFrameSpd = 200;

	// 렌더 타입
	m_eRenderType = RENDER_OBJ;
}

int CParty::Update(void)
{
	CObj::UpdateRect();

	if (CKeyMgr::GetInstance()->OnceKeyUp('P'))
	{
		if (!m_bIsShowed)
			CSoundMgr::GetInstance()->PlaySound(L"OpenWnd.mp3", CSoundMgr::CHANNEL_EFFECT);
		else
			CSoundMgr::GetInstance()->PlaySound(L"CloseWnd.mp3", CSoundMgr::CHANNEL_EFFECT);

		m_bIsShowed = !m_bIsShowed;
	}

	return 0;
}

void CParty::Render(HDC hDc)
{
	CMyBmp* pBit = CBitmapMgr::GetInstance()->FindImage(m_pImgName);
	if (NULL == pBit)  return;

	if (m_bIsShowed)
	{
		// 파티창
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
			RGB(0, 255, 0));

		// 파티원 출력
		for (int i = 0; i < g_vecplayer.size(); ++i)
		{
			if (g_vecplayer[i].id == -1)	// 플레이어 벡터에 값이 없다면
				continue;

			int y = 0;
			if (i == 0)	y = 128;
			else		y = 145;

			// name
			wchar_t name[512];
			mbstowcs(name, g_vecplayer[i].nickname, strlen(g_vecplayer[i].nickname) + 1);
			TextOut(hDc, 540, y, name, strlen(g_vecplayer[i].nickname));

			// 직업
			if (g_vecplayer[i].job == JOB_CAPTIN)	TextOut(hDc, 640, y, L"Captain", 7);
			else									TextOut(hDc, 640, y, L"Striker", 7);

			// 상태
			if (g_vecplayer[i].ready == true)		TextOut(hDc, 715, y, L"O", 1);
			else									TextOut(hDc, 715, y, L"X", 1);
		}
	}
}