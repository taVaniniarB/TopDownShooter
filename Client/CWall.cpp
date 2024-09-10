#include "CWall.h"

#include "CCollider.h"
#include "CTexture.h"
#include "CResMgr.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CRigidBody.h"

CWall::CWall()
	: m_iImgIdx(-1)
	, m_eType(WALL_TYPE::NONE)
{
	CreateCollider();
}

CWall::~CWall()
{
}


void CWall::start()
{
	Vec2 vScale = GetScale();
	GetCollider()->SetScale(vScale);
	GetCollider()->SetOffsetPos(vScale - vScale/2);
}

void CWall::update()
{
}

void CWall::render(HDC _dc)
{
	CTexture* pTileTex = CObject::GetTexture();

	// �ؽ�ó ������ �ȶ��� ������ ����� + �ε����� -1
	if (nullptr == pTileTex || -1 == m_iImgIdx)
	{
		Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(GetPos());
		Vec2 vScale = GetScale();

		Rectangle(_dc
			, (int)(vRenderPos.x)
			, (int)(vRenderPos.y)
			, (int)(vRenderPos.x + vScale.x)
			, (int)(vRenderPos.y + vScale.y));
		return;
	}

	UINT iWidth = pTileTex->Width();
	UINT iHeight = pTileTex->Height();

	// Ÿ�� ���� ���̿� ���� �ִ� �� ����
	UINT iMaxCol = iWidth / TILE_SIZE;

	//�� ����(���� ����: �ε��� �ʰ� ����ó��)
	UINT iMaxRow = iHeight / TILE_SIZE;

	UINT iCurRow = (UINT)m_iImgIdx / iMaxCol;
	UINT iCurCol = (UINT)m_iImgIdx % iMaxCol;

	// �̹��� ������ �Ѿ �ε���
	if (iMaxCol <= iCurCol)
		assert(nullptr);


	Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(GetPos());
	Vec2 vScale = GetScale();


	BitBlt(_dc
		, (int)(vRenderPos.x)
		, (int)(vRenderPos.y)
		, (int)(vScale.x)
		, (int)(vScale.y)
		, pTileTex->GetDC()
		, iCurCol * TILE_SIZE
		, iCurRow * TILE_SIZE
		, SRCCOPY);

	//component_render(_dc);
}

// �浹ü���� �浹 �̺�Ʈ �߻� �� ����� �Լ�

