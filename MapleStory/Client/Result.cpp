#include "stdafx.h"
#include "Result.h"
#include <algorithm>

CResult::CResult()
	: m_bIsShowed(false)
{
}

CResult::~CResult()
{
}

void CResult::Initialize(void)
{
	m_tInfo.pt.x = 400.f;
	m_tInfo.pt.y = 200.f;
	m_tInfo.size.cx = 600.f;
	m_tInfo.size.cy = 250.f;

	m_pImgName = L"Result";

	m_dwFrameOldTime = GetTickCount();
	m_dwFrameCurTime = 0;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iScene = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.dwFrameSpd = 200;

	m_eRenderType = RENDER_OBJ;

	UpdateRect();

	m_tExitButton.left = 360.f;
	m_tExitButton.right = 445.f;
	m_tExitButton.bottom = 300.f;
	m_tExitButton.top = 270.f;
}

int CResult::Update(void)
{
	if (CKeyMgr::GetInstance()->OnceKeyUp('P'))	// 임시
		SetShowed(!m_bIsShowed);

	// 몬스터의 모든 hp가 0이면 결과창 활성화
	for (int i = 0; i < g_vecgreen.size(); ++i)
	{
		if (g_vecgreen[i].hp > 0)
			break;

		SetShowed(true);
	}

	if (m_bIsShowed)
	{
		// 마우스가 들어오는지
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		if (PtInRect(&m_tExitButton, pt) &&
			CKeyMgr::GetInstance()->OnceKeyDown(VK_LBUTTON))
			g_bIsProgramEnd = true;	// 프로그램 종료
	}

	return 0;
}

void CResult::Render(HDC hDc)
{
	if (m_bIsShowed)
	{
		CMyBmp* pBit = CBitmapMgr::GetInstance()->FindImage(m_pImgName);
		if (NULL == pBit)  return;

		// 결과창
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

		vector<PLAYERINFO> temp = g_vecplayer;	// 킬 관여율로 정렬
		sort(begin(temp), end(temp), [](PLAYERINFO a, PLAYERINFO b) -> bool {
			return a.attackAccValue > b.attackAccValue;
		});

		// 파티원 정보, 킬 관여율
		for (int i = 0; i < temp.size(); ++i)
		{
			if (temp[i].id == -1)	// 플레이어 벡터에 값이 없다면
				continue;

			int y = 0;
			if (i == 0)	y = 190;
			else		y = 230;

			// name
			wchar_t name[512];
			mbstowcs(name, temp[i].nickname, strlen(temp[i].nickname) + 1);
			TextOut(hDc, 260, y, name, strlen(temp[i].nickname));

			// 직업
			if (temp[i].job == JOB_CAPTIN)	TextOut(hDc, 373, y, L"Captain", 7);
			else							TextOut(hDc, 373, y, L"Striker", 7);

			// 메소
			char bur[512];
			sprintf(bur, "%d", temp[i].money);

			wchar_t money[512];
			mbstowcs(money, bur, strlen(bur) + 1);

			TextOut(hDc, 475, y, money, strlen(bur));

			// 킬 관여율
			char bur2[512];
			sprintf(bur2, "%.1f", temp[i].attackAccValue);

			wchar_t kill[512];
			mbstowcs(kill, bur2, strlen(bur2) + 1);

			SetTextColor(hDc, RGB(255, 0, 0));
			TextOut(hDc, 580, y, kill, strlen(bur2));
			SetTextColor(hDc, RGB(0, 0, 0));
		}
	}
}

void CResult::SetShowed(bool bShow)
{
	m_bIsShowed = bShow;

	if (m_bIsShowed)
		CSoundMgr::GetInstance()->PlaySound(L"LevelUp.wav", CSoundMgr::CHANNEL_EFFECT);
}
