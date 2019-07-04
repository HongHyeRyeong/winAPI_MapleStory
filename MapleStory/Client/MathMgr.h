#pragma once

class CMathMgr
{
public:
	CMathMgr(void);
	~CMathMgr(void);

public:
	static float CalcDistance( CObj* pDest,  CObj* pSour);
	static float CalcAngleToRadian( CObj* pDest,  CObj* pSour);
	static float CalcAngleToDegree( CObj* pDest,  CObj* pSour);
};
