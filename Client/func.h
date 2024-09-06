#pragma once
// ���� ���̴� ���� �Լ��� ��Ƶδ� ���

class CObject;
class AI;

// ������Ʈ ���� �̺�Ʈ ����
// ����: ������Ʈ, �׷�
void CreateObject(CObject* _pObj, GROUP_TYPE e_Group);
void DeleteObject(CObject* _pObj);
void ChangeScene(SCENE_TYPE _eNext);
void ChangeAIState(AI* _pAI, MON_STATE _eNextState);

// ���� �ȿ� �����͸� �־��� ��
// �̰� ���� �� �ܼ��� ���͸� Ŭ������ �� �ƴ϶�
// �� ���� �����Ͱ� ����Ű�� ��ü���� ������� ��
// �̷� ��Ȳ ���� ���� ���̹Ƿ�, �Լ����ø����� ���������
// ��.���� �Լ��� �ٸ�. �� ��ü�� �����ϴ� �ڵ尡 �ƴ�. ��û�� Ÿ���� �־�� �� �������� �����Ǵ� ��
// ���ø��̹Ƿ� ����� ����
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

// ���๮�ڰ� ���� ������ ���ۿ� ���� �ϳ��ϳ��� �����ϴ� �Լ�
void FScanf(char* _pOutBuff, FILE* _pFile);

// wstring ��ü�� ����ȭ�Ͽ� ���Ͽ� �����ϴ� �Լ�
void SaveWstring(const wstring& _str, FILE* _pFile);
// ���Ϸκ��� ���ڿ��� �о _str�� �����ϴ� �Լ�
void LoadWstring(wstring& _str, FILE* _pFile);