#pragma once
#include "CScene.h"


enum class SELECT_OPTION
{
	TILE,
	MONSTER,
	WALL,
	PLAYER,


	NONE,
};


class CUI;
class CTile;

class CScene_Tool:
	public CScene
{
private:
	// 강제 포커싱할 UI 저장
	CUI* m_pUI;

	// 현재 선택된 오브젝트 옵션
	SELECT_OPTION m_eSelctedObj;

	// 현재 선택된 타일 인덱스
	int m_pSelectedTileIdx;

public:
	virtual void Enter();
	virtual void Exit();
	virtual void update();

public:
	// 마우스 클릭 시 타일 인덱스 1 증가
	//void SetTileIdx();
	
	void SaveTileData();
	// 타일을 파일로 저장 (절대경로 사용)
	void SaveTile(const wstring& _strFilePath);
	void SetSelectedTile(int _idx);
	
	
	void LoadTileData();

	void SetSelectedObj(SELECT_OPTION _eOption) { m_eSelctedObj = _eOption; }
	SELECT_OPTION GetSelectedObj() { return m_eSelctedObj; }

	
	

public:
	CScene_Tool();
	~CScene_Tool();
};

