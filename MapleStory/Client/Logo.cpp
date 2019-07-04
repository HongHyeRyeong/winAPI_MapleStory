#include "StdAfx.h"
#include "Logo.h"
#include "Mouse.h"
#include <comdef.h>


CLogo::CLogo(void)
{
}

CLogo::~CLogo(void)
{
	Release();
}

void CLogo::Initialize()
{
	// DC 받아오기.
	// m_hDC = GetDC(g_hWnd);
	m_hDC = (CBitmapMgr::GetInstance()->GetMapBit()[L"BackBuffer"])->GetMemDC(); // mem dc에 받아오는게 맞음.

	CSoundMgr::GetInstance()->PlayBGM(L"BGM_Logo.mp3");

	CBitmapMgr::GetInstance()->LoadImageByScene(SCENE_LOGO);

	// 마우스
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CMouse>::CreateObj(), OBJ_MOUSE);

	// Login
// 	CObjMgr::GetInstance()->AddObject(
// 		CAbstractFactory<CLogin>::CreateObj(), OBJ_BUTTON);


	m_dwFrameOldTime = GetTickCount();
	m_dwFrameCurTime = 0;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 9;
	m_tFrame.iScene = 0;
	m_tFrame.dwFrameSpd = 200;

	m_bIsMouseInExit = false;
	m_bIsMouseInLogin = false;


	m_ImgCX = 800;
	m_ImgCY = 600;


	m_pImgName = L"Logo1";

	m_eRenderType = RENDER_OBJ;

	// 버튼
	m_tLoginInfo.size.cx = 50.f;
	m_tLoginInfo.size.cy = 50.f;
	m_tLoginInfo.pt.x = 476.f;
	m_tLoginInfo.pt.y = 307.f;
	
	m_tLoginRect.left = static_cast<LONG>(m_tLoginInfo.pt.x - m_tLoginInfo.size.cx * 0.5f);
	m_tLoginRect.right = static_cast<LONG>(m_tLoginInfo.pt.x + m_tLoginInfo.size.cx * 0.5f);
	m_tLoginRect.top = static_cast<LONG>(m_tLoginInfo.pt.y - m_tLoginInfo.size.cy * 0.5f);
	m_tLoginRect.bottom = static_cast<LONG>(m_tLoginInfo.pt.y + m_tLoginInfo.size.cy * 0.5f);

	m_tExitInfo.size.cx = 69.f;
	m_tExitInfo.size.cy = 30.f;
	m_tExitInfo.pt.x = 466.f;
	m_tExitInfo.pt.y = 373.f;
	m_tExitRect.left = static_cast<LONG>(m_tExitInfo.pt.x - m_tExitInfo.size.cx * 0.5f);
	m_tExitRect.right = static_cast<LONG>(m_tExitInfo.pt.x + m_tExitInfo.size.cx * 0.5f);
	m_tExitRect.top = static_cast<LONG>(m_tExitInfo.pt.y - m_tExitInfo.size.cy * 0.5f);
	m_tExitRect.bottom = static_cast<LONG>(m_tExitInfo.pt.y + m_tExitInfo.size.cy * 0.5f);

}

