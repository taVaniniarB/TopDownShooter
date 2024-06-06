#include "pch.h"
#include "CPathMgr.h"

#include "CCore.h"

CPathMgr::CPathMgr()
	: m_szContentPath{}
{
}

CPathMgr::~CPathMgr()
{}

// 패스매니저가 초기화될 때 content 폴더를 찾아낼 것이다!
void CPathMgr::init()
{
	// 버퍼 길이, 값 넣을 주소
	GetCurrentDirectory(255, m_szContentPath);
	// 실행 결과 : 현재 실행중인 위치 경로가 변수에 채워짐

	// 상위폴더로
	// + bin \ content 하면, 어느 상황에서든 컨텐츠 폴더에 접근할 수 있겠다

	int iLen = (int)wcslen(m_szContentPath);

	for (int i = iLen - 1; 0 <= i; --i)
	{
		if ('\\' == m_szContentPath[i])
		{
			m_szContentPath[i] = '\0';
			break;
		}
	}

	wcscat_s(m_szContentPath, 255, L"\\bin\\content\\");

	//SetWindowText(CCore::GetInst()->GetMainHwnd(), m_szContentPath);
}