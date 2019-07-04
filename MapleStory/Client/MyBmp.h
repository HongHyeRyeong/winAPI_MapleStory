#pragma once

class CMyBmp
{
public:
	CMyBmp(void);
	~CMyBmp(void);

public:
	HDC GetMemDC() { return m_hMemDc; }

public:
	CMyBmp* LoadBmp(const TCHAR* pFileName);
	void	Release();

private:
	HDC		m_hDc;
	HDC		m_hMemDc;

	HBITMAP m_bitmap;
	HBITMAP	m_oldBmp;
};
