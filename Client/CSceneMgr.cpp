#include "pch.h"
#include "CSceneMgr.h"

#include "Cscene_Start.h"
#include "Cscene_Tool.h"

CSceneMgr::CSceneMgr()
	: m_arrScene{}
	, m_pCurScene(nullptr)
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
	// 씬 전부 삭제
	// 씬 저장한 형태 보면, CScene* 타입임.
	// 이 상태로 delete 해봤자 부모 소멸자만 호출돼서 자식 부분은 안 지워지겠지?
	// 따라서, 소멸자에 virtual 붙여서 자식 부분 소멸자 호출되도록 해주어야 한다
}

void CSceneMgr::init()
{
	// 모든 Scene들 생성해놓고 전환해줄 것
	m_arrScene[(UINT)SCENE_TYPE::START] = new CScene_Start;
	m_arrScene[(UINT)SCENE_TYPE::START]->SetName(L"Start Scene");
	
	m_arrScene[(UINT)SCENE_TYPE::TOOL] = new CScene_Tool;
	m_arrScene[(UINT)SCENE_TYPE::TOOL]->SetName(L"Tool Scene");
	
	//m_arrScene[(UINT)SCENE_TYPE::STAGE_01] = new CScene_Stage01;
	//m_arrScene[(UINT)SCENE_TYPE::STAGE_02] = new CScene_Stage02;

	// 현재 씬 지정
	m_pCurScene = m_arrScene[(UINT)SCENE_TYPE::TOOL];
	m_pCurScene->Enter();
}

void CSceneMgr::update()
{
	m_pCurScene->update();
	m_pCurScene->finalUpdate();
}

void CSceneMgr::render(HDC _dc)
{
	m_pCurScene->render(_dc);
}

void CSceneMgr::ChangeScene(SCENE_TYPE _eNext)
{ // 씬의 전환은, 한 프레임의 마무리까지 완료된 후 진행되어야 한다
	// 따라서 씬 전환 트리거 시 일단 이벤트 등록 > 다음번 프레임부터 전환
	
	m_pPrevScene = m_pCurScene;

	m_pCurScene->Exit();
	m_pCurScene = m_arrScene[(UINT)_eNext];

	m_pCurScene->Enter();
}