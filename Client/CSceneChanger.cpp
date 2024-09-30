#include "CSceneChanger.h"
#include "CCollider.h"

#include "SelectGDI.h"

CSceneChanger::CSceneChanger(SCENE_TYPE _eType)
	: m_eScene(_eType)
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

void CSceneChanger::update()
{
}

void CSceneChanger::render(HDC _dc)
{
#ifdef _DEBUG
	PEN_TYPE ePen = PEN_TYPE::YELLOW;
	// 객체의 생성자를 이용하는 방식
	SelectGDI p(_dc, ePen);
	SelectGDI b(_dc, BRUSH_TYPE::HOLLOW);

	Vec2 vPos = GetPos();
	Vec2 vScale = GetScale();

	Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(vPos);

	Rectangle(_dc
		, (int)(vRenderPos.x - vScale.x / 2.f)
		, (int)(vRenderPos.y - vScale.y / 2.f)
		, (int)(vRenderPos.x + vScale.x / 2.f)
		, (int)(vRenderPos.y + vScale.y / 2.f));

	wchar_t strName[10] = {};
	wcscpy_s(strName, L"SC");

	SetTextColor(_dc, RGB(255, 255, 255));   // 텍스트 색상 (검정)
	SetBkMode(_dc, TRANSPARENT);       // 배경 투명 설정
	TextOut(_dc, (int)(vRenderPos.x), (int)(vRenderPos.y), strName, wcslen(strName)); // 텍스트 출력 위치

#endif
}
