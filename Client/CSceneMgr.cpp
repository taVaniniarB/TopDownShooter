#include "pch.h"
#include "CSceneMgr.h"
#include "CScene_Main.h"
#include "CScene_Combat.h"
#include "Cscene_Tool.h"
#include "CTimeMgr.h"

CSceneMgr::CSceneMgr()
	: m_arrScene{}
	, m_pCurScene(nullptr)
	, m_pPrevScene(nullptr)
	, m_fCurTransitionTime(0.f)
	, m_bSceneChanging(false)
{}

CSceneMgr::~CSceneMgr()
{
	for (UINT i = 0; i < (UINT)SCENE_TYPE::END; ++i)
	{
		if (nullptr != m_arrScene[i])
		{
			delete m_arrScene[i];
		}
	}
	// �� ���� ����
	// �� ������ ���� ����, CScene* Ÿ����.
	// �� ���·� delete �غ��� �θ� �Ҹ��ڸ� ȣ��ż� �ڽ� �κ��� �� ����������?
	// ����, �Ҹ��ڿ� virtual �ٿ��� �ڽ� �κ� �Ҹ��� ȣ��ǵ��� ���־�� �Ѵ�
}

void CSceneMgr::init()
{
	// ��� Scene�� �����س��� ��ȯ���� ��
	m_arrScene[(UINT)SCENE_TYPE::MENU] = new CScene_Main();
	m_arrScene[(UINT)SCENE_TYPE::MENU]->SetName(L"Menu Scene");
	m_arrScene[(UINT)SCENE_TYPE::MENU]->SetSceneType(SCENE_TYPE::MENU);
	
	m_arrScene[(UINT)SCENE_TYPE::START] = new CScene_Combat(L"scene\\start");
	m_arrScene[(UINT)SCENE_TYPE::START]->SetName(L"Start Scene");
	m_arrScene[(UINT)SCENE_TYPE::START]->SetSceneType(SCENE_TYPE::START);

	m_arrScene[(UINT)SCENE_TYPE::STAGE_01] = new CScene_Combat(L"scene\\stage01");
	m_arrScene[(UINT)SCENE_TYPE::STAGE_01]->SetName(L"Lab Scene");
	m_arrScene[(UINT)SCENE_TYPE::STAGE_01]->SetSceneType(SCENE_TYPE::STAGE_01);

	m_arrScene[(UINT)SCENE_TYPE::STAGE_02] = new CScene_Combat(L"scene\\stage02");
	m_arrScene[(UINT)SCENE_TYPE::STAGE_02]->SetName(L"test Scene");
	m_arrScene[(UINT)SCENE_TYPE::STAGE_02]->SetSceneType(SCENE_TYPE::STAGE_02);

	m_arrScene[(UINT)SCENE_TYPE::BOSS] = new CScene_Combat(L"scene\\boss");
	m_arrScene[(UINT)SCENE_TYPE::BOSS]->SetName(L"Start Scene");
	m_arrScene[(UINT)SCENE_TYPE::BOSS]->SetSceneType(SCENE_TYPE::BOSS);
	
	m_arrScene[(UINT)SCENE_TYPE::TOOL] = new CScene_Tool;
	m_arrScene[(UINT)SCENE_TYPE::TOOL]->SetName(L"Tool Scene");
	m_arrScene[(UINT)SCENE_TYPE::TOOL]->SetSceneType(SCENE_TYPE::TOOL);

	// ���� �� ����
	m_pCurScene = m_arrScene[(UINT)SCENE_TYPE::MENU];
	m_pCurScene->Enter();

	// git Test
}

void CSceneMgr::update()
{
	m_pCurScene->update();
	m_pCurScene->finalUpdate();

	if (m_bSceneChanging)
	{
		m_fCurTransitionTime += fDT;
		// �� ��ȯ ȿ�� ���������� ���� ���߰� ����
		if (FADEOUT_TIME > m_fCurTransitionTime)
		{
			return;
		}

		// �����Լ� ChangeScene ȣ��
		::ChangeScene(m_eChangeScene);

		m_bSceneChanging = false;
	}
}

void CSceneMgr::render(HDC _dc)
{
	m_pCurScene->render(_dc);
}

void CSceneMgr::ChangeScene(SCENE_TYPE _eNext)
{ // ���� ��ȯ��, �� �������� ���������� �Ϸ�� �� ����Ǿ�� �Ѵ�
	// ���� �� ��ȯ Ʈ���� �� �ϴ� �̺�Ʈ ��� > ������ �����Ӻ��� ��ȯ
	m_pPrevScene = m_pCurScene;

	m_pCurScene->Exit();


	m_pCurScene = m_arrScene[(UINT)_eNext];

	m_pCurScene->Enter();
}