#pragma once

class CMyBmp;
class CBitmapMgr
{
	DECLARE_SINGLETON(CBitmapMgr)
private:
	CBitmapMgr(void);
	~CBitmapMgr(void);

public:
	map<const TCHAR*, CMyBmp*>& GetMapBit()
	{
		return m_MapBit;
	}

	CMyBmp* FindImage(const TCHAR* pFindKey);

	void LoadImageByScene(SCENE_TYPE eType);
	void Release();

private:
	map<const TCHAR*, CMyBmp*>	m_MapBit;
};
