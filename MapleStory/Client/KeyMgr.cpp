#include "StdAfx.h"
#include "KeyMgr.h"

IMPLEMENT_SINGLETON(CKeyMgr)

CKeyMgr::CKeyMgr(void)
{
	// 키 버퍼 초기화.
	ZeroMemory(m_bKeyDown, 256);
	ZeroMemory(m_bKeyUp, 256);
}

CKeyMgr::~CKeyMgr(void)
{
}

// 계속해서 눌리는지 체크
bool CKeyMgr::StayKeyDown( INT nKey )
{
	if(GetAsyncKeyState(nKey) & 0x8000)
		return true;

	return false;
}

// 한번만 눌리는지 체크
bool CKeyMgr::OnceKeyDown( INT nKey )
{
	if(GetAsyncKeyState(nKey) & 0x8000)
	{
		// 키가 눌리는 순간 true
		if(m_bKeyDown[nKey] == false)
		{
			m_bKeyDown[nKey] = true;
			return true;
		}
	}
	else
	{
		// 키를 떼는 순간 false
		m_bKeyDown[nKey] = false;
	}
	return false;
}

// 한번만 눌렀다가 떼는지 체크
bool CKeyMgr::OnceKeyUp( INT nKey )
{
	if(GetAsyncKeyState(nKey) & 0x8000)
		m_bKeyUp[nKey] = true;
	else
	{
		// 눌려진 순간 true가 되므로 true일때만 진입한다.
		if(m_bKeyUp[nKey] == true)
		{
			// 키 상태를 false로 만들어 재진입을 막는다.
			m_bKeyUp[nKey] = false;

			return true;
		}
	}

	return false;
}

bool CKeyMgr::IsToggleKey( INT nKey )
{
	//GetKeyState의 0x0001은 이전에 눌렸는지를 체크한다.
	if(GetKeyState(nKey) & 0x0001)
		return true;

	return false;
}
