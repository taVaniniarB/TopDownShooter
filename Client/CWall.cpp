#include "CWall.h"

#include "CCollider.h"
//#include "CGravity.h"

CWall::CWall()
{
	CreateCollider();
}

CWall::~CWall()
{
}


void CWall::start()
{
	GetCollider()->SetScale(Vec2(GetScale()));
}

void CWall::update()
{
}

void CWall::render(HDC _dc)
{
	Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(GetPos());
	Vec2 vScale = GetScale();

	Rectangle(_dc
		, (int)(vRenderPos.x)
		, (int)(vRenderPos.y)
		, (int)(vRenderPos.x + vScale.x)
		, (int)(vRenderPos.y + vScale.y));
}

// �浹ü���� �浹 �̺�Ʈ �߻� �� ����� �Լ�
void CWall::OnCollisionEnter(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();
	if (pOtherObj->GetName() == L"Player")
	{
		// Ground �������� �˷��ش�
		//pOtherObj->GetGravity()->SetGround(true);

		// DT�� ���� ����� ���� ���� (����ϰ� �� ���� �ö󼭵���)
		// ������Ʈ�� �ƴ� �浹ü �� �񱳷� ����ؾ� �Ѵ�.
		// Enter�Ӹ� �ƴ϶� OnCollision������ ������ ó�� �ʿ��� (���� ���ٴڿ� �� ���̰� ���� ���� enter�� �ƴ� OnCollision�̱� ������, �İ�� ���� �������ָ� enter�� �ɸ��� �ʾƼ�
		// ���� �ӵ� �ۿ� �� ��� �İ��� ��)

		Vec2 vObjPos = _pOther->GetFinalPos();
		Vec2 vObjScale = _pOther->GetScale();

		Vec2 vPos = GetCollider()->GetFinalPos();
		Vec2 vScale = GetCollider()->GetScale();



		// ���� ������ �÷��̾� ��ǥ �����ͼ� ���� ��� �鿡 �������� ��


		// �İ�� ����
		float fLen = abs(vObjPos.y - vPos.y);
		float fValue = (vObjScale.y / 2.f + vScale.y / 2.f) - fLen;

		vObjPos = pOtherObj->GetPos();

		vObjPos.y -= fValue;

		pOtherObj->SetPos(vObjPos);
	}
}
	
void CWall::OnCollision(CCollider* _pOther)
{
	// Enter�Ӹ� �ƴ϶� OnCollision������ ������ ó�� �ʿ��� (���� ���ٴڿ� �� ���̰� ���� ���� enter�� �ƴ� OnCollision�̱� ������, �İ�� ���� �������ָ� enter�� �ɸ��� �ʾƼ�
	// ���� �ӵ� �ۿ� �� ��� �İ��� ��)

	CObject* pOtherObj = _pOther->GetObj();
	if (pOtherObj->GetName() == L"Player")
	{
		// Ground �������� �˷��ش�
		//pOtherObj->GetGravity()->SetGround(true);

		Vec2 vObjPos = _pOther->GetFinalPos();
		Vec2 vObjScale = _pOther->GetScale();

		Vec2 vPos = GetCollider()->GetFinalPos();
		Vec2 vScale = GetCollider()->GetScale();

		// �İ�� ����
		float fLen = abs(vObjPos.y - vPos.y);
		float fValue = (vObjScale.y / 2.f + vScale.y / 2.f) - fLen;

		vObjPos = pOtherObj->GetPos();

		vObjPos.y -= fValue;

		// �÷��̾� ��ǥ�� ����
		pOtherObj->SetPos(vObjPos);
	}

	// �̻��ϰ� �� �浹 ��
	if (pOtherObj->GetName() == L"Missile")
	{
		// �̻��� ��ü ����
	}
}

void CWall::OnCollisionExit(CCollider* _pOther)
{
	// �ٴڰ� �浹 ���°� �ƴ� �� �׶��� ����

	CObject* pOtherObj = _pOther->GetObj();
	if (pOtherObj->GetName() == L"Player")
	{
		//pOtherObj->GetGravity()->SetGround(false);
	}
}