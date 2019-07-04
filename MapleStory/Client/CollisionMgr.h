#pragma once

class CFont;
class CollisionMgr
{
public:
	CollisionMgr(void);
	~CollisionMgr(void);

public:
	static void CollisionRect(OBJLIST& DstList, OBJLIST& SrcList, COLLISION_ID eID = COL_END);

	static int CalculatingCipher(int iNumber);
	static int ReturningRandomNumber(int iNumber);
	static FONT_TYPE ReturningFontType(int iCipher);
	static unsigned long WELLRNG512(void);
	static void InitWELLRNG512(unsigned long seed);

public:
	template <typename T>
	static CObj*	CreateFont(CObj* pTarget, int iAtt,
		FONT_ID eID, FONT_TYPE eType, FONT_FLOOR eFloor)
	{
		CObj* pFont = new T;
		(dynamic_cast<CFont*>(pFont))->SetObj(pTarget);
		(dynamic_cast<CFont*>(pFont))->SetNumber(iAtt);
		(dynamic_cast<CFont*>(pFont))->SetID(eID);
		(dynamic_cast<CFont*>(pFont))->SetType(eType);
		(dynamic_cast<CFont*>(pFont))->SetFloor(eFloor);
		(dynamic_cast<CObj*>(pFont))->Initialize();
		return pFont;
	}

private:
	static bool m_bIsFontCreated;
};
