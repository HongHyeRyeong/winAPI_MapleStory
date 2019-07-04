#include "StdAfx.h"
#include "Line.h"

CLine::CLine(void)
{
}

CLine::CLine(MYPOINT tLPoint, MYPOINT tRPoint)
: m_tInfo(tLPoint, tRPoint)
{

}

CLine::~CLine(void)
{
}

void CLine::Render(HDC hDC)
{
	MoveToEx(hDC, static_cast<int>(m_tInfo.tLPoint.x + g_fScrollX), static_cast<int>(m_tInfo.tLPoint.y + g_fScrollY), NULL);
	LineTo(hDC, static_cast<int>(m_tInfo.tRPoint.x + g_fScrollX), static_cast<int>(m_tInfo.tRPoint.y + g_fScrollY));
}
