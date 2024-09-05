#pragma once

#include "global.h"

class CAnimator;
class CTexture;

struct tAnimFrm
{
	Vec2 vLT; // �»��
	Vec2 vSlice; // �ڸ� ������ (�����Ӹ��� ����� �ٸ� �� �����ϱ�)
	Vec2 vOffset;
	float fDuration; // �����ӿ��� �ӹ��� �ð�
};

class CAnimation
{
private:
	wstring				m_strName;
	CAnimator*			m_pAnimator;
	CTexture*			m_pTex; // Animation�� ����ϴ� �ؽ���
	vector<tAnimFrm>	m_vecFrm; // ��� ������ ����
	int					m_iCurFrm; // ���� ������
	float				m_fAccTime; // �ð�����

	bool				m_bFinish; // ��� �� ���� ����

public:
	const wstring& GetName() { return m_strName; }
	bool IsFinish() { return m_bFinish; }
	void SetFrame(int _iFrameIdx)
	{
		m_bFinish = false;
		m_iCurFrm = _iFrameIdx;
		m_fAccTime = 0.f;
	}

	// �޾ư� �ʿ��� ���� �����ϰ� ���۷���
	tAnimFrm& GetFrame(int iIdx) { return m_vecFrm[iIdx]; }
	UINT GetMaxFrame() { return (UINT)m_vecFrm.size(); }

private:
	// �� �� ������ �̸��� ���߿��� �ٲ�� �� �Ǵϱ� private
	void SetName(const wstring& _strName) { m_strName = _strName; }

public:
	void update();
	void render(HDC _dc);
	void Create(CTexture* _pTex, Vec2 _vLT, Vec2 _vSliceSize, Vec2 _vStep, float _fDuration, UINT _iFrameCount);

public:
	// �ִϸ��̼��� ���Ϸ� ����
	void Save(const wstring& _strRelativePath);
	void Load(const wstring& _strRelativePath);

public:
	CAnimation();
	~CAnimation();

	friend class CAnimator;
};

