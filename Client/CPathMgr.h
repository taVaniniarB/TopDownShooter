#pragma once
class CPathMgr
{
	SINGLE(CPathMgr)
private:
	wchar_t		m_szContentPath[256]; // content 경로 저장할 변수(윈도우 상 최대 경로 문자 수가 255자라 여기서도 그렇게 제한함)
	wchar_t		m_szRelativePath[256];
	
public:
	void init();
	const wchar_t* GetContentPath() { return m_szContentPath; }

	// 전체 파일 경로가 들어오면, 절대 경로 파트를 잘라서 상대 경로 파트만 되돌려줌
	wstring GetRelativePath(const wchar_t* _filepath);
};

