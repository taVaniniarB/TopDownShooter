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

	
	// Animation�� �̸��� �����Ѵ�
	fprintf(pFile, "[Animation Name]\n");
	// 1����Ʈ ���ڿ��� ��ȯ
	string strName = string(m_strName.begin(), m_strName.end());
	fprintf(pFile, strName.c_str());
	fprintf(pFile, "\n");

	// �ؽ�ó
	// �������ε�... �ּҸ� ���� �����ؼ� �� ��
	fprintf(pFile, "[Texture Name]\n");
	// 1����Ʈ ���ڿ��� ��ȯ
	strName = string(m_pTex->GetKey().begin(), m_pTex->GetKey().end());
	fprintf(pFile, strName.c_str());
	fprintf(pFile, "\n");

	fprintf(pFile, "[Texture Path]\n");
	strName = string(m_pTex->GetRelativePath().begin(), m_pTex->GetRelativePath().end());
	fprintf(pFile, strName.c_str());
	fprintf(pFile, "\n");


	// ������ ����
	fprintf(pFile, "[Frame Count]\n");
	fprintf(pFile, "%d\n", (int)m_vecFrm.size());
	
	// �� �������� ������ ��� ����ü ���� m_vecFrm�� ����

	for (size_t i = 0; i < m_vecFrm.size(); ++i)
	{
		fprintf(pFile, "\n\n");

		fprintf(pFile, "[Frame Index]\n");
		fprintf(pFile, "%d\n", (int)i);

		fprintf(pFile, "[Left Top Position]\n");
		fprintf(pFile, "%d %d\n", (int)m_vecFrm[i].vLT.x, (int)m_vecFrm[i].vLT.y);

		fprintf(pFile, "[Slice Size]\n");
		fprintf(pFile, "%d %d\n", (int)m_vecFrm[i].vSlice.x, (int)m_vecFrm[i].vSlice.y);

		fprintf(pFile, "[Offset]\n");
		fprintf(pFile, "%d %d\n", (int)m_vecFrm[i].vOffset.x, (int)m_vecFrm[i].vOffset.y);

		fprintf(pFile, "[Duration]\n");
		fprintf(pFile, "%f\n", m_vecFrm[i].fDuration);
	}

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

	// Animation�� �̸��� �о�´�
	// fscanf(%s)�� ���ڿ� �б��� ������: ���� ������ �Է��� ����
	// ���๮�ڰ� ���� ������ ���ۿ� ���� �ϳ��ϳ��� �����ϴ� �Լ��� �������.
	// �������� \0���� ���Ƶ� ���̹Ƿ�, �������� ���۴� ��� �����ϵ��� ����.
	char szBuff[256];
	// ��ȯ�� string��ü
	string str;

	FScanf(szBuff, pFile); // ���⼭�� [Animation Name]�̶� �ʵ� �̸��� ���� ����
	FScanf(szBuff, pFile); // �о �����ؾ� �ϴ� ������ ������
	str = szBuff;

	m_strName = wstring(str.begin(), str.end());



	//�����ϴ� �ؽ�ó �̸� �� ���

	FScanf(szBuff, pFile); // [Texture Name]
	FScanf(szBuff, pFile);
	str = szBuff;
	wstring strTexKey = wstring(str.begin(), str.end());


	FScanf(szBuff, pFile); // [Texture Path]
	FScanf(szBuff, pFile);
	str = szBuff;
	wstring strTexPath = wstring(str.begin(), str.end());


	// Ű�� ��� ������� �ؽ�ó �ε�
	m_pTex = CResMgr::GetInst()->LoadTexture(strTexKey, strTexPath);



	// ������ ����
	FScanf(szBuff, pFile); // ����

	int iFrameCount = 0;
	// �����̱� ������ fscanf_s(%d) �̿��ؼ� ���� �� �ִ�
	fscanf_s(pFile, "%d", &iFrameCount);



	// �� �������� ������ ��� ����ü ���� m_vecFrm�� ����
	tAnimFrm frm = {};
	for (int i = 0; i < iFrameCount; ++i)
	{
		POINT pt = {}; // ���� 2���� ������ ����ü

		while (true)
		{
			FScanf(szBuff, pFile);

			// �� ���ڿ��� ������ ���ϴ� �Լ�
			// ������ 0 ��ȯ�ϱ� ������ ���� ������
			if (!strcmp("[Frame Index]", szBuff))
			{
				fscanf_s(pFile, "%d", &pt.x);
			}
			else if (!strcmp("[Left Top Position]", szBuff))
			{
				// ������ %d��, ������ �ش��ϴ� ���� ���� ������ ��� �о����(�� ���鹫��)
				fscanf_s(pFile, "%d", &pt.x);
				fscanf_s(pFile, "%d", &pt.y);

				frm.vLT = pt;
			}
			else if (!strcmp("[Slice Size]", szBuff))
			{
				fscanf_s(pFile, "%d", &pt.x);
				fscanf_s(pFile, "%d", &pt.y);

				frm.vSlice = pt;
			}
			else if (!strcmp("[Offset]", szBuff))
			{
				fscanf_s(pFile, "%d", &pt.x);
				fscanf_s(pFile, "%d", &pt.y);

				frm.vOffset = pt;
			}
			else if (!strcmp("[Duration]", szBuff))
			{
				fscanf_s(pFile, "%f", &frm.fDuration);
				break;
			}
		}
		
		m_vecFrm.push_back(frm);
	}
	
	fclose(pFile);
}
