#pragma once

// 서버, 클라이언트 공통.
// 프로토콜 정의.
#define SERVERPORT 9000
#define BUFSIZE			4000
#define MAX_USER		2
#define MAX_GREEN		12

// #define DEBUG
// #define DEBUG_SCROLL
#define DEBUG_1206

// --------------------------------------------------------------------------
// 클라이언트 컨텐츠 적인 변수들.

// 필요한 변수 정의.
#define PI	3.1415f
#define GRAVITY 9.8f


// 윈도우 X, Y 사이즈.
#define WINCX 800
#define WINCY 600

// 보스 맵 X,Y 사이즈.
#define BOSSMAPCX 1799
#define BOSSMAPCY 781

// 타일 가로, 세로 개수
#define TILEX 300
#define TILEY 50

// 타일 가로, 세로 사이즈
#define TILECX 30
#define TILECY 30

// 맵 스프라이트 크기 정의.
#define FIELDCX 1920
#define FIELDCY 681

#define HENESISCX 1773
#define HENESISCY 1464

#define BOSSMAPCX 1799
#define BOSSMAPCY 781


// 이펙트(스킬) 거리 정의. (플레이어와의)
#define SWINGEFFECTX 200.f
#define DRAGONX 50.f
#define FIREX 250.f
#define FIREY 50.f

// 포션 HP, MP 정의
#define HPPORTION 500
#define MPPORTION 500



// 싱글톤 정의.
// 클래스를 heap에 할당해 놓고, 할당해 놓은 주소를
// 바깥에서 쓸 수 있게 하려고! (static)
#define DECLARE_SINGLETON(ClassName)	\
private:								\
	static ClassName* m_pInstance;		\
public:									\
	static ClassName* GetInstance()		\
	{									\
	if(NULL == m_pInstance)			\
	m_pInstance = new ClassName;\
	return m_pInstance;			\
	}									\
	void DestroyInstance()				\
	{									\
	if(m_pInstance)					\
		{								\
		delete m_pInstance;			\
		m_pInstance = NULL;			\
		}								\
	}

#define IMPLEMENT_SINGLETON(ClassName)			\
	ClassName* ClassName::m_pInstance = NULL;