#include "pch.h"
#include "func.h"

#include "CEventMgr.h"

void CreateObject(CObject* _pObj, GROUP_TYPE _eGroup)
{
	tEvent evn = {};
	evn.eEven = EVENT_TYPE::CREATE_OBJECT;
	evn.lParam = (DWORD_PTR)_pObj;
	evn.wParam = (DWORD_PTR)_eGroup;

	CEventMgr::GetInst()->AddEvent(evn);
}

void DeleteObject(CObject* _pObj)
{
	tEvent evn = {};
	evn.eEven = EVENT_TYPE::DELETE_OBJECT;
	evn.lParam = (DWORD_PTR)_pObj;

	CEventMgr::GetInst()->AddEvent(evn);
}

void ChangeScene(SCENE_TYPE _eNext)
{
	tEvent evn = {};
	evn.eEven = EVENT_TYPE::SCENE_CHANGE;
	evn.lParam = (DWORD_PTR)_eNext;

	CEventMgr::GetInst()->AddEvent(evn);
}

void ChangeAIState(AI* _pAI, MON_STATE _eNextState)
{
	tEvent evn = {};
	evn.eEven = EVENT_TYPE::CHANGE_AI_STATE;
	evn.lParam = (DWORD_PTR)(_pAI);
	evn.wParam = (DWORD_PTR)_eNextState;

	CEventMgr::GetInst()->AddEvent(evn);
}
void UIBTNDown(CScene* _pScene)
{
	tEvent evn = {};
	evn.eEven = EVENT_TYPE::UI_BTN_DOWN;
	evn.lParam = (DWORD_PTR)(_pScene);

	CEventMgr::GetInst()->AddEvent(evn);
}

void UIBTNUp(CScene* _pScene)
{
	tEvent evn = {};
	evn.eEven = EVENT_TYPE::UI_BTN_UP;
	evn.lParam = (DWORD_PTR)(_pScene);

	CEventMgr::GetInst()->AddEvent(evn);
}

void FScanf(char* _pOutBuff, FILE* _pFile)
{
	int i = 0;
	while (true)
	{
		// ���Ϸκ��� ���� �ϳ��ϳ� �д� �Լ�
		char c = (char)getc(_pFile);

		// ���ۿ� �ֱ� �� ���๮�� ���� �˻�
		if (c == '\n')
		{
			// ���ڿ��� ���� ǥ���ϱ� ���� ���� �� ���鹮�� ����
			_pOutBuff[i++] = '\0';
			break;
		}
		_pOutBuff[i++] = c;
	}
}

void SaveWstring(const wstring& _str, FILE* _pFile)
{
	// ������ ����ȭ
	// wstring�� �����ϴ� ���ڿ��� �ּ�
	const wchar_t* pStrName = _str.c_str();
	// �� ���ڿ��� ����
	size_t iLen = _str.length();

	// ���Ͽ� ���� ���� ���� (���� ���� �� �� ���� �������� ���� ��)
	fwrite(&iLen, sizeof(size_t), 1, _pFile);

	// wchar_t ũ���� ���ڸ� ���ڱ��� ������ŭ ���Ͽ� �ۼ� (���ڿ� ����)
	fwrite(pStrName, sizeof(wchar_t), iLen, _pFile);
}

void LoadWstring(wstring& _str, FILE* _pFile)
{
	// �о�� �� ���ڿ��� ���̸� �����ϴ� ������ �����Ѵ�.
	size_t iLen = 0;
	fread(&iLen, sizeof(size_t), 1, _pFile);

	// �ִϸ��̼��� �̸��� ������ ���ڿ� ���� �����
	wchar_t szBuff[256] = {};

	// wchar_t ũ���� ���ڸ� ���ڱ��� ������ŭ �о�鿩 ���ۿ� ����
	fread(szBuff, sizeof(wchar_t), iLen, _pFile);

	_str = szBuff;

	// wstring ��ü�� �ٷ� �����ϱ⿡��, wstring ����� ���� �󸶳� �Ҵ�Ǿ� �ִ��� �� �� ������
	// �迭 ���۸� �غ��ؼ�, �װ��� �����͸� ������ ����, wstring ��ü�� ���Կ����ڷ� �־���
	// fread���ٴ� ���Կ����ڷ� �־��־�� �����Ҵ� ����, wstring�� ������ �����ε����� ������� �ִ� ��ɵ���
	// ����� ������� �� ����
}
