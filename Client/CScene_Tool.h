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
	// ���� ��Ŀ���� UI ����
	CUI* m_pUI;

	// ���� ���õ� ������Ʈ �ɼ�
	SELECT_OPTION m_eSelctedObj;

	// ���� ���õ� Ÿ�� �ε���
	int m_pSelectedTileIdx;

public:
	virtual void Enter();
	virtual void Exit();
	virtual void update();

public:
	// ���콺 Ŭ�� �� Ÿ�� �ε��� 1 ����
	//void SetTileIdx();
	
	void SaveTileData();
	// Ÿ���� ���Ϸ� ���� (������ ���)
	void SaveTile(const wstring& _strFilePath);
	void SetSelectedTile(int _idx);
	
	
	void LoadTileData();

	void SetSelectedObj(SELECT_OPTION _eOption) { m_eSelctedObj = _eOption; }
	SELECT_OPTION GetSelectedObj() { return m_eSelctedObj; }

	
	

public:
	CScene_Tool();
	~CScene_Tool();
};

