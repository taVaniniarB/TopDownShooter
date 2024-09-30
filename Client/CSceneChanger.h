#pragma once
#include "CObject.h"

class CSceneChanger :
	public CObject
{
private:
	SCENE_TYPE m_eScene;
	bool m_bActive;

public:
	void SetScene(SCENE_TYPE _eScene) { m_eScene = _eScene; }
	void SetActive(bool _b) { m_bActive = _b; }
	void OnCollisionEnter(CCollider* _pOther);

public:
	virtual void update();
	virtual void render(HDC _dc);
	CLONE(CSceneChanger);

public:
	CSceneChanger(SCENE_TYPE _eScene);
	~CSceneChanger();
};

