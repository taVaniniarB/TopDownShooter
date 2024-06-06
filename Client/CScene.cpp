#include "pch.h"
#include "CScene.h"
#include "CObject.h"




CScene::CScene()
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

void CScene::DeleteAllException(GROUP_TYPE _eLType, GROUP_TYPE _eRType)
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		if ((UINT)_eLType != i && (UINT)_eRType != i)
			DeleteGroup((GROUP_TYPE)i);
	}
}