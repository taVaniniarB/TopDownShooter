#include "CWall.h"

#include "CCollider.h"
//#include "CGravity.h"
#include "CTexture.h"
#include "CResMgr.h"
#include "CSceneMgr.h"
#include "CScene.h"

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
	GetCollider()->SetScale(Vec2(GetScale()));
}

void CWall::update()
{
}

void CWall::render(HDC _dc)
{/*
	Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(GetPos());
	Vec2 vScale = GetScale();

	Rectangle(_dc
		, (int)(vRenderPos.x)
		, (int)(vRenderPos.y)
		, (int)(vRenderPos.x + vScale.x)
		, (int)(vRenderPos.y + vScale.y));*/


	CTexture* pTileTex = CObject::GetTexture();

	// 텍스처 없으면 안띄우고 있으면 띄우자 + 인덱스가 -1
	if (nullptr == pTileTex || -1 == m_iImgIdx)
		return;

	UINT iWidth = pTileTex->Width();
	UINT iHeight = pTileTex->Height();

	// 타일 파일 높이에 따른 최대 열 개수
	UINT iMaxCol = iWidth / TILE_SIZE;

	//행 개수(쓰는 이유: 인덱스 초과 예외처리)
	UINT iMaxRow = iHeight / TILE_SIZE;

	UINT iCurRow = (UINT)m_iImgIdx / iMaxCol;
	UINT iCurCol = (UINT)m_iImgIdx % iMaxCol;

	// 이미지 범위를 넘어선 인덱스
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
}

// 충돌체에서 충돌 이벤트 발생 시 실행될 함수
void CWall::OnCollisionEnter(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();
	if (pOtherObj->GetName() == L"Player" || pOtherObj->GetName() == L"Monster")
	{
		// DT로 인해 생기는 오차 보정 (깔끔하게 땅 위로 올라서도록)
		// 오브젝트가 아닌 충돌체 간 비교로 계산해야 한다.
		// Enter뿐만 아니라 OnCollision에서도 동일한 처리 필요함 (발을 땅바닥에 딱 붙이고 있을 때는 enter가 아닌 OnCollision이기 때문에, 파고든 값을 보정해주면 enter에 걸리지 않아서
		// 남은 속도 작용 시 계속 파고들게 됨)

		Vec2 vObjPos = _pOther->GetFinalPos();
		Vec2 vObjScale = _pOther->GetScale();

		//Vec2 vPrevObjPos = _pOther->GetPrevPos();

		Vec2 vPos = GetCollider()->GetFinalPos();
		
		Vec2 vScale = GetCollider()->GetScale();

		// 좌상단 기준 좌표를 중앙 기준으로 변환
		vPos.x += vScale.x / 2;
		vPos.y += vScale.y / 2;

		// 이전 프레임 플레이어 좌표 가져와서 벽의 어느 면에 닿은건지 비교
		WALL_DIR collisionDir = WALL_DIR::END;
		
		/*

		// 이전 프레임 플레이어 밑면 y가 Wall 윗면 y보다 높았다
		if (vPrevObjPos.y + (vObjScale.y / 2) < vPos.y - (vScale.y / 2))
			collisionDir = WALL_DIR::TOP;
		// 이전 프레임 플레이어 윗면이 벽의 바닥보다 컸다 (벽의 아랫면에 충돌) 
		else if (vPrevObjPos.y - (vObjScale.y / 2) > vPos.y + (vScale.y / 2))
			collisionDir = WALL_DIR::BOTTOM;
		// 이전 프레임 플레이어 오른쪽이 Wall 왼쪽보다 작았다 (벽의 왼편에 충돌)
		else if (vPrevObjPos.x + (vObjScale.x / 2) < vPos.x - (vScale.x / 2))
			collisionDir = WALL_DIR::LEFT;
		// 이전 프레임 플레이어 왼쪽이 벽의 오른쪽보다 컸다 (벽의 오른편에 충돌)
		else if (vPrevObjPos.x - (vObjScale.x / 2) > vPos.x + (vScale.x / 2))
			collisionDir = WALL_DIR::RIGHT;

			*/

		if (WALL_DIR::TOP == collisionDir) // 윗면과 충돌
		{
			// 파고든 길이 구하기
			// 정상적인 pos끼리의 거리
			float fLen = abs(vObjPos.y - vPos.y);
			// 겹침범위로 인해 짧아진 거리와 정상 거리의 차를 구함 (= 파고든 길이)
			float fValue = (vObjScale.y / 2.f + vScale.y / 2.f) - fLen;

			vObjPos = pOtherObj->GetPos();

			vObjPos.y -= fValue;
		}
		else if (WALL_DIR::BOTTOM == collisionDir) // 아랫면과 충돌
		{
			// 파고든 길이
			float fLen = abs(vObjPos.y - vPos.y);
			float fValue = (vObjScale.y / 2.f + vScale.y / 2.f) - fLen;

			vObjPos = pOtherObj->GetPos();

			vObjPos.y += fValue;
		}
		else if (WALL_DIR::LEFT == collisionDir) // 왼쪽 면과 충돌
		{
			// 파고든 길이
			float fLen = abs(vObjPos.x - vPos.x);
			float fValue = (vObjScale.x / 2.f + vScale.x / 2.f) - fLen;

			vObjPos = pOtherObj->GetPos();

			vObjPos.x -= fValue;
		}
		else if (WALL_DIR::RIGHT == collisionDir) // 오른쪽 면과 충돌
		{
			// 파고든 길이
			float fLen = abs(vObjPos.x - vPos.x);
			float fValue = (vObjScale.x / 2.f + vScale.x / 2.f) - fLen;

			vObjPos = pOtherObj->GetPos();

			vObjPos.x += fValue;
		}

		pOtherObj->SetPos(vObjPos);
	}
}
	
