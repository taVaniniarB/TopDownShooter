#pragma once
#include "CScene.h"

class CUI;
class CTile;

class CScene_Tool:
	public CScene
{
private:
	// ���� ��Ŀ���� UI ����
	CUI* m_pUI;
	CTile* m_pSelectedTile;

public:
	virtual void Enter();
	virtual void Exit();
	virtual void update();

	// �۾� ���¿� ���� �б�ó�� or Enum �����ص���

public:
	// ���콺 Ŭ�� �� Ÿ�� �ε��� 1 ����
	void SetTileIdx();
	
	void SaveTileData();
	// Ÿ���� ���Ϸ� ���� (������ ���)
	void SaveTile(const wstring& _strFilePath);
	
	
	void LoadTileData();

	
	

public:
	CScene_Tool();
	~CScene_Tool();
};

