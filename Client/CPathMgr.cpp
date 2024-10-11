#include "pch.h"
#include "CPathMgr.h"

#include "CCore.h"

CPathMgr::CPathMgr()
	: m_szContentPath{}
	, m_szRelativePath{}
{
}

CPathMgr::~CPathMgr()
{}

// �н��Ŵ����� �ʱ�ȭ�� �� content ������ ã�Ƴ� ���̴�!
void CPathMgr::init()
{
	// ���� ����, �� ���� �ּ�
	GetCurrentDirectory(255, m_szContentPath);
	// ���� ��� : ���� �������� ��ġ ��ΰ� ������ ä����

	// ����������
	// + bin \ content �ϸ�, ��� ��Ȳ������ ������ ������ ������ �� �ְڴ�

	int iLen = (int)wcslen(m_szContentPath);

	for (int i = iLen - 1; 0 <= i; --i)
	{
		if ('\\' == m_szContentPath[i])
		{
			m_szContentPath[i] = '\0';
			break;
		}
	}

	wcscat_s(m_szContentPath, 255, L"\\Output\\bin\\content\\");

	//SetWindowText(CCore::GetInst()->GetMainHwnd(), m_szContentPath);
}

wstring CPathMgr::GetRelativePath(const wchar_t* _filepath)
{
	_filepath;

	// string �Լ��� �̿�����
	wstring strFilePath = _filepath;

	// Ư�� �������κ��� ���ڿ� �߶󳻱� (�߶󳽰� ��ȯ)
	size_t iAbsLen = wcslen(m_szContentPath);
	size_t iFullLen = strFilePath.length();

	// ���� �ε���, �߶� ���� ����
	wstring strRelativePath = strFilePath.substr(iAbsLen, iFullLen - iAbsLen);

	return strRelativePath;
}
