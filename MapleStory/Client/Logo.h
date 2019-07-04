#pragma once
#include "scene.h"

class CLogo :
	public CScene
{
public:
	CLogo(void);
	virtual ~CLogo(void);

public:
	virtual void Initialize();
	virtual int Update();
	virtual void Render(HDC hDc);
	virtual void Release();

private:
	// 서버 추가.
	HDC		m_hDC;

	FRAME	m_tFrame;
	const TCHAR*	m_pImgName;
	RENDER_TYPE m_eRenderType;
	DWORD			m_dwFrameOldTime;
	DWORD			m_dwFrameCurTime;

	DWORD			m_dwLogo2OldTime;
	DWORD			m_dwLogo2CurTime;


	int		m_ImgCX;
	int		m_ImgCY;


	// 버튼
	RECT		m_tLoginRect;
	RECT		m_tExitRect;
	INFO		m_tLoginInfo;
	INFO		m_tExitInfo;

	bool		MouseInLogin(void);
	bool		m_bIsMouseInLogin;
	bool		MouseInExit(void);
	bool		m_bIsMouseInExit;
};
