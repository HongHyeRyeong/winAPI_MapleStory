#pragma once
#include "scene.h"

class CPlayer;
class CField :
	public CScene
{
public:
	CField(void);
	virtual ~CField(void);

public:
	virtual void Initialize();
	virtual int Update();
	virtual void Render(HDC hDc);
	virtual void Release();

public:
	void SetOtherPlayer(CPlayer* pOtherPlayer) { m_pOtherPlayer = pOtherPlayer; }
	CPlayer* GetPlayer() { return m_pPlayer; }
	CPlayer* GetOtherPlayer() {  return m_pOtherPlayer; }
private:
	CPlayer* m_pPlayer = nullptr;
	CPlayer* m_pOtherPlayer = nullptr;
};
 