void CWall::OnCollisionEnter(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();
	if (pOtherObj->GetName() == L"Player" || pOtherObj->GetName() == L"Monster")
	{
		// DT�� ���� ����� ���� ���� (����ϰ� �� ���� �ö󼭵���)
		// ������Ʈ�� �ƴ� �浹ü �� �񱳷� ����ؾ� �Ѵ�.
		// Enter�Ӹ� �ƴ϶� OnCollision������ ������ ó�� �ʿ��� (���� ���ٴڿ� �� ���̰� ���� ���� enter�� �ƴ� OnCollision�̱� ������, �İ�� ���� �������ָ� enter�� �ɸ��� �ʾƼ�
		// ���� �ӵ� �ۿ� �� ��� �İ��� ��)

		// �÷��̾� �浹ü ����
		Vec2 vObjPos = _pOther->GetFinalPos();
		Vec2 vObjScale = _pOther->GetScale();
		
		// vObjPos += vObjScale/2;

		Vec2 vPrevObjPos = _pOther->GetPrevPos();

		// �� ����
		Vec2 vPos = GetCollider()->GetFinalPos();
		Vec2 vScale = GetCollider()->GetScale();

		// ���� ������ �÷��̾� ��ǥ �����ͼ� ���� ��� �鿡 �������� ��
		WALL_DIR collisionDir = WALL_DIR::END;
		
		

		// ���� ������ �÷��̾� �ظ� y�� Wall ���� y���� ���Ҵ�
		if ((vPrevObjPos.y + (vObjScale.y / 2)) < (vPos.y - (vScale.y / 2)))
			collisionDir = WALL_DIR::TOP;
		// ���� ������ �÷��̾� ������ ���� �ٴں��� �Ǵ� (���� �Ʒ��鿡 �浹) 
		else if (vPrevObjPos.y - (vObjScale.y / 2) > vPos.y + (vScale.y / 2))
			collisionDir = WALL_DIR::BOTTOM;
		// ���� ������ �÷��̾� �������� Wall ���ʺ��� �۾Ҵ� (���� ���� �浹)
		else if (vPrevObjPos.x + (vObjScale.x / 2) < vPos.x - (vScale.x / 2))
			collisionDir = WALL_DIR::LEFT;
		// ���� ������ �÷��̾� ������ ���� �����ʺ��� �Ǵ� (���� ������ �浹)
		else if (vPrevObjPos.x - (vObjScale.x / 2) > vPos.x + (vScale.x / 2))
			collisionDir = WALL_DIR::RIGHT;

			

		if (WALL_DIR::TOP == collisionDir) // ����� �浹
		{
			// �İ�� ���� ���ϱ�
			// �������� pos������ �Ÿ�
			float fLen = abs(vObjPos.y - vPos.y);
			// ��ħ������ ���� ª���� �Ÿ��� ���� �Ÿ��� ���� ���� (= �İ�� ����)
			float fValue = (vObjScale.y / 2.f + vScale.y / 2.f) - fLen;

			vObjPos = pOtherObj->GetPos();

			vObjPos.y -= (fValue + 1);
		}
		else if (WALL_DIR::BOTTOM == collisionDir) // �Ʒ���� �浹
		{
			// �İ�� ����
			float fLen = abs(vObjPos.y - vPos.y);
			float fValue = (vObjScale.y / 2.f + vScale.y / 2.f) - fLen;

			vObjPos = pOtherObj->GetPos();

			vObjPos.y += (fValue + 1);
		}
		else if (WALL_DIR::LEFT == collisionDir) // ���� ��� �浹
		{
			// �İ�� ����
			float fLen = abs(vObjPos.x - vPos.x);
			float fValue = (vObjScale.x / 2.f + vScale.x / 2.f) - fLen;

			vObjPos = pOtherObj->GetPos();

			vObjPos.x -= (fValue + 1);
		}
		else if (WALL_DIR::RIGHT == collisionDir) // ������ ��� �浹
		{
			// �İ�� ����
			float fLen = abs(vObjPos.x - vPos.x);
			float fValue = (vObjScale.x / 2.f + vScale.x / 2.f) - fLen;

			vObjPos = pOtherObj->GetPos();

			vObjPos.x += (fValue + 1);
		}

		pOtherObj->SetPos(vObjPos);
		pOtherObj->GetRigidBody()->SetGround(true, collisionDir);
	}
}
	
void CWall::OnCollision(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();
	if (pOtherObj->GetName() == L"Player" || pOtherObj->GetName() == L"Monster")
	{
		// DT�� ���� ����� ���� ���� (����ϰ� �� ���� �ö󼭵���)
		// ������Ʈ�� �ƴ� �浹ü �� �񱳷� ����ؾ� �Ѵ�.
		// Enter�Ӹ� �ƴ϶� OnCollision������ ������ ó�� �ʿ��� (���� ���ٴڿ� �� ���̰� ���� ���� enter�� �ƴ� OnCollision�̱� ������, �İ�� ���� �������ָ� enter�� �ɸ��� �ʾƼ�
		// ���� �ӵ� �ۿ� �� ��� �İ��� ��)

		// �÷��̾� �浹ü ����
		Vec2 vObjPos = _pOther->GetFinalPos();
		Vec2 vObjScale = _pOther->GetScale();


		Vec2 vPrevObjPos = _pOther->GetPrevPos();

		// �� ����
		Vec2 vPos = GetCollider()->GetFinalPos();
		Vec2 vScale = GetCollider()->GetScale();

		//���� ������ ������ �ʿ䰡 ����? ������ equal�� ���̶�

		WALL_DIR collisionDir = pOtherObj->GetRigidBody()->GetGroundType();

		if (WALL_DIR::TOP == collisionDir) // ����� �浹
		{
			// �İ�� ���� ���ϱ�
			// �������� pos������ �Ÿ�
			float fLen = abs(vObjPos.y - vPos.y);
			// ��ħ������ ���� ª���� �Ÿ��� ���� �Ÿ��� ���� ���� (= �İ�� ����)
			float fValue = (vObjScale.y / 2.f + vScale.y / 2.f) - fLen;

			vObjPos = pOtherObj->GetPos();

			vObjPos.y -= (fValue + 1);
		}
		else if (WALL_DIR::BOTTOM == collisionDir) // �Ʒ���� �浹
		{
			// �İ�� ����
			float fLen = abs(vObjPos.y - vPos.y);
			float fValue = (vObjScale.y / 2.f + vScale.y / 2.f) - fLen;

			vObjPos = pOtherObj->GetPos();

			vObjPos.y += (fValue + 1);
		}
		else if (WALL_DIR::LEFT == collisionDir) // ���� ��� �浹
		{
			// �İ�� ����
			float fLen = abs(vObjPos.x - vPos.x);
			float fValue = (vObjScale.x / 2.f + vScale.x / 2.f) - fLen;

			vObjPos = pOtherObj->GetPos();

			vObjPos.x -= (fValue + 1);
		}
		else if (WALL_DIR::RIGHT == collisionDir) // ������ ��� �浹
		{
			// �İ�� ����
			float fLen = abs(vObjPos.x - vPos.x);
			float fValue = (vObjScale.x / 2.f + vScale.x / 2.f) - fLen;

			vObjPos = pOtherObj->GetPos();

			vObjPos.x += (fValue + 1);
		}

		pOtherObj->SetPos(vObjPos);

		// Ư�� �������� �̵� ��ü�� ��������
		pOtherObj->GetRigidBody()->SetGround(true, collisionDir);
	}
}

