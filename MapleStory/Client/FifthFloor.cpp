#include "StdAfx.h"
#include "FifthFloor.h"

CFifthFloor::CFifthFloor(void)
{
}

CFifthFloor::~CFifthFloor(void)
{
}

void CFifthFloor::Initialize(void)
{

}

int CFifthFloor::Update(void)
{
	UpdateRect();
	return 0;
}

void CFifthFloor::Render(HDC hDc)
{
	if(GetAsyncKeyState('1'))
		Rectangle(hDc, 
		static_cast<int>(m_tRect.left + g_fScrollX), 
		static_cast<int>(m_tRect.top  + g_fScrollY), 
		static_cast<int>(m_tRect.right + g_fScrollX), 
		static_cast<int>(m_tRect.bottom + g_fScrollY));
}

void CFifthFloor::Release(void)
{

}

void CFifthFloor::FrameMove()
{

}
