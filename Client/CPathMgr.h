#pragma once
class CPathMgr
{
	SINGLE(CPathMgr)
private:
	wchar_t		m_szContentPath[255]; // content 경로 저장할 변수(윈도우 상 최대 경로 문자 수가 255자라 여기서도 그렇게 제한함)
public:
	void init();
	const wchar_t* GetContentPath() { return m_szContentPath; }
};

