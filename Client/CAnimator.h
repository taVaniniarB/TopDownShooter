#pragma once

#include "global.h"

class CObject;
class CAnimation;
class CTexture;

class CAnimator
{
private:
	map<wstring, CAnimation*>	m_mapAnim; // 모든 Animation 컨테이너. 애니메이션 빨리 찾기 위해, 서칭에 특화된 map으로 애니메이션을 저장
	CAnimation*					m_pCurAnim; // 현재 재생 중인 Animation
	CObject*					m_pOwner;  // Animator 소유 오브젝트
	bool						m_bRepeat;

public:
	CObject* GetObj() { return m_pOwner; }

public:
	void CreateAnimation(const wstring& _strName, CTexture* _pTex, Vec2 _vLT, Vec2 _vSliceSize, Vec2 _vStep, float _fDuration, UINT _iFrameCount); // 특정 애니메이션 생성
	CAnimation* FindAnimation(const wstring& _strName);	 // 특정 애니메이션 찾아오기
	void Play(const wstring& _strName, bool _bRepeat);	 // 특정 애니메이션 재생

	void update();
	void render(HDC _dc);

public:
	CAnimator();
	~CAnimator();
	
	friend class CObject;
};