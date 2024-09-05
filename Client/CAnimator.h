#pragma once

#include "global.h"

class CObject;
class CAnimation;
class CTexture;

class CAnimator
{
private:
	map<wstring, CAnimation*>	m_mapAnim; // ��� Animation �����̳�. �ִϸ��̼� ���� ã�� ����, ��Ī�� Ưȭ�� map���� �ִϸ��̼��� ����
	CAnimation*					m_pCurAnim; // ���� ��� ���� Animation
	CObject*					m_pOwner;  // Animator ���� ������Ʈ
	bool						m_bRepeat;

public:
	CObject* GetObj() { return m_pOwner; }

public:
	void CreateAnimation(const wstring& _strName, CTexture* _pTex, Vec2 _vLT, Vec2 _vSliceSize, Vec2 _vStep, float _fDuration, UINT _iFrameCount); // Ư�� �ִϸ��̼� ����
	void LoadAnimation(const wstring& _strRelativePath);
	CAnimation* FindAnimation(const wstring& _strName);	 // Ư�� �ִϸ��̼� ã�ƿ���
	void Play(const wstring& _strName, bool _bRepeat);	 // Ư�� �ִϸ��̼� ���

	void update();
	void finalUpdate();
	void render(HDC _dc);

public:
	CAnimator();
	~CAnimator();
	
	friend class CObject;
};