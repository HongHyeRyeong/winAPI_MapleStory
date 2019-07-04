#include <winsock2.h>
#include <iostream>
#include <vector>
#include <time.h>
#include "Server.h"
#include "Include.h"
using namespace std;

#pragma comment(lib, "Ws2_32.lib")

#define DEBUG

// 클라이언트
vector<SOCKET> g_vecsocket;
vector<PLAYERINFO> g_vecplayer;
vector<SKILLINFO> g_vecskill;
bool g_arrayconnected[MAX_USER]; // connected 배열 (id 부여 위함)

// 몬스터
MONSTERPACKET g_monster_packet;

// 스레드 동기화
CRITICAL_SECTION cs;

//소켓 클라이언트 체크 

bool isEnd = false;

DWORD WINAPI ClientThread(LPVOID arg)
{
	SOCKET client_sock = (SOCKET)arg;
	SOCKADDR_IN clientaddr;
	// 클라이언트 정보 얻기
	int addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR *)&clientaddr, &addrlen);

	int retval{ 0 };
	PACKETINFO packetinfo;
	PLAYERINFO playerinfo;

	// -------------------------------------------
	// 구조 : 고정 길이 + 가변 길이.
	while (true) {
		cout << "--Player Thread--" << endl;
		
		cout << "플레이어벡터크기:" << g_vecplayer.size() << endl;
		char buf[BUFSIZE];

		// 고정 길이.
		// 앞으로 받을 패킷 정보를 알아낸다.
		{
			ZeroMemory(&packetinfo, sizeof(packetinfo));
			retval = recvn(client_sock, buf, BUFSIZE, 0);
			if (retval == SOCKET_ERROR) {
				err_display("packetinfo recv()");
				break;
			}
			else
				memcpy(&packetinfo, buf, sizeof(packetinfo));
#ifdef DEBUG
			cout << "고정 길이 패킷을 받아왔어요!" << endl;
#endif
		}

		// 가변 길이.
		switch (packetinfo.type) {
		case CS_PACKET_PLAYERINFO_INITIALLY:
		{
			EnterCriticalSection(&cs);
			// 클라이언트가 초기 설정 한 playerinfo를 recvn 한다.
			// 1. 초기 설정된 playeinfo를 수신한다.
			// 2. playerinfo의 나머지 멤버 변수들을 채워준다. (id, size, hp, mp, ..)
			// 3. 나머지 멤버 변수들이 채워진 playerinfo를 벡터에 push 한다.
			// 4. 클라이언트에게 "니 정보는 이거야!" 라고 send 하여 알린다.
			// 5. 나머지 다른 클라이언트에게 나의 playerinfo를 전송하여 나의 접속을 알린다. (2가 접속했다면 1에게, 1이 접속했다면 아무에게도 X)
			// 6. 새 클라이언트에게 이미 존재하는 클라이언트의 정보를 send 한다.

			// --------------------Process---------------------
			// 1. 초기 설정된 playeinfo를 수신한다.
			{
				ZeroMemory(buf, sizeof(buf));	 /// 재사용 할 거니까. 계속 비워줌.		
				retval = recvn(client_sock, buf, BUFSIZE, 0);
				if (retval == SOCKET_ERROR) {
					err_display("intial playerinfo recv()");
					break;
				}
				else {
					/// 고정 길이 데이터에 recv 해온 메모리만큼 복사 한다.
					ZeroMemory(&playerinfo, sizeof(playerinfo));
					memcpy(&playerinfo, buf, sizeof(playerinfo));
#ifdef DEBUG
					cout << "가변 길이 - 초기 생성된 playerinfo를 수신했어요!" << endl;
#endif
				}
			}

			// 2. playerinfo의 나머지 멤버 변수들을 채워준다. (id, size, hp, mp, ..)
			{
				int id = -1;
				for (int i = 0; i < MAX_USER; ++i) {
					if (false == g_arrayconnected[i]) { /// 순차적으로 접근해서 연결 되지 않은 인덱스 i를 찾고, 
						g_arrayconnected[i] = true;
						id = i; /// 아이디를 부여한다.
#ifdef DEBUG		
						cout << "접속한 플레이어에게 id를 부여했습니다!" << endl;
#endif
						break;
					}
				}

				/// 유저 2인 제한.
				if (-1 == id) {
					cout << "user가 다 찼습니다." << std::endl;
					closesocket(client_sock);
					break;
				}

				// 실제 playerinfo에 얻은 id를 부여한다.
				playerinfo.id = id;
				playerinfo.connected = true;
				playerinfo.ready = false;
				playerinfo.pt.x = 100.f; // 초기 좌표는 (100, 500)
				playerinfo.pt.y = 500.f;
				playerinfo.hp = 30000;
				playerinfo.size.cx = 100.f;
				playerinfo.size.cy = 100.f;
				playerinfo.attackAccValue = 0.f;
				playerinfo.money = 0;
				playerinfo.frame = { 0, 0, 0, 50 };
				playerinfo.state = PLAYER_STAND;
				playerinfo.prestate = PLAYER_STAND;
			}

			// 3. 나머지 멤버 변수들이 채워진 playerinfo를 벡터에 push 한다.
			{
#ifdef DEBUG
				cout << "정보가 채워진 playerinfo를 g_vecplayer에 push 했어요!" << endl;
#endif
				g_vecplayer.push_back(playerinfo);
				
			}

			// 4. 새로 접속한 클라이언트에게 "니 정보는 이거야!" 라고 send 하여 알린다.
			{
				// 고정 길이. 
				ZeroMemory(&packetinfo, sizeof(packetinfo));
				packetinfo.id = playerinfo.id;
				packetinfo.size = sizeof(playerinfo);
				packetinfo.type = SC_PACKET_YOUR_PLAYERINFO;
				ZeroMemory(buf, sizeof(buf));
				memcpy(buf, &packetinfo, sizeof(packetinfo));
				retval = send(client_sock, buf, BUFSIZE, 0);
				if (retval == SOCKET_ERROR) {
					cout << ("send() 실패 - 고정 길이 - SC_PACKET_YOUR_PLAYERINFO") << endl;
					break;
				}
#ifdef DEBUG
				cout << "클라이언트에게 YOUR_PLAYERINFO를 고정 길이 전송했어요!" << endl;
#endif

				// 가변 길이. 
				ZeroMemory(buf, sizeof(buf));
				memcpy(buf, &(playerinfo), sizeof(playerinfo));
				retval = send(client_sock, buf, BUFSIZE, 0);
				if (retval == SOCKET_ERROR) {
					err_display("send() 실패 - 가변 길이 - SC_PACKET_YOUR_PLAYERINFO");
					break;
				}
#ifdef DEBUG
				cout << "클라이언트에게 YOUR_PLAYERINFO를 가변 길이 전송했어요!" << endl;
#endif

			}

			// 5. 기존에 접속해 있는 클라이언트에게 새 클라이언트의 playerinfo를 전송한다.
			if (g_vecplayer.size() >= 2) {
				/// 고정 길이.
				ZeroMemory(&packetinfo, sizeof(packetinfo));
				packetinfo.type = SC_PACKET_NEW_OTHER_PLAYERINFO;
				packetinfo.size = sizeof(playerinfo);
				packetinfo.id = playerinfo.id; /// playerinfo.id가 뉴비의 아이디임.
				ZeroMemory(buf, sizeof(buf));
				memcpy(buf, &packetinfo, sizeof(packetinfo));

				int recvid;  // 받을 클라이언트의 id는?
				if (playerinfo.id == 0) recvid = 1; else recvid = 0;
				retval = send(g_vecsocket[recvid], buf, BUFSIZE, 0);
				if (retval == SOCKET_ERROR) {
					err_display("send() - SC_PACKET_NEW_OTHER_PLAYERINFO");
					break;
				}
#ifdef DEBUG
				cout << recvid << "번째 클라이언트에게 " << playerinfo.id << "의 OTHER_PLAYERINFO를 고정 길이 전송했어요!" << endl;
#endif
				/// 가변 길이.
				ZeroMemory(buf, sizeof(buf)); /// 버퍼 재사용.
				memcpy(buf, &playerinfo, sizeof(g_vecplayer[playerinfo.id]));
				retval = send(g_vecsocket[recvid], buf, BUFSIZE, 0);
				if (retval == SOCKET_ERROR) {
					err_display("send() - SC_PACKET_PLAYERINFO");
					break;
				}
#ifdef DEBUG
				cout << recvid << "번째 클라이언트에게 " << playerinfo.id << "의 OTHER_PLAYERINFO를 가변 길이 전송했어요!" << endl;
#endif
			}

			// 6. 새 클라이언트에게 이미 존재하는 클라이언트의 정보를 send 한다.
			if (g_vecplayer.size() >= 2) {
				// 동접 플레이어가 2일 때만 해당한다. 즉 1번째 클라이언트만 해당됨.
				// 고정 길이.
				PACKETINFO temppacketinfo = {};
				temppacketinfo.id = 0;// 1번째 클라이언트는 0번째 클라이언트의 정보를 받아야 한다.
				temppacketinfo.size = sizeof(g_vecplayer[temppacketinfo.id]);
				temppacketinfo.type = SC_PACKET_NEW_OTHER_PLAYERINFO;
				ZeroMemory(buf, sizeof(buf));
				memcpy(buf, &temppacketinfo, sizeof(temppacketinfo));
				retval = send(g_vecsocket[1], buf, BUFSIZE, 0);
				if (retval == SOCKET_ERROR) {
					err_display("send() - SC_PACKET_NEW_OTHER_PLAYERINFO");
					break;
				}
				else {
#ifdef DEBUG
					cout << "새로 접속한 [" << 1 << "] 에게 이미 존재한 [" << temppacketinfo.id << "] 번째 클라이언트의 고정 길이를 보냈어요!" << endl;
#endif 
				}

				// 가변 길이.
				ZeroMemory(buf, sizeof(buf));
				memcpy(buf, &(g_vecplayer[temppacketinfo.id]), sizeof(g_vecplayer[temppacketinfo.id]));
				retval = send(g_vecsocket[1], buf, BUFSIZE, 0);
				if (retval == SOCKET_ERROR) {
					err_display("send() - SC_PACKET_NEW_OTHER_PLAYERINFO");
					break;
				}
				else {
#ifdef DEBUG
					cout << "새로 접속한 [" << packetinfo.id << "] 에게 이미 존재한 [" << temppacketinfo.id << "] 번째 클라이언트의 고정 길이를 보냈어요!" << endl;
#endif 
				}
			}
			LeaveCriticalSection(&cs);
		}
		break;

		case CS_PACKET_PLAYERINFO_MOVE:
		{
			EnterCriticalSection(&cs);

			// 플레이어가 움직였을 때 패킷을 보내어 들어오는 부분.
			// 1. 고정 길이 패킷에서, 어느 클라에 해당되는지 id 알아옴.
			// 2. g_vecplayer[받은 playerinfo의 id] 에 접근하여 정보를 갱신한다.

			//
			// 2. 가변 길이 패킷을 받아 playerinfo을 받는다.
			// 3. g_vecplayer[받은 playerinfo의 id] 에 접근하여 정보를 갱신한다.
			// 4. 다른 클라이언트에게도 보낸다. (SC_PACKET_OTHER_PLAYERINFO)
			// 5. 움직인 클라이언트에게도 보낸다. (SC_PACKET_YOUR_PLAYERINFO)

			// -------------------Process---------------------
			// 1. 고정 길이 패킷에서, 어느 클라에 해당되는지 id 알아옴.
			int id = packetinfo.id;
			// 2. 가변 길이 패킷을 받아 playerinfo를 받는다.
			PLAYERINFO tempplayerinfo = {};
			{
				ZeroMemory(buf, sizeof(buf));
				retval = recvn(client_sock, buf, BUFSIZE, 0);
				if (retval == SOCKET_ERROR) {
					cout << "실패 - recvn - CS_PACKET_PLAYERINFO_MOVE" << endl;
					break;
				}
				else
					memcpy(&tempplayerinfo, buf, sizeof(tempplayerinfo));
#ifdef DEBUG
				cout << id << "번째 클라이언트가 움직였으므로 가변 길이 패킷을 받아왔어요!" << endl;
#endif
			}

			// 3. g_vecplayer[받은 playerinfo의 id]에 접근하여 정보를 갱신한다.
			{
				g_vecplayer[id] = tempplayerinfo;
			}

			// 4. 다른 클라이언트에게도 보낸다. (SC_PACKET_OTHER_PLAYERINFO)
			if (g_vecplayer.size() == 2) { // 2인 접속 시에만.
				// 고정 길이. 
				ZeroMemory(&packetinfo, sizeof(packetinfo));
				packetinfo.id = id;
				packetinfo.size = sizeof(PLAYERINFO);
				packetinfo.type = SC_PACKET_OTHER_PLAYERINFO;
				memcpy(buf, &packetinfo, sizeof(packetinfo));
				if (0 == id) {	// 0 정보는 1에게 보내야 한다.
					retval = send(g_vecsocket[id + 1], buf, BUFSIZE, 0);
				}
				else if (1 == id) {
					retval = send(g_vecsocket[id - 1], buf, BUFSIZE, 0);
				}
				if (retval == SOCKET_ERROR) {
					cout << "failed : send - 고정 - SC_PACKET_OTHER_PLAYERINFO" << endl;
					break;
				}
				else {
#ifdef DEBUG
					cout << id << "번째 클라이언트가 움직였으므로 " << "다른 클라이언트에게 고정 길이 패킷을 전송합니다!" << endl;
#endif
				}

				// 가변 길이
				ZeroMemory(buf, sizeof(buf));
				memcpy(buf, &(g_vecplayer[id]), sizeof(playerinfo));
				if (0 == id) {	// 0 정보는 1에게 보내야 한다.
					retval = send(g_vecsocket[id + 1], buf, BUFSIZE, 0);
				}
				else if (1 == id) {
					retval = send(g_vecsocket[id - 1], buf, BUFSIZE, 0);
				}
				if (retval == SOCKET_ERROR) {
					cout << "failed : send - 가변 - SC_PACKET_OTHER_PLAYERINFO" << endl;
					break;
				}
				else {
#ifdef DEBUG
					cout << id << "번째 클라이언트가 움직였으므로 " << "다른 클라이언트에게 가변 길이 패킷을 전송합니다!" << endl;
#endif
				}

			}

			// 5. 움직인 클라이언트에게도 보낸다. (SC_PACKET_YOUR_PLAYERINFO)
//			 {  
//				// 고정 길이. 
//				ZeroMemory(&packetinfo, sizeof(packetinfo));
//				packetinfo.id = id;
//				packetinfo.size = sizeof(PLAYERINFO);
//				packetinfo.type = SC_PACKET_YOUR_PLAYERINFO;
//				memcpy(buf, &packetinfo, sizeof(packetinfo));
//					retval = send(g_vecsocket[id], buf, BUFSIZE, 0);
//				if (retval == SOCKET_ERROR) {
//					cout << "failed : send - 고정 - SC_PACKET_OTHER_PLAYERINFO" << endl;
//					break;
//				}
//				else {
//#ifdef DEBUG
//					cout << id << "번째 클라이언트가 움직였으므로 " << "해당 클라이언트에게 고정 길이 패킷을 전송합니다!" << endl;
//#endif
//				}
//
//				// 가변 길이
//				ZeroMemory(buf, sizeof(buf));
//				memcpy(buf, &(g_vecplayer[id]), sizeof(playerinfo));
//				retval = send(g_vecsocket[id], buf, BUFSIZE, 0);
//				if (retval == SOCKET_ERROR) {
//					cout << "failed : send - 가변 - SC_PACKET_OTHER_PLAYERINFO" << endl;
//					break;
//				}
//				else {
//#ifdef DEBUG
//					cout << id << "번째 클라이언트가 움직였으므로 " << "해당 클라이언트에게 가변 길이 패킷을 전송합니다!" << endl;
//#endif
//				}
//
//			}

			LeaveCriticalSection(&cs);
		}
		break;

		case CS_PACKET_PLAYERINFO_INCHANGINGSCENE:
		{
			EnterCriticalSection(&cs);
			// 씬이 바뀌었을 때, 클라이언트는 이 패킷을 한번 보낸다.
			// 1. 고정 길이 패킷에서, 어느 클라에 해당되는지 id 알아온다.
			// 2. 가변 길이 패킷을 받고, 받아온 정보대로 playerinfo를 저장한다.
			// 3. g_vecplayer[받은 playerinfo의 id]에 접근하여 정보를 갱신한다.
			// 4. 모든 클라이언트에게 g_vecplayer[0], g_vecplayer[1] 정보를 보낸다.

			// ----------------------- Process -------------------------
			// 1. 고정 길이 패킷에서, 어느 클라에 해당되는지 id 알아온다.
			int id = packetinfo.id;
			// 2. 가변 길이 패킷을 받고, 받아온 정보대로 playerinfo를 저장한다.
			PLAYERINFO tempplayerinfo = {};
			{
				ZeroMemory(buf, sizeof(buf));
				retval = recvn(client_sock, buf, BUFSIZE, 0);
				if (retval == SOCKET_ERROR) {
					cout << "실패 - recvn - CS_PACKET_PLAYERINFO_INCHANGINGSCENE" << endl;
					break;
				}
				else
					memcpy(&tempplayerinfo, buf, sizeof(tempplayerinfo));
#ifdef DEBUG
				cout << id << "번째 클라이언트의 씬이 바뀌었으므로 가변 길이 패킷을 받아왔어요!" << endl;
#endif
			}
			// 3. g_vecplayer[받은 playerinfo의 id]에 접근하여 정보를 갱신한다.
			{
				g_vecplayer[id] = tempplayerinfo;
			}
			// 4. 다른 클라이언트에게 g_vecplayer[0], g_vecplayer[1] 정보를 보낸다.
			{
				if (g_vecplayer.size() == 2) { // 2인 접속 시에만.
					// 고정 길이. 
					ZeroMemory(&packetinfo, sizeof(packetinfo));
					packetinfo.id = id;
					packetinfo.size = sizeof(PLAYERINFO);
					packetinfo.type = SC_PACKET_OTHER_PLAYERINFO;
					memcpy(buf, &packetinfo, sizeof(packetinfo));
					if (0 == id) {	// 0 정보는 1에게 보내야 한다.
						retval = send(g_vecsocket[id + 1], buf, BUFSIZE, 0);
					}
					else if (1 == id) { // 1 정보는 0에게 보낸다.
						retval = send(g_vecsocket[id - 1], buf, BUFSIZE, 0);
					}
					if (retval == SOCKET_ERROR) {
						cout << "failed : send - 고정 - SC_PACKET_OTHER_PLAYERINFO" << endl;
						break;
					}
					else {
#ifdef DEBUG
						cout << id << "번째 클라이언트의 씬이 바뀌었으므로 " << "다른 클라이언트에게 고정 길이 패킷을 전송합니다!" << endl;
#endif
					}

					// 가변 길이
					ZeroMemory(buf, sizeof(buf));
					memcpy(buf, &(g_vecplayer[id]), sizeof(playerinfo));
					if (0 == id) {	// 0 정보는 1에게 보내야 한다.
						retval = send(g_vecsocket[id + 1], buf, BUFSIZE, 0);
					}
					else if (1 == id) {
						retval = send(g_vecsocket[id - 1], buf, BUFSIZE, 0);
					}
					if (retval == SOCKET_ERROR) {
						cout << "failed : send - 가변 - SC_PACKET_OTHER_PLAYERINFO" << endl;
						break;
					}
					else {
#ifdef DEBUG
						cout << id << "번째 클라이언트가 씬이 바뀌었으므로 " << "다른 클라이언트에게 가변 길이 패킷을 전송합니다!" << endl;
#endif
					}

				}
			}
			LeaveCriticalSection(&cs);
		}
		break;

		case CS_PACKET_GRRENMUSH:
		{
			EnterCriticalSection(&cs);
			// -------------------Process---------------------
			int id = packetinfo.id;
			MONSTERINFO tempmonsterinfo = {};
			{
				ZeroMemory(buf, sizeof(buf));
				retval = recvn(client_sock, buf, BUFSIZE, 0);
				if (retval == SOCKET_ERROR) {
					cout << "실패 - recvn - CS_PACKET_GRRENMUSH" << endl;
					break;
				}
				else
					memcpy(&tempmonsterinfo, buf, sizeof(tempmonsterinfo));
#ifdef DEBUG
				cout << id << "번째 버섯 클라에서 받아옴" << endl;
#endif
				{
					MONSTERINFO monsterinfo{};
					monsterinfo.id = id;
					monsterinfo.hp = g_monster_packet.green[id].hp;
					monsterinfo.money = g_monster_packet.green[id].money;
					monsterinfo.pt.x = g_monster_packet.green[id].pt.x;
					monsterinfo.pt.y = g_monster_packet.green[id].pt.y;
					monsterinfo.dir = g_monster_packet.green[id].dir;
					monsterinfo.state = g_monster_packet.green[id].state;


					ZeroMemory(&packetinfo, sizeof(packetinfo));
					packetinfo.id = id;
					packetinfo.size = sizeof(MONSTERINFO);
					packetinfo.type = SC_PACKET_GRRENMUSH;

					ZeroMemory(buf, sizeof(buf));
					memcpy(buf, &packetinfo, sizeof(packetinfo));
					retval = send(client_sock, buf, BUFSIZE, 0);
					if (retval == SOCKET_ERROR) {
						err_display("send() - SC_PACKET_GRRENMUSH_INITIALLY");
						break;
					}
					retval = send(client_sock, (char*)&g_monster_packet, sizeof(MONSTERPACKET), 0);

					if (retval == SOCKET_ERROR) {
						err_display("send()");
						break;
					}
				}
			}

			LeaveCriticalSection(&cs);
		}
		break;

		case CS_PACKET_SKILL_CREATE:
		{
			EnterCriticalSection(&cs);
			// 클라이언트로부터 스킬이 생성되었다는 메시지를 받았다.
			// 1. 새로 생성된 스킬의 skillinfo를 recv 해 온다.
			// 2. g_vecskill에 skillinfo를 push 한다.
			// 3. 새로 생성된 스킬에게 부여할 id를 정한다.
			// 4. 다른 클라이언트에게도 스킬 생성 메시지를 send 한다.

			// ---------------- Process -------------------
			// 0. 필요한 변수들
			SKILLINFO skillinfo;
			int skillid;
			// 1. 새로 생성된 스킬의 skillinfo를 recv 해 온다.
			{
				ZeroMemory(buf, sizeof(buf));
				retval = recvn(client_sock, buf, BUFSIZE, 0);
				if (retval == SOCKET_ERROR) {
					err_display("recvn() - CS_PACKET_SKILL_CREATE");
					break;
				}
				memcpy(&skillinfo, buf, sizeof(skillinfo));
			}
			// 2. g_vecskill에 skillinfo를 push 한다.
			{
				g_vecskill.push_back(skillinfo);
			}
			// 3. 새로 생성된 스킬에게 부여할 id를 정한다.
			{
				skillid = g_vecskill.size() - 1;
			}
			// 4. 다른 클라이언트에게도 스킬 생성 메시지를 send 한다.
			if (g_vecplayer.size() >= 2) {
				// 받을 다른 클라이언트의 id는?
				int recvid{ -1 };
				if (packetinfo.id == 0) recvid = 1;
				else		recvid = 0;

				// 고정 길이.
				ZeroMemory(&packetinfo, sizeof(PACKETINFO));
				packetinfo.id = /*skillid*/packetinfo.id;
				packetinfo.size = sizeof(packetinfo);
				packetinfo.type = SC_PACKET_SKILL_CREATE;
				ZeroMemory(buf, sizeof(buf));
				memcpy(buf, &packetinfo, sizeof(packetinfo));

				retval = send(g_vecsocket[recvid], buf, BUFSIZE, 0);
				if (retval == SOCKET_ERROR) {
					err_display("send() - SC_PACKET_SKILL_CREATE");
					break;
				}

				// 가변 길이.
				ZeroMemory(buf, sizeof(buf));
				memcpy(buf, &skillinfo, sizeof(skillinfo));
				retval = send(g_vecsocket[recvid], buf, BUFSIZE, 0);
				if (retval == SOCKET_ERROR) {
					err_display("send() - SC_PACKET_SKILL_CREATE");
					break;
				}
			}

			LeaveCriticalSection(&cs);
		}
		break;

		case SC_PACKET_CLIENT_END:
		{
			isEnd = true;

			closesocket(client_sock);
			cout << "[클라이언트 정상 종료] IP 주소 (" << inet_ntoa(clientaddr.sin_addr) <<
				"), 포트 번호 (" << ntohs(clientaddr.sin_port) << ")" << endl;
			return 0;
		}
		break;
		}
	}

	closesocket(client_sock);
	cout << "[클라이언트 강제 종료] IP 주소 (" << inet_ntoa(clientaddr.sin_addr) <<
		"), 포트 번호 (" << ntohs(clientaddr.sin_port) << ")" << endl;

	return 0;
}