void CWall::OnCollision(CCollider* _pOther)
{
	// Enter뿐만 아니라 OnCollision에서도 동일한 처리 필요함 (발을 땅바닥에 딱 붙이고 있을 때는 enter가 아닌 OnCollision이기 때문에, 파고든 값을 보정해주면 enter에 걸리지 않아서
	// 남은 속도 작용 시 계속 파고들게 됨)

	CObject* pOtherObj = _pOther->GetObj();
	if (pOtherObj->GetName() == L"Player")
	{
		// Ground 상태임을 알려준다
		//pOtherObj->GetGravity()->SetGround(true);

		Vec2 vObjPos = _pOther->GetFinalPos();
		Vec2 vObjScale = _pOther->GetScale();

		Vec2 vPos = GetCollider()->GetFinalPos();
		Vec2 vScale = GetCollider()->GetScale();

		// 파고든 길이
		float fLen = abs(vObjPos.y - vPos.y);
		float fValue = (vObjScale.y / 2.f + vScale.y / 2.f) - fLen;

		vObjPos = pOtherObj->GetPos();

		vObjPos.y -= fValue;

		// 플레이어 좌표를 세팅
		pOtherObj->SetPos(vObjPos);
	}

	// 미사일과 벽 충돌 시
	if (pOtherObj->GetName() == L"Missile")
	{
		// 미사일 객체 삭제
	}
}

void CWall::OnCollisionExit(CCollider* _pOther)
{
	// 바닥과 충돌 상태가 아닐 때 그라운드 해제

	CObject* pOtherObj = _pOther->GetObj();
	if (pOtherObj->GetName() == L"Player")
	{
		//pOtherObj->GetGravity()->SetGround(false);
	}
}

void CWall::Save(FILE* _pFile)
{
	// 위치 작성
	Vec2 vPos = GetPos();
	fwrite(&vPos, sizeof(Vec2), 1, _pFile);

	// 스케일 작성
	Vec2 vScale = GetScale();
	fwrite(&vScale, sizeof(Vec2), 1, _pFile);

	// 텍스쳐 키 작성
	CTexture* pTex = GetTexture();
	wstring wTexKey = pTex->GetKey();
	string strName = string(wTexKey.begin(), wTexKey.end());
	fprintf(_pFile, strName.c_str());
	fprintf(_pFile, "\n");
	// 텍스쳐 상대경로 작성
	strName = string(pTex->GetRelativePath().begin(), pTex->GetRelativePath().end());
	fprintf(_pFile, strName.c_str());
	fprintf(_pFile, "\n");

	// 텍스쳐 인덱스 저장
	fwrite(&m_iImgIdx, sizeof(int), 1, _pFile);

	// 그룹 타입 저장
	fwrite(&m_eType, sizeof(WALL_TYPE), 1, _pFile);
}

void CWall::Load(FILE* _pFile)
{
	// 위치 멤버 채우기
	Vec2 vPos = {};
	fread(&vPos, sizeof(Vec2), 1, _pFile);
	SetPos(vPos);

	// 스케일 멤버 채우기
	Vec2 vScale = {};
	fread(&vScale, sizeof(Vec2), 1, _pFile);
	SetScale(vScale);

	// 텍스쳐 멤버 채우기
	// 텍스쳐 키 가져오기 / 텍스쳐 상대경로 가져오기
	char szBuff[256];
	string str;
	FScanf(szBuff, _pFile);
	str = szBuff;
	wstring strTexKey = wstring(str.begin(), str.end());


	FScanf(szBuff, _pFile);
	str = szBuff;
	wstring strTexPath = wstring(str.begin(), str.end());
	// 키와 경로 기반으로 텍스처 로드
	CTexture* pTex = CResMgr::GetInst()->LoadTexture(strTexKey, strTexPath);
	SetTexture(pTex);


	// 텍스쳐 인덱스 멤버 채우기
	fread(&m_iImgIdx, sizeof(int), 1, _pFile);

	GROUP_TYPE eType;
	fread(&eType, sizeof(GROUP_TYPE), 1, _pFile);

	CScene* pScene = CSceneMgr::GetInst()->GetCurScene();
	int iType = (int)eType + 2;
	pScene->AddObject(this, (GROUP_TYPE)iType);
}
