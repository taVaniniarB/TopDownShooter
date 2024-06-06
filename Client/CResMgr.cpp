#include "pch.h"
#include "CResMgr.h"

#include "CPathMgr.h"
#include "CTexture.h"

CResMgr::CResMgr()
{
}

CResMgr::~CResMgr()
{
	Safe_Delete_Map(m_mapTex);
	// 전역함수를 통해 priviate인 자식(텍스쳐)의 소멸자를 호출하는법
	// 맵의 데이터 타입을 부모(리소스)로 해서, 지우기 함수를 호출한다
	// 그럼 오버라이딩 된 자식 소멸자가 호출됨
	// private 회피
}


CTexture* CResMgr::LoadTexture(const wstring& _strKey, const wstring& _strRelativePath)
{

	CTexture* pTex = FindTexture(_strKey);
	if (nullptr != pTex)
	{
		return pTex;
	}

	// content 폴더까지의 절대경로
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	pTex = new CTexture; // 리소스 매니저 내에서 new로 할당했기 때문에, 리소스 매니저 소멸자에서 메모리 해제 필요
	pTex->Load(strFilePath);
	
	// 텍스처가 스스로의 경로와 스스로 어떤 키(이름)으로 저장됐는지 알게 한다
	pTex->SetKey(_strKey);
	pTex->SetRelativePath(strFilePath);

	m_mapTex.insert(make_pair(_strKey, pTex));

	return pTex;
}

// 텍스처 맵 순회 후, 키 맞는거 있으면 그 텍스처 객체 반환
CTexture* CResMgr::FindTexture(const wstring& _strKey)
{
	map<wstring, CRes*>::iterator iter = m_mapTex.find(_strKey);

	if (iter == m_mapTex.end())
	{
		return nullptr;
	}

	return (CTexture*)iter->second;
}
