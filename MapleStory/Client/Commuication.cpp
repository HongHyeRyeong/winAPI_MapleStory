#include "StdAfx.h"
#include "Commuication.h"

CCommuication::CCommuication(void)
{
}

CCommuication::~CCommuication(void)
{
}

void CCommuication::SetNextButtonRect()
{
	m_tNextButton.left = static_cast<LONG>(m_tInfo.pt.x + 194.f);
	m_tNextButton.right = static_cast<LONG>(m_tInfo.pt.x + 233.f);
	m_tNextButton.bottom = static_cast<LONG>(m_tInfo.pt.y + 57.f);
	m_tNextButton.top = static_cast<LONG>(m_tInfo.pt.y + 43.f);
}
