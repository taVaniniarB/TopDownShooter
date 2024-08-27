#include "global.h"
#include "CResMgr.h"

#include "CPathMgr.h"
#include "CTexture.h"

CResMgr::CResMgr()
{
}

CResMgr::~CResMgr()
{
	Safe_Delete_Map(m_mapTex);
	// �����Լ��� ���� priviate�� �ڽ�(�ؽ���)�� �Ҹ��ڸ� ȣ���ϴ¹�
	// ���� ������ Ÿ���� �θ�(���ҽ�)�� �ؼ�, ����� �Լ��� ȣ���Ѵ�
	// �׷� �������̵� �� �ڽ� �Ҹ��ڰ� ȣ���
	// private ȸ��
}


CTexture* CResMgr::LoadTexture(const wstring& _strKey, const wstring& _strRelativePath)
{

	CTexture* pTex = FindTexture(_strKey);
	if (nullptr != pTex)
	{
		return pTex;
	}

	// content ���������� ������
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	pTex = new CTexture; // ���ҽ� �Ŵ��� ������ new�� �Ҵ��߱� ������, ���ҽ� �Ŵ��� �Ҹ��ڿ��� �޸� ���� �ʿ�
	pTex->Load(strFilePath);
	
	// �ؽ�ó�� �������� ��ο� ������ � Ű(�̸�)���� ����ƴ��� �˰� �Ѵ�
	pTex->SetKey(_strKey);
	pTex->SetRelativePath(strFilePath);

	m_mapTex.insert(make_pair(_strKey, pTex));

	return pTex;
}

CTexture* CResMgr::CreateTexture(const wstring& _strKey, UINT _iwidth, UINT _iheight)
{
	// ���� Ű �� üũ
	CTexture* pTex = FindTexture(_strKey);
	if (nullptr != pTex)
	{
		return pTex;
	}

	pTex = new CTexture; 
	pTex->Create(_iwidth, _iheight);
	// �ؽ�ó�� �������� ��ο� ������ � Ű(�̸�)���� ����ƴ��� �˰� �Ѵ�
	pTex->SetKey(_strKey);
	m_mapTex.insert(make_pair(_strKey, pTex));

	return pTex;
}

// �ؽ�ó �� ��ȸ ��, Ű �´°� ������ �� �ؽ�ó ��ü ��ȯ
CTexture* CResMgr::FindTexture(const wstring& _strKey)
{
	map<wstring, CRes*>::iterator iter = m_mapTex.find(_strKey);

	if (iter == m_mapTex.end())
	{
		return nullptr;
	}

	return (CTexture*)iter->second;
}
