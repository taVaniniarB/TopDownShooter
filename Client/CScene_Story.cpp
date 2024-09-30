#include "pch.h"
#include "CScene_Story.h"

#include "CMousePtr.h"


CScene_Story::CScene_Story()
{
}

CScene_Story::~CScene_Story()
{
}

void CScene_Story::Enter()
{
	//LoadScene(m_wSceneRelativePath);

	// 추후 변경: 씬의 종류에 따라 다른 텍스쳐
	CMousePtr* mousePtr = new CMousePtr();
	AddObject(mousePtr, GROUP_TYPE::MOUSE_POINTER);
}

void CScene_Story::Exit()
{
}

void CScene_Story::update()
{

}

void CScene_Story::render()
{
}
