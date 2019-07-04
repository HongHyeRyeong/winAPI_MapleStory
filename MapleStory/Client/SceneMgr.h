#pragma once

class CScene;
class CSceneMgr
{
	DECLARE_SINGLETON(CSceneMgr)

private:
	CSceneMgr(void);
	~CSceneMgr(void);

public:
	SCENE_TYPE GetSceneType() { return m_eSceneType; }

public:
	void SetScene(SCENE_TYPE eType);
	void SetChangeScene(SCENE_TYPE eType);
	CScene* GetScene() { return m_pScene; }

public:
	void Update();
	void Render(HDC hDc);
	void Release();

private:
	CScene*		m_pScene;
	SCENE_TYPE  m_eSceneType;

	bool		m_bChangeCheck;
};
