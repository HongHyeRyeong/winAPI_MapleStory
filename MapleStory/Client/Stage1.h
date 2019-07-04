#pragma once
#include "scene.h"

class CStage1 :
	public CScene
{
public:
	CStage1(void);
	virtual ~CStage1(void);

public:
	virtual void Initialize();
	virtual int Update();
	virtual void Render(HDC hDc);
	virtual void Release();
public:
	void SetOtherPlayer(CPlayer* pOtherPlayer) { m_pOtherPlayer = pOtherPlayer; }
	void SetPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }
	CPlayer* GetPlayer() { return m_pPlayer; }
	CPlayer* GetOtherPlayer() { return m_pOtherPlayer; }
private:
	CPlayer* m_pPlayer = nullptr;
	CPlayer* m_pOtherPlayer = nullptr;
};
