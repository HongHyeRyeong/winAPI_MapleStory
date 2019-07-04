#pragma once
#include "obj.h"
#include "PlayerHP.h"
#include "PlayerExp.h"
#include "PlayerMP.h"
#include "Arrow.h"

class CPlayer :
	public CObj
{
public:
	CPlayer(void);
	virtual ~CPlayer(void);

public:
	virtual void	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(HDC hDc);
	virtual void	Release(void);
	virtual void	FrameMove();

public:
	void	Jump();
	void	Scroll();
	void	KeyCheck();
	void	LineCollision();
	
	void	SendMovePacket();
	void SendSceneChangePacket();
	void SendSkillCreatePacket(SKILL_TYPE eType);

	void UpdateINFOinPLAYERINFO();
	void UpdateImageInJob(OBJECT_DIR dir);
	int WhatIsID();
	int WhatIsOtherID();
	void LevelUp();
	void PreventOut();
	void InChangingScene();
	void InInvincible();

	void FrameMove_otherclient();


	// ---------------------
	// 서버 추가
	// static DWORD WINAPI SendThread(LPVOID, int, bool&, INFO&, PLAYER_STATE&)

	// 밧줄태우기
public:
	void    SetIsRopeColl(bool bRopeColl) { m_bIsRopeColl = bRopeColl; }

	// 층 태우기
	void	SetPlayerFloor(int iPlayerFloor) { m_iPlayerFloor = iPlayerFloor; }
	int		GetPlayerFloor() { return m_iPlayerFloor; }
	bool	GetIsJumpUp() { return m_bIsJumpUp; }

	// 몬스터랑 충돌할 때 뒤로 포물선 그리면서 밀리기
	void	SetIsJump(bool bIsJump) { m_bIsJump = bIsJump; }
	void	SetMinusAngle(float fAngle) { m_fAngle -= fAngle; }
	void    SetPlusAngle(float fAngle) { m_fAngle += fAngle; }

	void	SetIsLineColl(bool bLineColl) { m_bLineColl = bLineColl;}
	float*  GetLinepY() { return &m_fLineY; }

public:
	// set, get (주로 상태)
	PLAYER_STATE GetPlayerState() { return m_eCurState; }
	void		 SetPlayerState(PLAYER_STATE eState) { m_eCurState = eState; }
	void		 SetIsLeveling(bool bLeveling) { m_bIsLeveling = bLeveling; }

	void		SetDamaged(int iDamage) { m_tState.iHp -= iDamage; }
	void		SetGold(int iGold) {
		m_tState.iGold += iGold;

		g_bIsSend = true;
		g_vecplayer[g_myid].money += iGold;
	}

	void		SetHp(int iHp) 
	{
		// 맥스HP - HPPORTION 보다 많이 차있으면
		if(m_tState.iHp > m_tState.iMaxHp - HPPORTION)	
			m_tState.iHp += (m_tState.iMaxHp - m_tState.iHp); 
		else
			m_tState.iHp += HPPORTION;
	}
	void		SetMp(int iMp) 
	{
		// 맥스HP - HPPORTION 보다 많이 차있으면
		if(m_tState.iMp > m_tState.iMaxMp - MPPORTION)	
			m_tState.iMp += (m_tState.iMaxMp - m_tState.iMp); 
		else
			m_tState.iMp += MPPORTION;
	}

	// (서버) 조종 불가능한, 다른 클라이언트를 생성할 때 사용함.
	void		SetPlayerInfo(PLAYERINFO playerinfo) {
		m_playerinfo = playerinfo; 
		m_IsMaster = false;
	}
	void SetInfoPt(MYPOINT pt) { m_tInfo.pt = pt; }

	// 무적 상태
	bool		 GetIsInvincible() { return m_bIsInvincible; }
	void		 SetIsInvincible(bool bIsInvincible) { m_bIsInvincible = bIsInvincible; }
	
public:
	// 충돌박스 업데이트
	void	UpdateCollRect();

	// 화살
private:
	template <typename T> 
	CObj*	CreateArrow(float fX = 0.f, float fY = 0.f, ARROW_ID eID = ARROW_BASIC, int iOrder = 0)
	{
		int id = WhatIsID();
		CObj* pArrow = CAbstractFactory<T>::CreateObj(
			g_vecplayer[id].pt.x/*m_tInfo.pt.x*/ + fX, g_vecplayer[id].pt.y/*m_tInfo.pt.y*/ + fY);
		pArrow->SetAtt(this->GetState().iAtt);
		pArrow->SetDir(m_eDir);
		dynamic_cast<CArrow*>(pArrow)->SetID(eID);
		dynamic_cast<CArrow*>(pArrow)->SetOrder(iOrder);
		return pArrow;
	}

	// 샷, 스윙 (플레이어에서 나오는 스킬)
private:
	template <typename T>
	CObj*	CreateSkill()
	{
		CObj* pSkill = CAbstractFactory<T>::CreateObj(
			m_tInfo.pt.x, m_tInfo.pt.y);
		pSkill->SetDir(m_eDir);
		pSkill->SetAtt(this->GetState().iAtt);
		dynamic_cast<CSkill*>(pSkill)->SetPlayer(this);
		return pSkill;
	}

	// 드래곤
private:
	template <typename T>
	CObj*	CreateDragon()
	{
		if(DIR_LEFT == m_eDir)
		{
			CObj* pSkill = CAbstractFactory<T>::CreateObj(
				m_tInfo.pt.x - DRAGONX, m_tInfo.pt.y);
			pSkill->SetAtt(this->GetState().iAtt);
			pSkill->SetDir(m_eDir);
			return pSkill;
		}
		else
		{
			CObj* pSkill = CAbstractFactory<T>::CreateObj(
				m_tInfo.pt.x + DRAGONX, m_tInfo.pt.y);
			pSkill->SetAtt(this->GetState().iAtt);
			pSkill->SetDir(m_eDir);
			return pSkill;
		}
	}

	// 파이어 스킬
private:
	template <typename T>
	CObj*	CreateFire()
	{
		if(DIR_LEFT == m_eDir)
		{
			CObj* pSkill = CAbstractFactory<T>::CreateObj(
				m_tInfo.pt.x - FIREX, m_tInfo.pt.y - FIREY);
//			pSkill->SetPos(m_tInfo.pt.x - FIREX, m_tInfo.pt.y - 350.f);
			pSkill->SetAtt(this->GetState().iAtt);
			pSkill->SetDir(m_eDir);
			return pSkill;
		}
		else
		{
			CObj* pSkill = CAbstractFactory<T>::CreateObj(
				m_tInfo.pt.x + FIREX, m_tInfo.pt.y- FIREY);
//			pSkill->SetPos(m_tInfo.pt.x + FIREX, m_tInfo.pt.y - 350.f);
			pSkill->SetAtt(this->GetState().iAtt);
			pSkill->SetDir(m_eDir);
			return pSkill;
		}
	}

	// 스윙 이펙트
private:
	template <typename T>
	CObj*	CreateEffect()
	{
		if(DIR_LEFT == m_eDir)
		{
			CObj* pEffect = CAbstractFactory<T>::CreateObj(
				m_tInfo.pt.x - SWINGEFFECTX, m_tInfo.pt.y);
			pEffect->SetAtt(this->GetState().iAtt);
			pEffect->SetDir(m_eDir);
			return pEffect;
		}
		else
		{
			CObj* pEffect = CAbstractFactory<T>::CreateObj(
				m_tInfo.pt.x + SWINGEFFECTX, m_tInfo.pt.y);
			pEffect->SetAtt(this->GetState().iAtt);
			pEffect->SetDir(m_eDir);
			return pEffect;
		}
	}

	// 화면 중앙에 나타나는 스킬
	template <typename T>
	CObj*	CreateWing()
	{
		CObj* pSkill = CAbstractFactory<T>::CreateObj(
			WINCX * 0.5f, WINCY * 0.5f);
		pSkill->SetDir(m_eDir);
		pSkill->SetAtt(this->GetState().iAtt);
		//			dynamic_cast<CSkill*>(pSkill)->SetPlayer(this);
		return pSkill;
	}


	// HP바
private:
	template <typename T>
	CObj*	CreateHpBar()
	{
		CObj* pHpBar = CAbstractFactory<T>::CreateObj();
		dynamic_cast<CPlayerHP*>(pHpBar)->SetPlayer(this);
		return pHpBar;
	}

	// EXP
private:
	template <typename T>
	CObj*	CreateExpBar()
	{
		CObj* pExpBar = CAbstractFactory<T>::CreateObj();
		dynamic_cast<CPlayerExp*>(pExpBar)->SetPlayer(this);
		return pExpBar;
	}

	// MP
private:
		template <typename T>
		CObj*	CreateMpBar()
		{
			CObj* pMpBar = CAbstractFactory<T>::CreateObj();
			dynamic_cast<CPlayerMP*>(pMpBar)->SetPlayer(this);
			return pMpBar;
		}

private:
	float			m_fJumpSpeed; // 점프 파워
	float			m_fJumpAcc;   // 점프 가속도 (시간)

	float			m_fSpeedY;

	bool			m_bIsJump;	  // 점프 상태인지 판별.
	bool			m_bIsPressed; // 점프 키가 눌렸는지 판별

	float			m_fOffSet;	  // 스크롤 적정 기준
	float			m_fOffSetY;		// 스크롤 Y
	float			m_fOffSetGapY; // 씬마다 오프셋 달리 주기

	PLAYER_STATE	m_eCurState;  // 플레이어 현재 상태
	PLAYER_STATE	m_ePreState;  // 플레이어 이전 상태

	bool			m_bIsLeveling;

	// 이미지
	HDC				m_hDC;
	HDC				m_hMemDC;

	HBITMAP			m_hOldBmp;
	HBITMAP			m_hBitmap;

	int				m_iBlending;

	DWORD			m_dwBlendOldTime;
	DWORD			m_dwBlendCurTime;

	// 충돌
	bool			m_bIsRopeColl;
	int				m_iPlayerFloor;

	bool			m_bIsJumpUp;
	bool			m_bLineColl;

	DWORD			m_dwDamageTime;

	bool			m_bIsInvincible;

	// 라인
	list<CLine*>	m_LineList;
	float			m_fLineY;

	// 스킬
	bool			m_bIsSkillPressed;

	// 레벨업 이펙트
	FRAME			m_tLevelUpFrame;
	DWORD			m_dwLevelUpCurTime;
	DWORD			m_dwLevelUpOldTime;

	// -------------------------------
	// 서버 추가.
	PLAYERINFO	m_playerinfo;
	bool					m_IsMaster = true; // 내 클라이언트의 플레이어인지, 다른 클라이언트의 플레이어인지 구분.
	bool					m_bIsAdmitSend_inFrame = false; // 1초당 30번만 제한하기 위해서 만듦.
};
