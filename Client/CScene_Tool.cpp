#include "pch.h"
#include "CScene_Tool.h"
#include "CKeyMgr.h"

#include "CSceneMgr.h"


CScene_Tool::CScene_Tool()
{
}

CScene_Tool::~CScene_Tool()
{
}

void CScene_Tool::update()
{
	CScene::update();

	if (KEY_TAP(KEY::ENTER))
	{
		ChangeScene(SCENE_TYPE::START);
	}
}

void CScene_Tool::Enter()
{
	const vector<CObject*>* prevSceneArrObj = CSceneMgr::GetInst()->GetPrevScene()->GetObjectArr();
	
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		for (size_t j = 0; j < prevSceneArrObj[i].size(); ++j)
		{
			AddObject(prevSceneArrObj[i][j], GROUP_TYPE(i));
		}
	}
	// 충돌 시 상태가 그대로 넘어옴. 제어 필요
}

void CScene_Tool::Exit()
{
}



