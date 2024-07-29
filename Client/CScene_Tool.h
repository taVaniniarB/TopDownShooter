#pragma once
#include "CScene.h"

class CUI;
class CTile;

class CScene_Tool:
	public CScene
{
private:
	// 강제 포커싱할 UI 저장
	CUI* m_pUI;
	CTile* m_pSelectedTile;

public:
	virtual void Enter();
	virtual void Exit();
	virtual void update();

	// 작업 형태에 따라 분기처리 or Enum 정의해두자

public:
	// 마우스 클릭 시 타일 인덱스 1 증가
	void SetTileIdx();
	
	void SaveTileData();
	// 타일을 파일로 저장 (절대경로 사용)
	void SaveTile(const wstring& _strFilePath);
	
	
	void LoadTileData();

	
	

public:
	CScene_Tool();
	~CScene_Tool();
};