DWORD WINAPI CalculateThread(LPVOID arg)
{
	Monster_Old_Movetime = GetTickCount();
	Monster_Cur_Movetime;
	while (1)
	{
		Monster_Cur_Movetime = GetTickCount();
		if (Monster_Cur_Movetime - Monster_Old_Movetime >= 30)
		{
			GreenMushRoom_MoveInPattern();
			Monster_Old_Movetime = GetTickCount();
		}
	}
	return 0;
}
DWORD test_old_time = GetTickCount();

DWORD WINAPI MonsterThread(LPVOID)
{
	PACKETINFO packetinfo;
	int retval{ 0 };
	char buf[BUFSIZE];
	DWORD test_cur_time;

	while (1)
	{
		if (isEnd)	// 클라이언트 소켓이 종료되면 몬스터 소켓도 종료
		{
			cout << "[클라이언트 정상 종료: 몬스터 스레드 정상 종료]" << endl;
			break;
		}

		test_cur_time = GetTickCount();
		if (test_cur_time - test_old_time > 100)
		{
			EnterCriticalSection(&cs);
			if (g_vecplayer.size() >= 1 && g_vecsocket.size() >= 1)
			{
				//cout << "g_Vec사이즈:" << g_vecsocket.size() << endl;

				ZeroMemory(&packetinfo, sizeof(packetinfo));
				packetinfo.id = 0;
				packetinfo.size = sizeof(MONSTERINFO);
				packetinfo.type = SC_PACKET_GRRENMUSH;

				//고정 데이터 보내기 
				ZeroMemory(buf, sizeof(buf));
				memcpy(buf, &packetinfo, sizeof(packetinfo));
				retval = send(g_vecsocket[0], buf, BUFSIZE, 0);
				if (retval == SOCKET_ERROR) {
					err_display("send() - SC_PACKET_GRRENMUSH");
					break;
				}

				// 가변데이터 보내기 					
				{
					retval = send(g_vecsocket[0], (char*)&g_monster_packet, sizeof(MONSTERPACKET), 0);

				}
				if (g_vecplayer.size() >= 2 && g_vecsocket.size() >= 2)
				{
					retval = send(g_vecsocket[1], buf, BUFSIZE, 0);
					retval = send(g_vecsocket[1], (char*)&g_monster_packet, sizeof(MONSTERPACKET), 0);
				}
				if (retval == SOCKET_ERROR) {
					err_display("send()");
					break;
				}
			}
			LeaveCriticalSection(&cs);
			test_old_time = GetTickCount();
		}
	}

	closesocket(g_vecsocket[0]);
	if (g_vecplayer.size() >= 2 && g_vecsocket.size() >= 2)
		closesocket(g_vecsocket[1]);

	if (!isEnd)
		cout << "[클라이언트 강제 종료: 몬스터 스레드 강제 종료]" << endl;

	return 0;
}

