#include "StdAfx.h"
#include "Maingame.h"
#include "Player.h"
#include "Fire.h"
#include "Field.h"
#include "Stage1.h"

// 서버와 통신
SOCKET g_sock;
int g_retval;
char buf[BUFSIZE];	// 데이터 버퍼
HANDLE hThread;

// 스레드 동기화
CRITICAL_SECTION cs;

//데이터타입 구분하기
int datatype;
PACKETINFO packetinfo;
PLAYERINFO g_myinfo;

// 플레이어들의 정보를 담는 벡터.
vector<PLAYERINFO> g_vecplayer;
int g_myid = -1;	// 내 플레이어 정보 key(인덱스 값)

//몬스터 정보 받기 
vector<MONSTERINFO> g_vecgreen(MAX_GREEN);

//
float g_fScrollX = 0.f;
float g_fScrollY = 0.f;
bool  g_bIsSceneChange = false;
SCENE_TYPE g_eScene = SCENE_FIELD;

void CMaingame::Initialize(void)
{
	// 임계 영역 초기화
	InitializeCriticalSection(&cs);

	m_hDC = GetDC(g_hWnd);

	// 콘솔창 디버깅용
	AllocConsole();
	freopen("CONOUT$", "wt", stdout);

	// 관련 이미지 로드
	CBitmapMgr::GetInstance()->GetMapBit().insert(make_pair(
		L"Back", (new CMyBmp)->LoadBmp(L"../Image/Back.bmp")));

	CBitmapMgr::GetInstance()->GetMapBit().insert(make_pair(
		L"BackBuffer", (new CMyBmp)->LoadBmp(L"../Image/BackBuffer.bmp")));

	// 씬 바꾸기 (Logo로)
	CSceneMgr::GetInstance()->SetScene(SCENE_LOGO);

	CollisionMgr::InitWELLRNG512((unsigned long)time(NULL));

	//-------------------------------------------------------------------------------
	// 서버 추가

	// 윈속을 초기화 한다.
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return;

	// 소켓 생성.
	g_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (g_sock == INVALID_SOCKET) // 생성 실패시
		MessageBoxW(g_hWnd, L"socket()", MB_OK, MB_OK);

	// recv 전용 스레드.
	hThread = CreateThread(NULL, 0, RecvThread, (LPVOID)g_sock, 0, NULL);
	if (NULL == hThread)	 
		CloseHandle(hThread);	


	// 플레이어 벡터 초기화 - id로 초기화 검사
	g_vecplayer.push_back(PLAYERINFO());
	g_vecplayer.push_back(PLAYERINFO());

	for (int i = 0; i < g_vecplayer.size(); ++i)
		g_vecplayer[i].id = -1;
}

void CMaingame::Update(void)
{
	//cout << "CMaingame::Update()" << endl;

	CSceneMgr::GetInstance()->Update();
	CSoundMgr::GetInstance()->UpdateSound();
}

void CMaingame::Render(void)
{
	// 더블 버퍼링
	HDC hBackBuff = (CBitmapMgr::GetInstance()->GetMapBit()[L"BackBuffer"])->GetMemDC();
	HDC hMemDC = (CBitmapMgr::GetInstance()->GetMapBit()[L"Back"])->GetMemDC();

	BitBlt(hBackBuff, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);

	CSceneMgr::GetInstance()->Render(hBackBuff);

	BitBlt(m_hDC, 0, 0, WINCX, WINCY, hBackBuff, 0, 0, SRCCOPY);
}

void CMaingame::Release(void)
{
	CKeyMgr::GetInstance()->DestroyInstance();
	CBitmapMgr::GetInstance()->DestroyInstance();
	CSoundMgr::GetInstance()->DestroyInstance();
	CSceneMgr::GetInstance()->DestroyInstance();
	ReleaseDC(g_hWnd, m_hDC);

	// 종료하기전에 서버에 알려주기
	char buf[BUFSIZE] = {};
	PACKETINFO packetinfo;
	packetinfo.id = g_myid;
	packetinfo.size = 0;
	packetinfo.type = SC_PACKET_CLIENT_END;
	memcpy(buf, &packetinfo, sizeof(packetinfo));

	g_retval = send(g_sock, buf, BUFSIZE, 0);
	if (g_retval == SOCKET_ERROR)
		MessageBox(g_hWnd, L"send()", L"send - SC_PACKET_CLIENT_END", MB_OK);

	closesocket(g_sock);
	WSACleanup();

	// 임계 영역 삭제
	DeleteCriticalSection(&cs);
}

