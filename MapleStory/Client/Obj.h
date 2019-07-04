#pragma once

class CObj
{
public:
	CObj(void);
	virtual ~CObj(void);

public:
	virtual void Initialize(void) = 0;
	virtual int Update(void) = 0;
	virtual void Render(HDC hDc) = 0;
	virtual void Release(void) = 0;
	virtual void FrameMove() = 0;

public:
	const INFO& GetInfo()	 { return m_tInfo; }
	const RECT& GetRect()	 { return m_tRect; }
	const RECT& GetCollRect() { return m_tCollRect; }
	const bool& GetDeadState()  { return m_bIsDead; }

	RENDER_TYPE GetRenderType() { return m_eRenderType; }

	OBJECT_DIR  GetDir() { return m_eDir; }

public:
	void SetPos(float fX, float fY) { m_tInfo.pt.x = fX, m_tInfo.pt.y = fY; }
	void SetSize(float cx, float cy) { m_tInfo.size.cx = cx, m_tInfo.size.cy = cy; }
	void SetAngle(float fAngle) { m_fAngle = fAngle; }
	void SetTarget(CObj* pTarget) { m_pTarget = pTarget; }
	void SetInfo(INFO& rInfo) { m_tInfo = rInfo; }
	void SetDir(OBJECT_DIR eDir) { m_eDir = eDir; }
	void SetDeadState(bool bIsDead) { m_bIsDead = bIsDead; }
	void SetSpeed(float fSpeed) { m_fSpeed = fSpeed; }

	void SetHp(int iHp) { m_tState.iHp = iHp; }
	void SetAtt(int iAtt) { m_tState.iAtt = iAtt; }

	void SetCollMode(bool bCollMode) { m_bCollMode = bCollMode; }
	bool GetCollMode() { return m_bCollMode; }

public:
	const STATE& GetState()	 { return m_tState; }
	const STATE* GetStatePointer()	 { return &m_tState; }
	void		 SetState(STATE& rState) { m_tState = rState; };
	void		 SetDamage(int iAtt) { m_tState.iHp -= iAtt; }

public:
	void IsDead(void) { m_bIsDead = true; }

public:
	void UpdateRect();

protected:
	INFO			m_tInfo;
	RECT			m_tRect;
	RECT			m_tCollRect;
	// ป๓ลย
	STATE			m_tState;

	FRAME		m_tFrame;

	float		m_fSpeed;
	float		m_fAngle;
	float		m_fDist;

	bool		m_bIsDead;

	CObj*		m_pTarget;

	const TCHAR*		m_pImgName;

	RENDER_TYPE	m_eRenderType;

	DWORD			m_dwFrameOldTime;
	DWORD			m_dwFrameCurTime;

	OBJECT_DIR		m_eDir;

	bool		m_bCollMode;
};
