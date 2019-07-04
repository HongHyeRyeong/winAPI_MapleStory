#include "StdAfx.h"
#include "FourthFloor.h"

CFourthFloor::CFourthFloor(void)
{
}

CFourthFloor::~CFourthFloor(void)
{
}

void CFourthFloor::Initialize(void)
{

}

int CFourthFloor::Update(void)
{
	UpdateRect();
	return 0;
}

void CFourthFloor::Render(HDC hDc)
{
	if(GetAsyncKeyState('1'))
		Rectangle(hDc, 
		static_cast<int>(m_tRect.left + g_fScrollX), 
		static_cast<int>(m_tRect.top  + g_fScrollY), 
		static_cast<int>(m_tRect.right + g_fScrollX), 
		static_cast<int>(m_tRect.bottom + g_fScrollY));

}

void CFourthFloor::Release(void)
{

}

void CFourthFloor::FrameMove()
{

}
