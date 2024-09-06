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
		// 파일로부터 문자 하나하나 읽는 함수
		char c = (char)getc(_pFile);

		// 버퍼에 넣기 전 개행문자 여부 검사
		if (c == '\n')
		{
			// 문자열의 끝을 표시하기 위해 종료 전 공백문자 삽입
			_pOutBuff[i++] = '\0';
			break;
		}
		_pOutBuff[i++] = c;
	}
}

void SaveWstring(const wstring& _str, FILE* _pFile)
{
	// 데이터 직렬화
	// wstring이 관리하는 문자열의 주소
	const wchar_t* pStrName = _str.c_str();
	// 그 문자열의 길이
	size_t iLen = _str.length();

	// 파일에 문자 길이 저장 (파일 읽을 때 이 값을 기준으로 읽을 것)
	fwrite(&iLen, sizeof(size_t), 1, _pFile);

	// wchar_t 크기의 문자를 문자길이 개수만큼 파일에 작성 (문자열 저장)
	fwrite(pStrName, sizeof(wchar_t), iLen, _pFile);
}

void LoadWstring(wstring& _str, FILE* _pFile)
{
	// 읽어야 할 문자열의 길이를 저장하는 정수를 저장한다.
	size_t iLen = 0;
	fread(&iLen, sizeof(size_t), 1, _pFile);

	// 애니메이션의 이름을 저장할 문자열 버퍼 만들기
	wchar_t szBuff[256] = {};

	// wchar_t 크기의 문자를 문자길이 개수만큼 읽어들여 버퍼에 저장
	fread(szBuff, sizeof(wchar_t), iLen, _pFile);

	_str = szBuff;

	// wstring 객체에 바로 저장하기에는, wstring 사이즈가 현재 얼마나 할당되어 있는지 알 수 없으니
	// 배열 버퍼를 준비해서, 그곳에 데이터를 저장한 다음, wstring 객체에 대입연산자로 넣어줌
	// fread보다는 대입연산자로 넣어주어야 동적할당 같은, wstring의 연산자 오버로딩으로 만들어져 있는 기능들을
	// 제대로 보장받을 수 있음
}
