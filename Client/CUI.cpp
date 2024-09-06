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

// UI�� ���� ���� ���������
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
	// �ڽ��� 1������ ���� update ����

	// child ui update
	update_child();
}

void CUI::render(HDC _dc)
{// UI�� Ÿ�ϰ� ���������� �»�� �������� ������ ��
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


// �θ� ��ġ�� �����¸� ���� ��������ν�
// �׷�ȭ�ؼ� �̵��ϴ� ����
void CUI::finalUpdate()
{
	// �� UI�� ������Ʈ�� final Update�� ���� �ɱ�?
	// �ٸ� ��� ������Ʈ���� ������Ʈ�� ����� �����̾�� �ϱ� ������?
	CObject::finalUpdate();

	// �ڱ� �ڽ��� ���� ��ġ�� �����Ѵ�
	
	// �θ� ���� �ֻ��� UI: ���� position(�����°��̶�� �ǹ̵� ����)�� �� finalPos
	m_vFinalPos = GetPos();
	// �θ� �ִٸ�, �θ��� ���� ��ġ+������
	if (GetParent())
	{
		Vec2 vParentPos = GetParent()->GetFinalPos();
		m_vFinalPos += vParentPos;
	}

	// UI ���콺 üũ
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

// ���콺 ��ǥ�� �޾ƿͼ�, ���� ���콺�� UI�� �ö�� �ִ��� üũ
void CUI::MouseOnCheck()
{
	Vec2 vScale = GetScale();
	Vec2 vMousePos = MOUSE_POS;

	// ī�޶��� ������ �޳� �� �޳� �б� ó��

	// ���� X: ������ ���� ��ǥ���� �ٷ� ��
	// ���� O(ex: ���� ü�¹�): ������ǥ�� ��ȯ�Ͽ� ��

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
