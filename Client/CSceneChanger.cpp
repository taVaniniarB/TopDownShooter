#include "CSceneChanger.h"
#include "CCollider.h"

CSceneChanger::CSceneChanger()
	: m_eScene(SCENE_TYPE::END)
	, m_bActive(false)
{
	CreateCollider();
	SetName(L"SceneChanger");
}

CSceneChanger::~CSceneChanger()
{
}

void CSceneChanger::OnCollisionEnter(CCollider* _pOther)
{
	if (m_bActive && _pOther->GetObj()->GetName() == L"Player")
	{
		ChangeScene(m_eScene);
	}
}