void InitializeMonsterInfo()
{
	int greenposX[] = { HENESISCX * 0.5f,HENESISCX * 0.7f , HENESISCX * 0.6f ,HENESISCX * 0.7f,HENESISCX * 0.5f, HENESISCX * 0.4f,
		HENESISCX * 0.53f,HENESISCX * 0.68f , HENESISCX * 0.6f ,HENESISCX * 0.72f,HENESISCX * 0.55f, HENESISCX * 0.41f };
	int greenposY[] = { HENESISCY - 460.f,HENESISCY - 460.f, HENESISCY - 460.f, HENESISCY - 460.f, HENESISCY - 460.f, HENESISCY - 460.f, 
		HENESISCY - 220.f, HENESISCY - 220.f, HENESISCY - 220.f, HENESISCY - 220.f, HENESISCY - 220.f, HENESISCY - 220.f, };
	OBJECT_DIR greenDir[] = { DIR_LEFT, DIR_RIGHT ,DIR_RIGHT, DIR_LEFT, DIR_RIGHT, DIR_RIGHT,
		DIR_RIGHT, DIR_LEFT ,DIR_RIGHT, DIR_LEFT, DIR_RIGHT, DIR_LEFT };
	int greenPtr[] = { 1,1,3,1,1,3,1,1,3,1,1,3 };

	srand(time(NULL));

	// 초록버섯 초기화
	for (int i = 0; i < MAX_GREEN; ++i)
	{
		MONSTERINFO monsterinfo{};
		monsterinfo.id = i;
		monsterinfo.hp = 100;
		monsterinfo.money = rand() % 1000 + 1;	// 랜덤한 돈 부여
		monsterinfo.pt.x = greenposX[i];
		monsterinfo.pt.y = greenposY[i];
		monsterinfo.dir = greenDir[i];
		monsterinfo.state = MONSTER_WALK;

		g_monster_packet.green[i] = monsterinfo;
	}
}

