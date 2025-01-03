#include "global.h"
#include "CResMgr.h"

#include "CPathMgr.h"
#include "CTexture.h"
#include "CSound.h"

CResMgr::CResMgr()
{
}

CResMgr::~CResMgr()
{
	Safe_Delete_Map(m_mapTex);
	Safe_Delete_Map(m_mapSound);
	Safe_Delete_Map(m_mapImage);
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
	pTex->SetRelativePath(_strRelativePath);

	m_mapTex.insert(make_pair(_strKey, pTex));

	return pTex;
}

CTexture* CResMgr::CreateTexture(const wstring& _strKey, UINT _iwidth, UINT _iheight)
{
	// 동일 키 값 체크
	CTexture* pTex = FindTexture(_strKey);
	if (nullptr != pTex)
	{
		return pTex;
	}

	pTex = new CTexture; 
	pTex->Create(_iwidth, _iheight);
	// 텍스처가 스스로의 경로와 스스로 어떤 키(이름)으로 저장됐는지 알게 한다
	pTex->SetKey(_strKey);
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

Image* CResMgr::LoadGdiImage(const wstring& _strKey, const wstring& _strRelativePath)
{
	Image* pImage = FindImage(_strKey);
	if (nullptr != pImage)
	{
		return pImage;
	}

	// content 폴더까지의 절대경로
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	pImage = new Image(strFilePath.c_str()); // 리소스 매니저 내에서 new로 할당했기 때문에, 리소스 매니저 소멸자에서 메모리 해제 필요

	m_mapImage.insert(make_pair(_strKey, pImage));

	return pImage;
}


Image* CResMgr::FindImage(const wstring& _strKey)
{
	map<wstring, Image*>::iterator iter = m_mapImage.find(_strKey);

	if (iter == m_mapImage.end())
	{
		return nullptr;
	}

	return (Image*)iter->second;
}

CSound* CResMgr::LoadSound(const wstring& _strKey, const wstring& _strRelativePath)
{
	CSound* pSound = FindSound(_strKey);
	if (nullptr != pSound)
	{
		return pSound;
	}

	// content 폴더까지의 절대경로
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	pSound = new CSound; // 리소스 매니저 내에서 new로 할당했기 때문에, 리소스 매니저 소멸자에서 메모리 해제 필요
	pSound->Load(strFilePath);

	// 텍스처가 스스로의 경로와 스스로 어떤 키(이름)으로 저장됐는지 알게 한다
	pSound->SetKey(_strKey);
	pSound->SetRelativePath(_strRelativePath);

	m_mapSound.insert(make_pair(_strKey, pSound));

	return pSound;
}

CSound* CResMgr::FindSound(const wstring& _strKey)
{
	map<wstring, CRes*>::iterator iter = m_mapSound.find(_strKey);

	if (iter == m_mapSound.end())
	{
		return nullptr;
	}

	return (CSound*)iter->second;
}
