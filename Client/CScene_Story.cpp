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

	// ���� ����: ���� ������ ���� �ٸ� �ؽ���
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
