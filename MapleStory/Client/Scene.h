#pragma once

class CScene
{
public:
	CScene(void);
	virtual ~CScene(void);

public:
	virtual void Initialize() = 0;
	virtual int Update() = 0;
	virtual void Render(HDC hDc) = 0;
	virtual void Release() = 0;
};
