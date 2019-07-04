#include "stdafx.h"
#include "MakingPlayer.h"
#include "Mouse.h"
#include <comdef.h>

CMakingPlayer::CMakingPlayer()
{
}

CMakingPlayer::~CMakingPlayer()
{
	Release();
}

void CMakingPlayer::Initialize()
{
	m_hDC = (CBitmapMgr::GetInstance()->GetMapBit()[L"BackBuffer"])->GetMemDC();
	m_pImgName = L"MakingPlayer1";

	CBitmapMgr::GetInstance()->LoadImageByScene(SCENE_MAKINGPLAYER);

	// 마우스
	CObjMgr::GetInstance()->AddObject(
		CAbstractFactory<CMouse>::CreateObj(), OBJ_MOUSE);
}

int CMakingPlayer::Update()
{
	CObjMgr::GetInstance()->UpdateObj();

	// 직업 바꾸는 화살표 - 마우스 충돌 검사.
	RECT rcRight, rcLeft;
	POINT pt;
	POINT pt2;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	PLAYERINFO tempplayerinfo;

	pt2.x = static_cast<LONG>(pt.x - g_fScrollX) - 10 + static_cast<LONG>(g_fScrollX);
	pt2.y = static_cast<LONG>(pt.y - g_fScrollY) - 21 + static_cast<LONG>(g_fScrollY);

	rcRight.left = WINCX - 160; rcRight.right = WINCX - 120;
	rcRight.bottom = WINCY - 350; rcRight.top = WINCY - 380;
	rcLeft.left = WINCX - 260; rcLeft.right = WINCX - 230;
	rcLeft.bottom = WINCY - 350; rcLeft.top = WINCY - 380;

	if (PtInRect(&rcRight, pt2))
	{
		if (KEYMGR->OnceKeyDown(VK_LBUTTON)) {
			if (sceneNum) // 1
				sceneNum = 0;
			else
				sceneNum = 1;
		}
	}
	if (PtInRect(&rcLeft, pt2))
	{
		if (KEYMGR->OnceKeyDown(VK_LBUTTON)) {
			if (sceneNum) // 1
				sceneNum = 0;
			else
				sceneNum = 1;
		}
	}

	switch (sceneNum) {
	case 0:
		m_pImgName = L"MakingPlayer1";
		tempplayerinfo.job = JOB_STRIKER;
		break;
	case 1:
		m_pImgName = L"MakingPlayer2";
		tempplayerinfo.job = JOB_CAPTIN;
		break;
	}
	
	// 캐릭터 생성 버튼 - 마우스 충돌 검사.
	RECT rcMake{ WINCX - 300, WINCY - 180, WINCX - 200, WINCY - 130 };


	if (PtInRect(&rcMake, pt2)) {
		if (KEYMGR->OnceKeyDown(VK_LBUTTON)) {
			char buf[BUFSIZE];
			// 캐릭터 생성 버튼을 눌렀으면,
			// 1. 직업과 닉네임 정보가 채워진 playerinfo를 서버에 send 한다.
			// 2. 나머지 멤버 변수들이 채워진 playerinfo를 받는다. (여기서 id도 받는다.)
			
			// --------------------Process---------------------
			// 1. 직업과 닉네임 정보가 채워진 playerinfo를 서버에 send 한다.   
			while(true){
				// 입력 받은 id를 tempplayerinfo.id에 갱신한다.
				char* pStr;
				int strSize = WideCharToMultiByte(CP_ACP, 0, g_nicknamebuf, -1, NULL, 0, NULL, NULL);
				pStr = new char[strSize];
				WideCharToMultiByte(CP_ACP, 0, g_nicknamebuf, -1, pStr, strSize, 0, 0);
				memcpy(&(tempplayerinfo.nickname), (pStr), strSize);

				// 고정 길이.
				PACKETINFO packetinfo;
				packetinfo.type = CS_PACKET_PLAYERINFO_INITIALLY;
				packetinfo.size = sizeof(tempplayerinfo);
				memcpy(buf, &packetinfo, sizeof(packetinfo));
				g_retval = send(g_sock, buf, BUFSIZE, 0);
				if (g_retval == SOCKET_ERROR) {
					err_display("send() - 고정 - PLAYERINFO_INITIALLY");
					//MessageBoxW(g_hWnd, L"send()", L"send - 고정 - CS_PACKET_PLAYERINFO_INITIALLY", MB_OK);
					g_bIsProgramEnd = true;	// 프로그램 종료
					break;
				}
#ifdef DEBUG
				cout << "PLAYERINFO_INITIALLY - 고정 길이 패킷을 보냈어요!" << endl;
#endif
				// 가변 길이.
				ZeroMemory(buf, sizeof(buf));
				memcpy(buf, &tempplayerinfo, sizeof(tempplayerinfo));
				g_retval = send(g_sock, buf, BUFSIZE, 0);
				if (g_retval == SOCKET_ERROR) {
					err_display("send() - 고정 - PLAYERINFO_INITIALLY");
					//MessageBoxW(g_hWnd, L"send()", L"send - 가변 - CS_PACKET_PLAYERINFO_INITIALLY", MB_OK);
					g_bIsProgramEnd = true;	// 프로그램 종료
					break;
				}
				else {
#ifdef DEBUG
					cout << "PLAYERINFO_INITIALLY - 가변 길이 패킷을 보냈어요!" << endl;
#endif
					break;
				}
			}

			// 2. 나머지 멤버 변수들이 채워진 playerinfo를 받는다. (여기서 id도 받는다.)
			while(true){
				PACKETINFO temppacketinfo;
				PLAYERINFO tempplayerinfo;
				// 고정 길이. 
				/// 이렇게 안 해도 되긴 한데.. 안정성 있게 하자.
				ZeroMemory(buf, sizeof(buf));
				g_retval = recvn(g_sock, buf, BUFSIZE, 0);
				if (g_retval == SOCKET_ERROR) {
					MessageBoxW(g_hWnd, L"recv() - 고정 - SC_PACKET_YOUR_PLAYERINFO", MB_OK, MB_OK);
					g_bIsProgramEnd = true;	// 프로그램 종료
					break;
				}
				else {
					memcpy(&temppacketinfo, buf, sizeof(temppacketinfo));
#ifdef DEBUG
					cout << "YOUR_PLAYERINFO - 고정 길이 패킷을 받아왔어요!" << endl;
#endif
				}
				// 가변 길이. 
				if (temppacketinfo.type == SC_PACKET_YOUR_PLAYERINFO)
				{
					ZeroMemory(buf, sizeof(buf));
					g_retval = recvn(g_sock, buf, BUFSIZE, 0);
					if (g_retval == SOCKET_ERROR) {
						MessageBoxW(g_hWnd, L"recv() - 가변 - SC_PACKET_YOUR_PLAYERINFO", MB_OK, MB_OK);
						g_bIsProgramEnd = true;	// 프로그램 종료
						break;
					}
					else {
#ifdef DEBUG
						cout << "YOUR_PLAYERINFO - 가변 길이 패킷을 받아왔어요!" << endl;
#endif
						memcpy(&tempplayerinfo, buf, sizeof(tempplayerinfo));
						g_myid = tempplayerinfo.id;
						g_vecplayer[g_myid] = tempplayerinfo;

						// recv에 성공하면, 필드로 넘어간다.
						CSceneMgr::GetInstance()->SetScene(SCENE_FIELD);
						g_eScene = SCENE_FIELD;
						g_bIsSceneChange = true;
						CSoundMgr::GetInstance()->StopSoundAll();
						CSoundMgr::GetInstance()->PlaySound(L"Start.MP3", CSoundMgr::CHANNEL_EFFECT);
						CSoundMgr::GetInstance()->PlayBGM(L"BGM_Field.mp3");
						break;
					}
				}
				else
					break;
			}
		}
	}

	return 0;
}

void CMakingPlayer::Render(HDC hDc)
{
	CMyBmp* pBmp = CBitmapMgr::GetInstance()->FindImage(m_pImgName);
	if (NULL == pBmp)
		return;

	TransparentBlt(hDc, 0, 0, WINCX, WINCY,
		pBmp->GetMemDC(),
		0, 0,
		WINCX, WINCY, RGB(255, 255, 255));

	// 입력하는 id 출력.
	RECT rc = { WINCX - 240, WINCY - 313, WINCX - 100, WINCY - 293 };

	DrawText(m_hDC, g_nicknamebuf, wcslen(g_nicknamebuf), &rc, DT_SINGLELINE);
}

void CMakingPlayer::Release()
{

}

int CMakingPlayer::recvn(SOCKET s, char* buf, int len, int flags)
{
	int received;
	char *ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);

		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;

		else if (received == 0)
			break;

		left -= received;
		ptr += received;
	}
	return len - left;
}


// 소켓 함수 오류 출력
void CMakingPlayer::err_display(const char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}