void CWall::OnCollisionExit(CCollider* _pOther)
{
	// �ٴڰ� �浹 ���°� �ƴ� �� �׶��� ����

	CObject* pOtherObj = _pOther->GetObj();
	if (pOtherObj->GetName() == L"Player")
	{
		pOtherObj->GetRigidBody()->SetGround(false, WALL_DIR::END);
	}
}

void CWall::Save(FILE* _pFile)
{
	// ��ġ �ۼ�
	Vec2 vPos = GetPos();
	fwrite(&vPos, sizeof(Vec2), 1, _pFile);

	// ������ �ۼ�
	Vec2 vScale = GetScale();
	fwrite(&vScale, sizeof(Vec2), 1, _pFile);

	// �ؽ��� Ű �ۼ�
	CTexture* pTex = GetTexture();
	wstring wTexKey = pTex->GetKey();
	string strName = string(wTexKey.begin(), wTexKey.end());
	fprintf(_pFile, strName.c_str());
	fprintf(_pFile, "\n");
	// �ؽ��� ����� �ۼ�
	strName = string(pTex->GetRelativePath().begin(), pTex->GetRelativePath().end());
	fprintf(_pFile, strName.c_str());
	fprintf(_pFile, "\n");

	// �ؽ��� �ε��� ����
	fwrite(&m_iImgIdx, sizeof(int), 1, _pFile);

	// �׷� Ÿ�� ����
	fwrite(&m_eType, sizeof(WALL_TYPE), 1, _pFile);
}

void CWall::Load(FILE* _pFile)
{
	// ��ġ ��� ä���
	Vec2 vPos = {};
	fread(&vPos, sizeof(Vec2), 1, _pFile);
	SetPos(vPos);

	// ������ ��� ä���
	Vec2 vScale = {};
	fread(&vScale, sizeof(Vec2), 1, _pFile);
	SetScale(vScale);

	// �ؽ��� ��� ä���
	// �ؽ��� Ű �������� / �ؽ��� ����� ��������
	char szBuff[256];
	string str;
	FScanf(szBuff, _pFile);
	str = szBuff;
	wstring strTexKey = wstring(str.begin(), str.end());


	FScanf(szBuff, _pFile);
	str = szBuff;
	wstring strTexPath = wstring(str.begin(), str.end());
	// Ű�� ��� ������� �ؽ�ó �ε�
	CTexture* pTex = CResMgr::GetInst()->LoadTexture(strTexKey, strTexPath);
	SetTexture(pTex);


	// �ؽ��� �ε��� ��� ä���
	fread(&m_iImgIdx, sizeof(int), 1, _pFile);

	GROUP_TYPE eType;
	fread(&eType, sizeof(GROUP_TYPE), 1, _pFile);

	CScene* pScene = CSceneMgr::GetInst()->GetCurScene();
	int iType = (int)eType + 2;
	pScene->AddObject(this, (GROUP_TYPE)iType);
}
