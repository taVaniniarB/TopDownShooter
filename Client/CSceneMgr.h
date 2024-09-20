#pragma once

class CScene;

class CSceneMgr
{
	SINGLE(CSceneMgr);
private:
	CScene* m_arrScene[(UINT)SCENE_TYPE::END]; // ��� �� ���
	CScene* m_pCurScene; // ���� ��
	CScene* m_pPrevScene;

	bool m_bSceneChanging;	// Ʈ������ ���ΰ�?
	float m_fCurTransitionTime; // Ʈ������ ȿ�� �����ð� ������
	SCENE_TYPE m_eChangeScene; // �ٲ� ��

public:
	void init();
	void update();
	void render(HDC _dc);

public:
	CScene* GetCurScene() { return m_pCurScene; }
	CScene* GetPrevScene() { return m_pPrevScene; }

	void SetSceneChange(bool _b, SCENE_TYPE _eSceneType)
	{
		m_bSceneChanging = _b; 
		m_eChangeScene = _eSceneType;
	}

private:
	// ���� ��ȯ �� ����Ǵ� �Լ���, �ƹ��� ȣ���ؼ� �� ��
	void ChangeScene(SCENE_TYPE _eNext);

	friend class CEventMgr;
};

