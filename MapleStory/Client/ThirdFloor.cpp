#include "StdAfx.h"
#include "ThirdFloor.h"

CThirdFloor::CThirdFloor(void)
{
}

CThirdFloor::~CThirdFloor(void)
{
}

void CThirdFloor::Initialize(void)
{

}

int CThirdFloor::Update(void)
{
	UpdateRect();
	return 0;
}

void CThirdFloor::Render(HDC hDc)
{
	if(GetAsyncKeyState('1'))
		Rectangle(hDc, 
		static_cast<int>(m_tRect.left + g_fScrollX), 
		static_cast<int>(m_tRect.top  + g_fScrollY), 
		static_cast<int>(m_tRect.right + g_fScrollX), 
		static_cast<int>(m_tRect.bottom + g_fScrollY));

}

void CThirdFloor::Release(void)
{

}

void CThirdFloor::FrameMove()
{

}
