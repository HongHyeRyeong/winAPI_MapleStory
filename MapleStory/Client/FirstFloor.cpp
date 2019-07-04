#include "StdAfx.h"
#include "FirstFloor.h"

CFirstFloor::CFirstFloor(void)
{
}

CFirstFloor::~CFirstFloor(void)
{
}

void CFirstFloor::Initialize(void)
{
// 	if(SCENE_FIELD == g_eScene)
// 	{
// 		m_tInfo.size.cx = 1920.f;
// 		m_tInfo.size.cy = 280.f;
// 		m_tInfo.pt.x = 1920.f * 0.5f;
// 		m_tInfo.pt.y = 500.f;
// 	}
}

int CFirstFloor::Update(void)
{
	UpdateRect(); 
	m_tCollRect = m_tRect;

// 	if(SCENE_FIELD == g_eScene)
// 	{
// 		m_tInfo.size.cx = 1920.f;
// 		m_tInfo.size.cy = 140.f;
// 		m_tInfo.pt.x = 1920.f * 0.5f;
// 		m_tInfo.pt.y = 500.f;
// 	}

	return 0;

}

void CFirstFloor::Render(HDC hDc)
{
	if(GetAsyncKeyState('1'))
		Rectangle(hDc, 
		static_cast<int>(m_tRect.left + g_fScrollX), 
		static_cast<int>(m_tRect.top  + g_fScrollY), 
		static_cast<int>(m_tRect.right + g_fScrollX), 
		static_cast<int>(m_tRect.bottom + g_fScrollY));
}

void CFirstFloor::Release(void)
{

}

void CFirstFloor::FrameMove()
{

}