DWORD WINAPI CMaingame::RecvThread(LPVOID arg)
{
	SOCKET client_sock = (SOCKET)arg;

	while (true) {
		EnterCriticalSection(&cs);

		// 아이디 부여 받기 전이면 ㄴㄴ.
		if (-1 == g_myid)
			continue;

		// 고정 길이.
		{
			g_retval = recvn(g_sock, buf, BUFSIZE, 0);
			memcpy(&packetinfo, buf, sizeof(packetinfo));
			if (g_retval == SOCKET_ERROR) {
				MessageBoxW(g_hWnd, L"recvn() - SC_PACKET_PLAYERINFO_ID", MB_OK, MB_OK);
				return 0;
			}
			else {
#ifdef DEBUG
				cout << "OTHER PLAYERINFO - 고정 길이를 받아왔어요!" << endl;
#endif
			}
		}

		// 가변 길이.
		switch (packetinfo.type)
		{
		case SC_PACKET_NEW_OTHER_PLAYERINFO:
		{
			cout << "SC_PACKET_NEW_OTHER_PLAYERINFO" << endl;
			// 새로 접속한 다른 플레이어 info를 받아온다.

			// 1. 고정 길이 패킷을 받아온다. (packetinfo)
			// 2. 새로 접속한 플레이어의 id를, 고정 길이 패킷을 통해 알아낸다.
			// 3. 가변 길이 패킷을 받아온다. (playerinfo)
			// 4. g_vecplayer[새로 접속한 플레이어 id]에 접근하여, playerinfo 정보를 갱신한다.
			// 5. ★ CreateObj로 CPlayer를 할당 하고, 정보로는 playerinfo를 넣는다.

			// ------------------Process---------------------
			// 필요한 변수들
			int newplayerid{ 0 };
			PLAYERINFO newplayerinfo = {};
			// 1. 새로 접속한 플레이어의 id를, 고정 길이 패킷을 통해 알아낸다.
			{
				newplayerid = packetinfo.id;
			}

			// 2. 가변 길이 패킷을 받아온다. (playerinfo)
			// 3. g_vecplayer[새로 접속한 플레이어 id]에 접근하여, playerinfo 정보를 갱신한다.
			{
				ZeroMemory(buf, sizeof(buf));
				g_retval = recvn(g_sock, buf, BUFSIZE, 0);
				if (g_retval == SOCKET_ERROR) {
					MessageBoxW(g_hWnd, L"recvn() - SC_PACKET_PLAYERINFO_ID", MB_OK, MB_OK);
					break;
				}
				else {
					memcpy(&newplayerinfo, buf, sizeof(newplayerinfo));
					g_vecplayer[newplayerid] = newplayerinfo;
#ifdef DEBUG
					cout << "NEW OTHER PLAYERINFO - 가변 길이를 받아왔어요!" << endl;
#endif
				}
			}
			// 4. ★ CreateObj로 CPlayer를 할당 하고, 정보로는 playerinfo를 넣는다.
			{
				CObj* player = CAbstractFactory<CPlayer>::CreatePlayer(newplayerinfo);
				CObjMgr::GetInstance()->AddObject(player, OBJ_PLAYER);

				/// 생성한 다른 클라이언트의 플레이어 포인터를 씬에서 저장한다.
				/// 나중에 OTHER_PLAYERINFO 패킷을 받았을 때, 바로 SetInfoPt를 해 주기 위해서임.
				if (SCENE_FIELD == g_eScene) {
					CScene* pScene = CSceneMgr::GetInstance()->GetScene();
					if (pScene != nullptr) {
						(dynamic_cast<CField*>(pScene))->SetOtherPlayer(dynamic_cast<CPlayer*>(player));
					}
				}
			}

			// 5. 확인해 본다.
			{
				//cout << "----------현재 접속 정보-----------" << endl;
				//cout << "[0번째 클라이언트] 닉네임 : " << g_vecplayer[0].nickname << ", 직업 : " << g_vecplayer[0].job << endl;
				//cout << "[1번째 클라이언트] 닉네임 : " << g_vecplayer[1].nickname << ", 직업 : " << g_vecplayer[1].job << endl;
			}
		}
		break;
		case SC_PACKET_OTHER_PLAYERINFO:
		{
			cout << "SC_PACKET_OTHER_PLAYERINFO" << endl;
			// 상태가 바뀐 다른 플레이어 info를 받아온다.
			int id = packetinfo.id; // 바꿀 클라이언트의 id를 받아온다.

			ZeroMemory(buf, sizeof(buf));
			g_retval = recvn(g_sock, buf, BUFSIZE, 0);
			if (g_retval == SOCKET_ERROR)
				MessageBoxW(g_hWnd, L"recvn() - SC_PACKET_OTHER_PLAYERINFO", MB_OK, MB_OK);
			else {
				memcpy(&(g_vecplayer[id]), buf, sizeof(g_vecplayer[id]));
#ifdef DEBUG
				cout << "OTHER PLAYERINFO - 가변 길이를 받아왔어요!" << endl;
#endif
				// 순서 문제 때문에 추가.
				/// Field 씬일 때, CField가 가지고 있는 CPlayer에 직접 접근하여 
				/// 서버로부터 받아온 좌표를 직접 넘겨준다.
				if (SCENE_FIELD == g_eScene) {
					CScene* pScene = CSceneMgr::GetInstance()->GetScene();
					if (pScene != nullptr) {
						CPlayer* pOtherPlayer = dynamic_cast<CField*>(pScene)->GetOtherPlayer();
						if (pOtherPlayer != nullptr) {
							pOtherPlayer->SetInfoPt(g_vecplayer[id].pt);
						}
					}
				}
				if (SCENE_STAGE1 == g_eScene) {
					CScene* pScene = CSceneMgr::GetInstance()->GetScene();
					if (pScene != nullptr) {
						CPlayer* pOtherPlayer = dynamic_cast<CStage1*>(pScene)->GetOtherPlayer();
						if (pOtherPlayer != nullptr) {
							pOtherPlayer->SetInfoPt(g_vecplayer[id].pt);
						}
					}
				}

			}
		}
		break;
		case SC_PACKET_YOUR_PLAYERINFO:
		{
			// 상태가 바뀐 나의 플레이어 info를 받아온다.
			int id = packetinfo.id; // 바꿀 클라이언트의 id를 받아온다.
			ZeroMemory(buf, sizeof(buf));
			g_retval = recvn(g_sock, buf, BUFSIZE, 0);
			if (g_retval == SOCKET_ERROR)
				MessageBoxW(g_hWnd, L"recvn() - SC_PACKET_YOUR_PLAYERINFO", MB_OK, MB_OK);
			else {
				memcpy(&(g_vecplayer[id]), buf, sizeof(g_vecplayer[id]));
#ifdef DEBUG
				cout << "YOUR_PLAYERINFO - 가변 길이를 받아왔어요!" << endl;
#endif
			}
			// 순서 문제 때문에 추가.
			/// Field 씬일 때, CField가 가지고 있는 CPlayer에 직접 접근하여 
			/// 서버로부터 받아온 좌표를 직접 넘겨준다.
			if (SCENE_FIELD == g_eScene) {
				CScene* pScene = CSceneMgr::GetInstance()->GetScene();
				if (pScene != nullptr) {
					CPlayer* pPlayer = dynamic_cast<CField*>(pScene)->GetPlayer();
					if (pPlayer != nullptr) {
						pPlayer->SetInfoPt(g_vecplayer[id].pt);
					}
				}
			}
			if (SCENE_STAGE1 == g_eScene) {
				CScene* pScene = CSceneMgr::GetInstance()->GetScene();
				if (pScene != nullptr) {
					CPlayer* pPlayer = dynamic_cast<CStage1*>(pScene)->GetPlayer();
					if (pPlayer != nullptr) {
						pPlayer->SetInfoPt(g_vecplayer[id].pt);
					}
				}
			}
		}
		break;
		case SC_PACKET_GRRENMUSH:
		{
			MONSTERPACKET temp_monster;
			int id = packetinfo.id;
			ZeroMemory(buf, sizeof(buf));
			g_retval = recvn(g_sock, (char*)&temp_monster, sizeof(MONSTERPACKET), 0);
			if (g_retval == SOCKET_ERROR)
				MessageBoxW(g_hWnd, L"recvn() - SC_MONSTER_ID", MB_OK, MB_OK);
			else
			{
				for (int i = 0; i < MAX_GREEN; ++i)
					g_vecgreen[i] = temp_monster.green[i];
			}

		}
		break;
		case SC_PACKET_SKILL_CREATE:
		{
			// 서버로부터 스킬 생성 명령을 받았다.
			// 1. 가변 길이 패킷을 받아온다.
			SKILLINFO skillinfo = {};
			{
				ZeroMemory(buf, BUFSIZE);
				g_retval = recvn(g_sock, buf, BUFSIZE, 0);
				if (g_retval == SOCKET_ERROR) {
					MessageBox(g_hWnd, L"recvn()", L"recvn() - SC_PACKET_SKILL_CREATE", NULL);
					break;
				}
				else
					memcpy(&skillinfo, buf, sizeof(skillinfo));
			}
			// 무슨 스킬인지 알아낸 후, 타입에 맞게 객체를 생성한다.
			{
				CObj* pSkill = nullptr;
				switch (skillinfo.type) {
				case SKILL_FIRE:
				{
					CObjMgr::GetInstance()->AddObject(CreateSkill<CFire>(skillinfo.pt, g_vecplayer[packetinfo.id].dir), OBJ_SKILL_FIRE);
				}
				break;
				}
			}
		}
		break;
		}

		LeaveCriticalSection(&cs);
	}

	return 0;
}

int CMaingame::recvn(SOCKET s, char *buf, int len, int flags)
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