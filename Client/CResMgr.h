#pragma once

class CRes;
class CTexture;
class CSound;

class CResMgr
{
	SINGLE(CResMgr);
private:
	map<wstring, CRes*> m_mapTex; // 찾을 때 문자열로 찾도록string, 실질적 데이터 타입은 리소스 포인터
	map<wstring, CRes*>		m_mapSound;
	map<wstring, Image*>	m_mapImage;

public:
	CTexture* LoadTexture(const wstring& _strKey, const wstring& _strRelativePath);
	CTexture* CreateTexture(const wstring& _strKey, UINT _iwidth, UINT _iheight);
	CTexture* FindTexture(const wstring& _strKey);

	Image* LoadGdiImage(const wstring& _strKey, const wstring& _strRelativePath);
	Image* FindImage(const wstring& _strKey);

	CSound* LoadSound(const wstring& _strKey, const wstring& _strRelativePath);
	CSound* FindSound(const wstring& _strKey);
};

