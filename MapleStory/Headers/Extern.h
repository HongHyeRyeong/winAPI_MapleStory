#pragma once

extern HWND g_hWnd;
extern float g_fScrollX;
extern float g_fScrollY;
extern SCENE_TYPE g_eScene;
extern bool g_bIsSceneChange;

extern bool g_bIsSend;
extern bool g_bIsProgramEnd;

extern int g_iTakedMob1;
extern int g_iTakedMob2;
extern int g_iTakedMob1Max;
extern int g_iTakedMob2Max;

extern int g_iLevel;
extern int g_iExp;

// 서버 추가
extern SOCKET g_sock;
extern wchar_t g_ipbuf[50]; // 로고에서 입력하는 ip 주소 담을 버퍼.
extern wchar_t g_nicknamebuf[50]; // 캐릭터 생성 씬에서 입력하는 ip 주소 담을 버퍼.
extern int g_retval;

extern PLAYERINFO g_myinfo;
extern int g_myid;
extern vector<PLAYERINFO> g_vecplayer;

// monster 생성 단계 설정
extern vector<MONSTERINFO> g_vecgreen;