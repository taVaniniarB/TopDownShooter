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
	// ��ü�� �����ڸ� �̿��ϴ� ���
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

	SetTextColor(_dc, RGB(255, 255, 255));   // �ؽ�Ʈ ���� (����)
	SetBkMode(_dc, TRANSPARENT);       // ��� ���� ����
	TextOut(_dc, (int)(vRenderPos.x), (int)(vRenderPos.y), strName, wcslen(strName)); // �ؽ�Ʈ ��� ��ġ

#endif
}
