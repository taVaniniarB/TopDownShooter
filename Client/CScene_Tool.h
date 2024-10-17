 #pragma once
#include "CScene.h"


enum class SELECT_OPTION
{
	TILE,
	MONSTER,
	WALL,
	PLAYER,
	SCENE_CHANGER,

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

	// ���� ���� �� �ɼ�
	WALL_TYPE m_eSelectedWall;

	// ���� ���õ� Ÿ�� �ε���
	int m_iSelectedTileIdx;
	// ���� ���õ� �� �ε���
	WALL_DIR m_eSelectedWallDir;
	WALL_DIR m_eSelectedWallDir2;

	// �巡�� ��� ���� ���� �������� ����
	Vec2 m_vStartPos;
	Vec2 m_vEndPos;
	Vec2 m_vSCScale;
	Vec2 m_vSCPos;

	// �׸��� ǥ��
	bool m_bGrid;
	bool m_bDrawingSquare;

public:
	virtual void Enter();
	virtual void Exit();
	virtual void update();
	virtual void render(HDC _dc);

public:
	void SaveSceneData();
	void SaveScene(const wstring& _strFilePath);
	void LoadSceneData();

	// Ÿ��
public:
	// ���콺 Ŭ�� �� Ÿ�� �ε��� 1 ����
	//void SetTileIdx();
	

	void SaveTileData();
	// Ÿ���� ���Ϸ� ���� (������ ���)
	void SaveTile(const wstring& _strFilePath);
	void LoadTileData();

	void SetSelectedTile(int _idx);
	void SetSelectedWall();
	void SetSelectedCorner();
	void SetSelectedTileWall(int _idx);
	void SetSelectedPlayer();
	void SetSelectedMosnter();
	void SetSelectedSC();


	// ��
public:
	void SaveWallData();
	void SaveWall(const wstring& _strFilePath);
	void LoadWallData();

public:
	void SetSelectedObj(SELECT_OPTION _eOption) { m_eSelctedObj = _eOption; }
	SELECT_OPTION GetSelectedObj() { return m_eSelctedObj; }

private:
	// �巡�׷� Wall�� ����� �Լ�
	//void CreateWall(Vec2 _vStartPos, Vec2 _vEndPos);
	

public:
	CScene_Tool();
	~CScene_Tool();
};

