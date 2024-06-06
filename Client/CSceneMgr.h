#pragma once

class CScene;

class CSceneMgr
{
	SINGLE(CSceneMgr);
private:
	CScene* m_arrScene[(UINT)SCENE_TYPE::END]; // 모든 씬 목록
	CScene* m_pCurScene; // 현재 씬
	CScene* m_pPrevScene;

public:
	void init();
	void update();
	void render(HDC _dc);

public:
	CScene* GetCurScene() { return m_pCurScene; }
	CScene* GetPrevScene() { return m_pPrevScene; }

private:
	// ㄹㅇ 전환 시 실행되는 함수로, 아무나 호출해선 안 됨
	void ChangeScene(SCENE_TYPE _eNext);

	friend class CEventMgr;
};

