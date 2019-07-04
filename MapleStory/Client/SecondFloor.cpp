#include "StdAfx.h"
#include "SecondFloor.h"

CSecondFloor::CSecondFloor(void)
{
}

CSecondFloor::~CSecondFloor(void)
{
}

void CSecondFloor::Initialize(void)
{
// 	if(SCENE_FIELD == g_eScene)
// 	{
// 		m_tInfo.size.cx = 1920.f;
// 		m_tInfo.size.cy = 300.f;
// 		m_tInfo.pt.x = 1920.f * 0.5f;
// 		m_tInfo.pt.y = 250.f;
// 	}
}

int CSecondFloor::Update(void)
{
// 	if(SCENE_FIELD == g_eScene)
// 	{
// 		m_tInfo.size.cx = 1920.f;
// 		m_tInfo.size.cy = 300.f;
// 		m_tInfo.pt.x = 1920.f * 0.5f;
// 		m_tInfo.pt.y = 250.f;
// 	}
	UpdateRect();
	return 0;
}

void CSecondFloor::Render(HDC hDc)
{
	if(GetAsyncKeyState('1'))
		Rectangle(hDc, 
		static_cast<int>(m_tRect.left + g_fScrollX), 
		static_cast<int>(m_tRect.top  + g_fScrollY), 
		static_cast<int>(m_tRect.right + g_fScrollX), 
		static_cast<int>(m_tRect.bottom + g_fScrollY));
}

void CSecondFloor::Release(void)
{

}

void CSecondFloor::FrameMove()
{

}
