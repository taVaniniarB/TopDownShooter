#pragma once
// 자주 쓰이는 전역 함수를 모아두는 헤더

class CObject;

// 오브젝트 생성 이벤트 생성
// 인자: 오브젝트, 그룹
void CreateObject(CObject* _pObj, GROUP_TYPE e_Group);
void DeleteObject(CObject* _pObj);
void ChangeScene(SCENE_TYPE _eNext);

// 벡터 안에 포인터를 넣었을 시
// 이걸 지울 때 단순히 벡터만 클리어할 게 아니라
// 그 안의 포인터가 가리키는 객체까지 지워줘야 함
// 이런 상황 자주 있을 것이므로, 함수템플릿으로 만들어주자
// 함.템은 함수와 다름. 그 자체로 존재하는 코드가 아님. 요청된 타입이 있어야 그 버전으로 생성되는 것
// 템플릿이므로 헤더에 구현
template <typename T>
void Safe_Delete_Vec(vector<T>& _vec)
{
	for (size_t i = 0; i < _vec.size(); ++i)
	{
		if (nullptr != _vec[i])
			delete _vec[i];
	}

	_vec.clear();
}

template <typename T1, typename T2>
void Safe_Delete_Map(map<T1, T2>& _map)
{
	typename map<T1, T2>::iterator iter = _map.begin();
	for (; iter != _map.end(); ++iter)
	{
		if (nullptr != iter->second)
			delete iter->second;
	}

	_map.clear();
}