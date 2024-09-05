#include "pch.h"
#include "CAnimation.h"

#include "CTimeMgr.h"

#include "CAnimator.h"
#include "CTexture.h"
#include "CObject.h"
#include "CCamera.h"

#include "CResMgr.h"
#include "CPathMgr.h"


CAnimation::CAnimation()
	: m_pAnimator(nullptr)
	, m_pTex(nullptr)
	, m_iCurFrm(0)
	, m_fAccTime(0.f)
	, m_bFinish(false)
{
}

CAnimation::~CAnimation()
{
}

void CAnimation::update()
{
	if (m_bFinish)
		return;

	m_fAccTime += fDT;

	if (m_fAccTime > m_vecFrm[m_iCurFrm].fDuration)
	{
		++m_iCurFrm;

		
		if (m_vecFrm.size() <= m_iCurFrm) // ������ ������ ����
		{
			m_iCurFrm = -1; // �����ٴ� ������ -1
			m_bFinish = true;
			m_fAccTime = 0.f;
			return;
		}

		// �����ð� �ʱ�ȭ
		// dt�� �����ϴٺ��� ���� �ð��� �ʰ��� ���� ����
		// �� �ð��� �����ϱ� ���� 0���� �ʱ�ȭ���� �ʰ� �����ð� - 1f�� �ð����� �ʱ�ȭ
		// �ణ.. ���� �ð� �� �ִϸ��̼� ����Ƚ�� �����ϰ� �����ϵ��� �Ѵٴ� ����
		// �ε�, �� �ٸ� ������: �Ѽ��� ���� �ɸ��ٸ� dt�� ��Ŀ����, �� Ǯ���� ���� ��������
		// �� ���α��!
		m_fAccTime = m_fAccTime - m_vecFrm[m_iCurFrm].fDuration;

	}
}

void CAnimation::render(HDC _dc)
{
	if (m_bFinish)
		return;

	CObject* pObj = m_pAnimator->GetObj();
	Vec2 vPos = pObj->GetPos();

	vPos += m_vecFrm[m_iCurFrm].vOffset;

	// �������� ��ǥ�� ��ȯ
	vPos = CCamera::GetInst()->GetRenderPos(vPos);

	// dc, ��µ� ��ġ(������Ʈ�� ��µ� �»�� ��ġ), ���μ��α���, �ؽ�ó�� DC, �ؽ�ó ������ �ڸ� ��ġ, ���μ��α���
	TransparentBlt(_dc
		, (int)(vPos.x - m_vecFrm[m_iCurFrm].vSlice.x / 2.f)
		, (int)(vPos.y - m_vecFrm[m_iCurFrm].vSlice.y / 2.f)
		, (int)(m_vecFrm[m_iCurFrm].vSlice.x)
		, (int)(m_vecFrm[m_iCurFrm].vSlice.y)
		, m_pTex->GetDC()					
		, (int)(m_vecFrm[m_iCurFrm].vLT.x)
		, (int)(m_vecFrm[m_iCurFrm].vLT.y)
		, (int)(m_vecFrm[m_iCurFrm].vSlice.x)
		, (int)(m_vecFrm[m_iCurFrm].vSlice.y)
		, RGB(0, 255, 0));
}

void CAnimation::Create(CTexture* _pTex, Vec2 _vLT, Vec2 _vSliceSize, Vec2 _vStep, float _fDuration, UINT _iFrameCount)
{
	m_pTex = _pTex;
	tAnimFrm frm = {};
	for (UINT i = 0; i < _iFrameCount; ++i)
	{
		frm.fDuration = _fDuration;
		frm.vSlice = _vSliceSize;
		frm.vLT = _vLT + _vStep * (float)i; // ���Ϳ� int�� ���� �����ε� (����-int ���� �����ֱ�)

		m_vecFrm.push_back(frm);
	}
}


void CAnimation::Save(const wstring& _strRelativePath)
{
	// ������ �����
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	// ���� ��Ʈ�� ����
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"wb");
	assert(pFile);

	
	// Animation�� �̸��� �����Ѵ�.
	SaveWstring(m_strName, pFile);


	// �ؽ�ó
	// �������ε�... �ּҸ� ���� �����ؼ� �� ��
	SaveWstring(m_pTex->GetKey(), pFile);
	SaveWstring(m_pTex->GetRelativePath(), pFile);


	// �� �������� ������ ��� ����ü ���� m_vecFrm�� �����Ѵ�.
	// ���� ���� ������ �����̹Ƿ�, ���� ���� ���� ����Ͽ� ������ ���� ���� ����
	size_t iFrameCount = m_vecFrm.size();
	// ������ ����ü ���� ����
	fwrite(&iFrameCount, sizeof(size_t), 1, pFile);
	
	
	// ���Ͱ� �����ϰ� �ִ� �����ּ�(data()), ����ü ������, ������ ������ŭ ����
	fwrite(m_vecFrm.data(), sizeof(tAnimFrm), iFrameCount, pFile);


	// ���� ��Ʈ�� �ݱ�
	fclose(pFile);
}

void CAnimation::Load(const wstring& _strRelativePath)
{
	// ������ �����
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	// ���� ��Ʈ�� ���� (�б� ���)
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"rb");
	assert(pFile);

	// ���Ϸκ��� �ε��� �ִϸ��̼� �̸� �����͸� ����� ä���
	LoadWstring(m_strName, pFile);


	// �ؽ�ó�� Ű �б�
	wstring strTexKey, strTexPath;
	LoadWstring(strTexKey, pFile);
	LoadWstring(strTexPath, pFile);
	// ���� Ű�� ���ҽ��Ŵ����κ��� �ؽ�ó�� �޾ƿ�
	m_pTex = CResMgr::GetInst()->LoadTexture(strTexKey, strTexPath);

	// ������ ���� ����
	size_t iFrameCount = 0;
	fread(&iFrameCount, sizeof(size_t), 1, pFile);
	
	// ��� ������ ���� (����ü ����) ����
	// write ���� �ٸ��� �ϳ��ϳ� push back ���־�� ��
	// Ȥ�� ����.resizse �Լ��� iFrameCount���� ���� �̸� Ȯ���صΰ� �����ϸ�
	// �� ���� ��� ������ �о�鿩�� ä��� ����
	m_vecFrm.resize(iFrameCount);
	fread(m_vecFrm.data(), sizeof(tAnimFrm), iFrameCount, pFile);
	// �ݺ����� ���� ������: push back�� ���� ���ڶ� ������ ���� ���Ҵ� �ϴ� ���� �ִµ�
	// �� ���� ������ Ȯ�������ν� ���Ҵ� ��� ����
	
	fclose(pFile);
}
