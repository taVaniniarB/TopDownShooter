#pragma once

#include "global.h"

class CAnimator;
class CTexture;

struct tAnimFrm
{
	Vec2 vLT; // 좌상단
	Vec2 vSlice; // 자를 사이즈 (프레임마다 사이즈가 다를 수 있으니까)
	Vec2 vOffset;
	float fDuration; // 프레임에서 머무는 시간
};

class CAnimation
{
private:
	wstring				m_strName;
	CAnimator*			m_pAnimator;
	CTexture*			m_pTex; // Animation이 사용하는 텍스쳐
	vector<tAnimFrm>	m_vecFrm; // 모든 프레임 정보
	int					m_iCurFrm; // 현재 프레임
	float				m_fAccTime; // 시간누적

	bool				m_bFinish; // 재생 끝 도달 여부

public:
	const wstring& GetName() { return m_strName; }
	bool IsFinish() { return m_bFinish; }
	void SetFrame(int _iFrameIdx)
	{
		m_bFinish = false;
		m_iCurFrm = _iFrameIdx;
		m_fAccTime = 0.f;
	}

	// 받아간 쪽에서 수정 가능하게 레퍼런스
	tAnimFrm& GetFrame(int iIdx) { return m_vecFrm[iIdx]; }
	UINT GetMaxFrame() { return (UINT)m_vecFrm.size(); }

private:
	// 한 번 정해진 이름이 나중에라도 바뀌면 안 되니까 private
	void SetName(const wstring& _strName) { m_strName = _strName; }

public:
	void update();
	void render(HDC _dc);
	void Create(CTexture* _pTex, Vec2 _vLT, Vec2 _vSliceSize, Vec2 _vStep, float _fDuration, UINT _iFrameCount);

public:
	// 애니메이션을 파일로 저장
	void Save(const wstring& _strRelativePath);
	void Load(const wstring& _strRelativePath);

public:
	CAnimation();
	~CAnimation();

	friend class CAnimator;
};