void GreenMushRoom_MoveInPattern()
{
	float m_fSpeed = 0.3f;

	for (int i = 0; i < MAX_GREEN; ++i)
	{

		if (DIR_LEFT == g_monster_packet.green[i].dir)
		{
			switch (g_monster_packet.green[i].state)
			{
			case MONSTER_WALK:
			{
				g_monster_packet.green[i].pt.x -= m_fSpeed;

				if (g_monster_packet.green[i].pt.x < 465.f)
					g_monster_packet.green[i].dir = DIR_RIGHT;
			}
			break;
			case MONSTER_STAND:
				break;
			}

		}
		else
		{
			switch (g_monster_packet.green[i].state)
			{
			case MONSTER_WALK:
			{
				g_monster_packet.green[i].pt.x += m_fSpeed;
				if (g_monster_packet.green[i].pt.x > 1390.f)
					g_monster_packet.green[i].dir = DIR_LEFT;
			}
			break;
			case MONSTER_STAND:
				break;
			}
		}
	}
}

int main()
{
	// 임계 영역 초기화
	InitializeCriticalSection(&cs);

	// 윈속 초기화
	InitializeNetwork();

	// 공간 2개 예약.
	g_vecplayer.reserve(MAX_USER);
	// 몬스터 좌표 초기화
	InitializeMonsterInfo();

	int retval;

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET)
		err_quit("socket()");

	// 주소 구조체 생성
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);

	// bind()
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
		err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
		err_quit("listen()");

	// 데이터 통신에 사용할 변수
	HANDLE hThread[3];
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;

	int test_num{};
	while (true) {
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		// 접속한 클라이언트 socket을 배열에 담는다. (1201)
		g_vecsocket.push_back(client_sock);
		cout << "연결!:" << g_vecsocket.size() << endl;
		// 접속한 클라이언트 정보 출력
		cout << "[클라이언트 접속] IP 주소 (" << inet_ntoa(clientaddr.sin_addr) <<
			"), 포트 번호 (" << ntohs(clientaddr.sin_port) << ")" << endl;

		// 클라이언트 스레드 생성
		hThread[0] = CreateThread(NULL, 0, ClientThread, (LPVOID)client_sock, 0, NULL);
		if (hThread[0] == NULL)
			closesocket(client_sock);
		else
			CloseHandle(hThread[0]);

		//계산 스레드 생성
		if (g_vecsocket.size() < 2)
		{
			cout << "스레드 생성 test" << endl;
			hThread[1] = CreateThread(NULL, 0, CalculateThread, NULL, 0, NULL);
			if (hThread[1] == NULL)
				closesocket(client_sock);
			else
				CloseHandle(hThread[1]);

			// //몬스터 스레드 생성
			hThread[2] = CreateThread(NULL, 0, MonsterThread, NULL, 0, NULL);
			if (hThread[2] == NULL)
				closesocket(client_sock);
			else
				CloseHandle(hThread[2]);
		}

	}
	// 두 개의 스레드 종료 대기
	WaitForMultipleObjects(1, hThread, TRUE, INFINITE);

	// 임계 영역 삭제
	DeleteCriticalSection(&cs);

	// closesocket()
	closesocket(listen_sock);

	EndNetwork();

	return 0;
}

void InitializeNetwork()
{
	// 윈속 초기화.
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		err_quit("WSAStartup()");
}

void EndNetwork()
{
	// 윈속 종료
	WSACleanup();
}

int recvn(SOCKET s, char *buf, int len, int flags)
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