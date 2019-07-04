#pragma once

#include "Include.h"

#define KEYMGR CKeyMgr::GetInstance()

class CKeyMgr
{
	DECLARE_SINGLETON(CKeyMgr)

public:
	bool	m_bKeyDown[256];	// 키가 눌렸는지 체크할 배열
	bool	m_bKeyUp[256];	// 키가 떼졌는지 체크할 배열

private:
	CKeyMgr(void);
	~CKeyMgr(void);

public:
	bool StayKeyDown(INT nKey); // 키가 눌리고 있는지 체크
	bool OnceKeyDown(INT nKey); // 키가 한번 눌렸는지 체크
	bool OnceKeyUp(INT nKey);	// 키가 한번 눌렸다 떼졌는지 체크
	bool IsToggleKey(INT nKey);	// 한번 눌릴때마다 on off로 바꾸도록
};