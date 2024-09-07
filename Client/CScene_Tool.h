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
	int m_iSelectedTileIdx;
	// ���� ���õ� �� �ε���
	WALL_DIR m_eSelectedWallDir;

	// ���� �巡�� ���ΰ�?
	bool m_vWallDrag;
	// �� ���� �� �巡�� ��ǥ
	Vec2 m_vDragStartPos;
	Vec2 m_vDragEndPos;

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
	
	void SetSelectedWall(int _wallPos);
	
	void LoadTileData();

	void SetSelectedObj(SELECT_OPTION _eOption) { m_eSelctedObj = _eOption; }
	SELECT_OPTION GetSelectedObj() { return m_eSelctedObj; }

private:
	// �巡�׷� Wall�� ����� �Լ�
	//void CreateWall(Vec2 _vStartPos, Vec2 _vEndPos);
	

public:
	CScene_Tool();
	~CScene_Tool();
};

