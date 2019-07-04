#pragma once

class CLine
{
public:
	CLine(void);
	CLine(MYPOINT tLPoint, MYPOINT tRPoint);
	~CLine(void);

public:
	const LINEINFO& GetInfo() { return m_tInfo; }

public:
	void Render(HDC hDC);

private:
	LINEINFO	m_tInfo;
};
