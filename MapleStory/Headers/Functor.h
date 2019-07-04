#pragma once

class CStringCmp
{
public:
	CStringCmp(const TCHAR* pKey)
		: m_pKey(pKey) {}

public:
	template <typename T>
	bool operator()(T& dst)
	{
		return !_tcscmp(dst.first, m_pKey);
	}

private:
	const TCHAR*	m_pKey;
};