#include "pch.h"
#include "CScene.h"
#include "CObject.h"
#include "CTile.h"
#include "CResMgr.h"
#include "CPathMgr.h"




CScene::CScene()
	: m_iTileX(0)
	, m_iTileY(0)
{
}

// 벡터: 씬의 멤버이므로, 씬이 소멸할 때 자동으로 사라짐
// 객체: 벡터에는 동적할당 한 객체들의 "주소"를 넣은거라 위와 같이 수동 삭제 해주어야 함
// 객체그자체를 넣었으면 ㄱㅊ았을텐데. 주소를 저장한거니까, 직접가서 조져야만...
// 벡터 안에 열매가 맺히듯 주렁주렁한데 가지만 없앤다고 열매가 사라지지 않음!
CScene::~CScene()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); ++j)
		{
			// m_arrObj[i] 그룹 벡터의 j 물체 삭제
			delete m_arrObj[i][j];
		}
	}
}

void CScene::CreateTile(UINT _iXCount, UINT _iYCount)
{
	DeleteGroup(GROUP_TYPE::TILE);

	m_iTileX = _iXCount;
	m_iTileY = _iYCount;


	CTexture* pTileTex = CResMgr::GetInst()->LoadTexture(L"Tile", L"texture\\tile\\tile.bmp");

	// 타일 생성
	for (UINT i = 0; i < _iYCount; ++i)
	{
		for (UINT j = 0; j < _iXCount; ++j)
		{
			CTile* pTile = new CTile();

			pTile->SetTexture(pTileTex);
			pTile->SetPos(Vec2((float)(TILE_SIZE * j), (float)(i * TILE_SIZE)));
			AddObject(pTile, GROUP_TYPE::TILE);
		}
	}
}



void CScene::update()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); ++j)
		{
			if (!m_arrObj[i][j]->IsDead())
			{
				m_arrObj[i][j]->update();
			}
		}
	}
}

void CScene::finalUpdate()
{ // 작업을 마무리쳐주는 (확정지어주는) 업데이트
	// 충돌체가 오브젝트를 따라가게 한다
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); ++j)
		{
			// Dead 객체라도
			// 층돌체의 경우, 마무리 작업 필요하기 때문에 final update까지는 돌려준다
			m_arrObj[i][j]->finalUpdate();
		}
	}
}

void CScene::render(HDC _dc)
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		// earse 함수 쓰기 위해 operator(i, j...) 변수 쓰지 않고
		// iterator 쓴다

		vector<CObject*>::iterator iter = m_arrObj[i].begin();

		for (; iter != m_arrObj[i].end();)
		{
			if (!(*iter)->IsDead())
			{
				(*iter)->render(_dc);
				++iter;

			}
			else
			{
				iter = m_arrObj[i].erase(iter); //벡터에서 데이터 삭제 후엔 그 다음 이터 받는 것 기억
			}
		}
	}
}

void CScene::DeleteGroup(GROUP_TYPE _eTarget)
{
	Safe_Delete_Vec<CObject*>(m_arrObj[(UINT)_eTarget]);
}

void CScene::DeleteAll()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		DeleteGroup((GROUP_TYPE)i);
	}
}

// 상대경로 사용하는 이유
// 툴에서는 특정 창 열어서 불러왔지만,
// 씬 enter()에서 절대경로로 파일 불러오는 방식은 좋지 않을 것
void CScene::LoadTile(const wstring& _strRelativePath)
{
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	// 커널 오브젝트
	FILE* pFile = nullptr;

	_wfopen_s(&pFile, strFilePath.c_str(), L"rb");

	assert(pFile);

	// 타일 가로세로 개수 불러오기
	UINT xCount = 0;
	UINT yCount = 0;

	fread(&xCount, sizeof(UINT), 1, pFile);
	fread(&yCount, sizeof(UINT), 1, pFile);

	// 불러온 개수에 맞게 EmptyTile들 만들어두기
	CreateTile(xCount, yCount);

	// 만들어진 파일 개별로 필요한 정보 불러오게 함
	const vector<CObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);

	for (size_t i = 0; i < vecTile.size(); ++i)
	{
		((CTile*)vecTile[i])->Load(pFile);
	}

	// 이 파일에 대한 파일 입출력 닫기
	fclose(pFile);

}