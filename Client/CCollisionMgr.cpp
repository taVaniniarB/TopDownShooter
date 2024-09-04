#include "pch.h"
#include "CCollisionMgr.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CObject.h"
#include "CCollider.h"

CCollisionMgr::CCollisionMgr()
	: m_arrCheck{}
{}

CCollisionMgr::~CCollisionMgr()
{}

void CCollisionMgr::update()
{
	// �ݺ������� �迭 Ȯ���ϸ�, 1 üũ �� �ִٸ� �� �׷�� �� �׷��� �浹�̱���
	// �ϰ� �Ÿ� �� �־�� ��
	for (UINT iRow = 0; iRow < (UINT)GROUP_TYPE::END; ++iRow)
	{
		// ���ݸ� �˻��� ���̱� ������, ������ϵ��� �����ؼ�
		// for���� 0���� ������ �ƴ϶� iRow���� ������!!
		for (UINT iCol = iRow; iCol < (UINT)GROUP_TYPE::END; ++iCol)
		{
			if (m_arrCheck[iRow] & (1 << iCol))
			{
				// ���� ���� ����� ������Ʈ��ϵ��� ����
				// �׷� ������ ��û
				// ���� ���� row�� �տ�, ū ���� col�� �ھ� �ξ� ���� ����
				CollisionUpdateGroup((GROUP_TYPE)iRow, (GROUP_TYPE)iCol);
			}
		}
	}
}

void CCollisionMgr::CollisionUpdateGroup(GROUP_TYPE _eLeft, GROUP_TYPE _eRight)
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	
	// ���۷����� ��Ī / ���Ͻ��� ����
	// vecLeft�� GetGroupObject ��ȯ��� ���Ϳ� ���Ͻ� �ϰڴ�
	// ��ȯ�� �ְ� const�ϱ� const �ٿ���
	// ���۷����� ���������� ������ ���� �׻� �����ϵ��� ����
	const vector<CObject*>& vecLeft = pCurScene->GetGroupObject(_eLeft);
	const vector<CObject*>& vecRight = pCurScene->GetGroupObject(_eRight);

	map<ULONGLONG, bool>::iterator iter;

	// ����� ��
	// ���� �׷��� ������ ���, �� �ڽŰ��� �浹 ���ܼ���
	// �ش� ������Ʈ�� �浹ü�� ���� ���

	for (size_t i = 0; i < vecLeft.size(); ++i)
	{
		// �ش� ������Ʈ�� �浹ü �̺��� �� �н�
		if (nullptr == vecLeft[i]->GetCollider())
		{
			continue;
		}

		for (size_t j = 0; j < vecRight.size(); ++j)
		{
			// �浹ü�� ���ų�, �ڱ� �ڽŰ��� �浹 ��� �н�
			if (nullptr == vecRight[j]->GetCollider()
				|| vecLeft[i] == vecRight[j])
			{
				continue;
			}

			CCollider* pLeftCol = vecLeft[i]->GetCollider();
			CCollider* pRightCol = vecRight[j]->GetCollider();

			// �� �浹ü�� ID�� �����ͼ�, �� �浹ü ���� ���̵� ����
			COLLIDER_ID ID;
			ID.iLeft_id = pLeftCol->GetID();
			ID.iRight_id = pRightCol->GetID();
			ID.ID; // �̰��� ���� key�� ���ڴ�

			// �� �浹ü ���� �浹 ���θ� �����ϴ� ��� ����Ű�� ���ͷ�����
			iter = m_mapColInfo.find(ID.ID);

			// �� end iterator��� ����, ���� ��ȸ�� �����Ͱ� ���ٴ� ���̹Ƿ� ������ �߰�
			if (m_mapColInfo.end() == iter)
			{
				m_mapColInfo.insert(make_pair(ID.ID, false));
				iter = m_mapColInfo.find(ID.ID);
			}
			// iter�� �� �浹ü�� �浹 ���θ� �����ϴ� ��带 ����Ų��

			// �� ������Ʈ�� �浹ü�� �浹üũ �Լ��� ��������
			if (IsCollision(pLeftCol, pRightCol))
			{
				// ���� �浹 ���̴�

				if (iter->second)
				{// �������� �浹 ���̾���

					if (vecLeft[i]->IsDead() || vecRight[j]->IsDead())
					{
						// �ٵ� �� �� �ϳ��� ���� ������ ��, �浹 ����������
						pLeftCol->OnCollisionExit(pRightCol);
						pRightCol->OnCollisionExit(pLeftCol);
						iter->second = false;
					}
					else
					{
						pLeftCol->OnCollision(pRightCol);
						pRightCol->OnCollision(pLeftCol);
					}
					
				}
				else
				{// �������� �浹���� �ʴµ� �̹��� �浹�� (�浹���Խ���)

					if (!vecLeft[i]->IsDead() && !vecRight[j]->IsDead())
					{ // �� �� dead�� �ƴ� ��, �浹 Enter ����
						pLeftCol->OnCollisionEnter(pRightCol);
						pRightCol->OnCollisionEnter(pLeftCol);
						iter->second = true;
					}
				}
			}
			else // ���� �浹���� ����
			{
				if (iter->second)
				{// �������� �浹�ϰ� �־���
					pLeftCol->OnCollisionExit(pRightCol);
					pRightCol->OnCollisionExit(pLeftCol);
					iter->second = false;
				}
			}
		}
	}
}

// ��ǥ�� ��ȸ�Ͽ� ��¥ �浹 �Ѵ� �� �Ѵٸ� �˷��ִ� �Լ�
bool CCollisionMgr::IsCollision(CCollider* _pLeftCol, CCollider* _pRightCol)
{
	Vec2 vLeftPos = _pLeftCol->GetFinalPos();
	Vec2 vLeftScale = _pLeftCol->GetScale();

	Vec2 vRightPos = _pRightCol->GetFinalPos();
	Vec2 vRightScale = _pRightCol->GetScale();

	if (abs(vRightPos.x - vLeftPos.x) <= (vLeftScale.x + vRightScale.x) / 2.f
		&& abs(vRightPos.y - vLeftPos.y) <= (vLeftScale.y + vRightScale.y) / 2.f)
	{
		return true;
	}

	// �� �߽���ǥ�� x, y �� �� scale / 2 �̸� ��ŭ ������ ���� ��
	// �� ��ǥ�� ���̰� (left.scale / 2 + right.scale / 2) / 2 �̸�
	return false;
}



// �浹 ��� ���̺� ����
void CCollisionMgr::CheckGroup(GROUP_TYPE _eLeft, GROUP_TYPE _eRight)
{
	// �� ū ���� �׷� Ÿ���� ��

	UINT iRow = (UINT)_eLeft;
	UINT iCol = (UINT)_eRight;

	if (iCol < iRow)
	{
		iRow = (UINT)_eRight;
		iCol = (UINT)_eLeft;
	}

	// �� �ڸ��� ��Ʈ�� ���ΰ�?
	if (m_arrCheck[iRow] & (1 << iCol))
	{
		// ��Ʈ���� (üũ�ڽ� �� �� �� Ŭ���ؼ� �����ǵ�)
		m_arrCheck[iRow] &= ~(1 << iCol);
	}
	else
	{
		// ��Ʈ �־��ֱ�
		m_arrCheck[iRow] |= (1 << iCol);
	}
}