int CLogo::Update()
{

	if(CKeyMgr::GetInstance()->OnceKeyUp(VK_RETURN))
	{
		CSceneMgr::GetInstance()->SetScene(SCENE_FIELD);
		CSoundMgr::GetInstance()->StopSoundAll();
		CSoundMgr::GetInstance()->PlayBGM(L"BGM_Field.wav");
		return 0;
	}

	m_dwFrameCurTime = GetTickCount();
	m_dwLogo2CurTime = GetTickCount();

	if(m_pImgName == L"Logo1")
	{
		if(m_dwFrameOldTime + m_tFrame.dwFrameSpd < m_dwFrameCurTime)
		{
			++(m_tFrame.iFrameStart);
			m_dwFrameOldTime = m_dwFrameCurTime;
		}
	}


	if(m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
	{
		m_pImgName = L"Logo2";

		m_ImgCX = 800;
		m_ImgCY = 600;

		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 0;
		m_tFrame.iScene = 0;

		m_dwLogo2OldTime = GetTickCount();
	}

	// 로고 전 이미지 잠깐 빠르게 지나가게,,
	if(m_dwLogo2OldTime + /*7000*/ 100 < m_dwLogo2CurTime)
	{
		m_pImgName = L"Logo3";
	}

	CObjMgr::GetInstance()->UpdateObj();


	return 0;
}

void CLogo::Render(HDC hDc)
{
	CMyBmp* pBmp = CBitmapMgr::GetInstance()->FindImage(m_pImgName);

	if(NULL == pBmp)
		return;

	TransparentBlt(hDc, 0, 0, WINCX, WINCY,
		pBmp->GetMemDC(), 
		m_ImgCX * m_tFrame.iFrameStart, 0, 
		m_ImgCX, WINCY, RGB(255, 255, 255));

	if(m_pImgName == L"Logo3")
	{
		if(true == MouseInLogin())
		{
			// login 버튼
			CMyBmp* pBmp2 = CBitmapMgr::GetInstance()->FindImage(L"Login");

			if(NULL == pBmp2)
				return;

			TransparentBlt(hDc, m_tLoginRect.left, m_tLoginRect.top, static_cast<int>(m_tLoginInfo.size.cx), static_cast<int>(m_tLoginInfo.size.cy),
				pBmp2->GetMemDC(), 
				0, 0, 
				static_cast<int>(m_tLoginInfo.size.cx), static_cast<int>(m_tLoginInfo.size.cy), NULL);
		}
	
		if(true == MouseInExit())
		{
			// exit 버튼
			CMyBmp* pBmp3 = CBitmapMgr::GetInstance()->FindImage(L"Exit");

			if(NULL == pBmp3)
				return;

			TransparentBlt(hDc, m_tExitRect.left, m_tExitRect.top, static_cast<int>(m_tExitInfo.size.cx), static_cast<int>(m_tExitInfo.size.cy),
				pBmp3->GetMemDC(), 
				0, 0, 
				static_cast<int>(m_tExitInfo.size.cx), static_cast<int>(m_tExitInfo.size.cy), NULL);
		}
		
	}


	CObjMgr::GetInstance()->RenderObj(hDc);

	//- ----------------------------------------------------
	// 서버 추가 : 사용자가 키보드로 입력한 IP 주소를 출력한다.
	RECT rc;
	rc.left = m_tLoginRect.left - 160;
	rc.right = m_tLoginRect.right - 50;
	rc.bottom = m_tLoginRect.bottom - 10;
	rc.top = m_tLoginRect.top + 20 ;

	// Debugging.
	// Rectangle(m_hDC, rc.left, rc.top, rc.right, rc.bottom);
	DrawText(m_hDC, g_ipbuf, wcslen(g_ipbuf), &rc, DT_SINGLELINE );

}

void CLogo::Release()
{
	CObjMgr::GetInstance()->ReleaseAll();
}

bool CLogo::MouseInLogin(void)
{
	POINT pt;
	POINT pt2;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	pt2.x = static_cast<LONG>(pt.x - g_fScrollX) - 10 + static_cast<LONG>(g_fScrollX);
	pt2.y = static_cast<LONG>(pt.y - g_fScrollY) - 21 + static_cast<LONG>(g_fScrollY);


	if(PtInRect(&m_tLoginRect, pt2))
	{
		if(m_bIsMouseInLogin == false)
		{	
			CSoundMgr::GetInstance()->PlaySound(L"Button.mp3", CSoundMgr::CHANNEL_EFFECT);
			m_bIsMouseInLogin = true;
		}

		if(CKeyMgr::GetInstance()->OnceKeyUp(VK_LBUTTON))
		{
			// 서버와의 연결을 설정 한다. (connect)
			SOCKADDR_IN serveraddr;
			ZeroMemory(&serveraddr, sizeof(serveraddr));
			serveraddr.sin_family = AF_INET;
			// convert wchar_t to char 
			_bstr_t b(g_ipbuf);
			serveraddr.sin_addr.s_addr = inet_addr(b);
			serveraddr.sin_port = htons(SERVERPORT);
			g_retval = connect(g_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));

			if (g_retval == SOCKET_ERROR)
				MessageBoxW(g_hWnd,L"connect()", L"connect()", MB_OK);

			// connectdp 성공하면
			else {
				CSceneMgr::GetInstance()->SetScene(SCENE_MAKINGPLAYER);
			}


		}

		return true;
	}
	else
		m_bIsMouseInLogin = false;

	return false;

}

bool CLogo::MouseInExit(void)
{
	POINT pt;
	POINT pt2;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	pt2.x = static_cast<LONG>(pt.x - g_fScrollX) - 10 + static_cast<LONG>(g_fScrollX);
	pt2.y = static_cast<LONG>(pt.y - g_fScrollY) - 21 + static_cast<LONG>(g_fScrollY);

	if(PtInRect(&m_tExitRect, pt2))
	{
		if(m_bIsMouseInExit == false)
		{	
			CSoundMgr::GetInstance()->PlaySound(L"Button.mp3", CSoundMgr::CHANNEL_EFFECT);
			m_bIsMouseInExit = true;
		}
		if(CKeyMgr::GetInstance()->OnceKeyUp(VK_LBUTTON))
		{
			DestroyWindow(g_hWnd);
		}

		return true;
	}
	else
	{
		m_bIsMouseInExit = false;
	}

	return false;
}
