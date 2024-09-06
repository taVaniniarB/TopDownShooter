#include "CUI.h"
#include "CCamera.h"
#include "CKeyMgr.h"

#include "SelectGDI.h"

#include "CSceneMgr.h"
#include "CScene.h"

#include "CTexture.h"

CUI::CUI(bool _bCamAff)
	: m_pParentUI(nullptr)
	, m_bCamAffected(_bCamAff)
	, m_bMouseOn(false)
	, m_bLbtnDown(false)
	, m_pTex(nullptr)
{
}

// UI의 깊복을 위한 복사생성자
CUI::CUI(const CUI& _origin)
	: CObject(_origin)
	, m_pParentUI(nullptr)
	, m_bCamAffected(_origin.m_bCamAffected)
	, m_bMouseOn(false)
	, m_bLbtnDown(false)
{
	for (size_t i = 0; i < _origin.m_vecChildUI.size(); ++i)
	{
		AddChild(_origin.m_vecChildUI[i]->Clone());
	}
}

CUI::~CUI()
{
	Safe_Delete_Vec(m_vecChildUI);
}

void CUI::update()
{
	// 자신의 1프레임 단위 update 동작

	// child ui update
	update_child();
}

void CUI::render(HDC _dc)
{// UI는 타일과 마찬가지로 좌상단 기준으로 렌더할 것
	Vec2 vPos = GetFinalPos();
	Vec2 vScale = GetScale();

	if (m_bCamAffected)
	{
		vPos = CCamera::GetInst()->GetRenderPos(vPos);
	}

	if (m_bLbtnDown)
	{
		SelectGDI select(_dc, PEN_TYPE::GREEN);
		Rectangle(_dc
			, (int)(vPos.x)
			, (int)(vPos.y)
			, (int)(vPos.x + vScale.x)
			, (int)(vPos.y + vScale.y));
	}

	else
	{
		Rectangle(_dc
			, (int)(vPos.x)
			, (int)(vPos.y)
			, (int)(vPos.x + vScale.x)
			, (int)(vPos.y + vScale.y));
	}


	// child ui render
	render_child(_dc);
}


// 부모 위치에 오프셋만 더해 계산함으로써
// 그룹화해서 이동하는 느낌
void CUI::finalUpdate()
{
	// 왜 UI의 업데이트는 final Update에 적은 걸까?
	// 다른 모든 오브젝트들의 업데이트가 선행된 다음이어야 하기 때문에?
	CObject::finalUpdate();

	// 자기 자신의 최종 위치를 결정한다
	
	// 부모가 없는 최상위 UI: 본인 position(오프셋값이라는 의미도 있음)이 곧 finalPos
	m_vFinalPos = GetPos();
	// 부모가 있다면, 부모의 최종 위치+오프셋
	if (GetParent())
	{
		Vec2 vParentPos = GetParent()->GetFinalPos();
		m_vFinalPos += vParentPos;
	}

	// UI 마우스 체크
	MouseOnCheck();

	finalUpdate_Child();
}

void CUI::update_child()
{
	for (size_t i = 0; i < m_vecChildUI.size(); ++i)
	{
		m_vecChildUI[i]->update();
	}
}

void CUI::finalUpdate_Child()
{
	for (size_t i = 0; i < m_vecChildUI.size(); ++i)
	{
		m_vecChildUI[i]->finalUpdate();
	}
}

void CUI::render_child(HDC _dc)
{
	for (size_t i = 0; i < m_vecChildUI.size(); ++i)
	{
		m_vecChildUI[i]->render(_dc);
	}
}

// 마우스 좌표를 받아와서, 현재 마우스가 UI에 올라와 있는지 체크
void CUI::MouseOnCheck()
{
	Vec2 vScale = GetScale();
	Vec2 vMousePos = MOUSE_POS;

	// 카메라의 영향을 받냐 안 받냐 분기 처리

	// 영향 X: 렌더링 기준 좌표끼리 바로 비교
	// 영향 O(ex: 몬스터 체력바): 실제좌표로 변환하여 비교

	if (m_bCamAffected)
	{
		vMousePos = CCamera::GetInst()->GetRealPos(vMousePos);
	}

	if (m_vFinalPos.x <= vMousePos.x && m_vFinalPos.x + vScale.x >= vMousePos.x
		&& m_vFinalPos.y <= vMousePos.y && m_vFinalPos.y + vScale.y >= vMousePos.y)
	{
		m_bMouseOn = true;
	}
	else
	{
		m_bMouseOn = false;
	}
}

void CUI::MouseOn()
{
}

void CUI::MouseLbtnDown()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	
	pCurScene->SetUIClicked(true);
	//UIBTNDown(pCurScene);
}

void CUI::MousebtnUp()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	UIBTNUp(pCurScene);
}

void CUI::MouseLbtnClicked()
{
}
