#pragma once
#include "global.h"

#include "CMonFactory.h"

// ������ �ӵ� �ø��� ���� ��� ���� ��� ���漱��
// ������� �ߴٸ�, object ������ ������ scene�ʵ� �Ź� Ȯ�εż� ������
// ��ü�����δ� �𸣰�, �̷� �ְ� �ִ�~ ������ �˰ԵǴ°���
// Ÿ�Կ� ���� ��ü�� ������ ���� ������ �����ͷιۿ� ����� �� ��
class CObject;

class CScene
{
private:
	// �θ� Ÿ�� �����ͷ� �ڽ�Ÿ�� �� ���� �� ������, �̰� �ϳ��� ��ΰ���
	// ������Ʈ�� ���� ������ ���͸� �׷� ������ŭ ������
	// ������Ʈ �ּ� �޴� ������ �迭!
	vector<CObject*>	m_arrObj[(UINT)GROUP_TYPE::END];
	wstring				m_strName; // Scene �̸�

	UINT				m_iTileX; // Ÿ�� ���� ����
	UINT				m_iTileY; // Ÿ�� ���� ����

	CObject*			m_pPlayer; // Player

	bool m_bUIClicked; // �̹� ������ Ŭ���� UI Ŭ���ΰ�?

public:
	void SetName(const wstring& _strName) { m_strName = _strName; }
	const wstring& GetName() { return m_strName; }

	void SetUIClicked(bool _b) { m_bUIClicked = _b; }
	bool GetUIClicked() { return m_bUIClicked; }

	UINT GetTileX() { return m_iTileX; }
	UINT GetTileY() { return m_iTileY; }

	CObject* GetPlayer() { return m_pPlayer; }

	virtual void start();
	virtual void update();
	virtual void finalUpdate();
	virtual void render(HDC _dc);

	void render_tile(HDC _dc);

	// �� ���� �� �ʱ� �۾��� �ϴ� �Լ�. �θ𿡼� ���� ����.
	// �ڽĵ��� �ڱ⸸�� ����� �����ϸ� �Ǵϱ� �θ� �ʿ��� �����Լ�ȭ ���Ѽ�
	// ���� ��ü��(������)�� start�� ȣ��ǵ��� �ѰŴϱ�
	// �ڱ� �������� �����ؾ� �Ѵ�, ��� �� �˸��� �뵵�ϱ�
	// ���������Լ��� �Ǿ�� ��
	// > Scene�� ���� ��ü ������ �Ұ���������.
	// > �������� �����ϰ� �ִ� Ŭ������, 
	// ���� ��üȭ�� �� �ִ� Ŭ������ �ƴ϶�
	// � �Լ����� �����Ǿ�� �ϴ��� �˷��ִ�, �߻� Ŭ����ȭ
	virtual void Enter() = 0;
	// ������ ���� �� ȣ��
	virtual void Exit() = 0;

public:

	void AddObject(CObject* _pObj, GROUP_TYPE _eType) { m_arrObj[(UINT)_eType].push_back(_pObj); }
	void RegisterPlayer(CObject* _pPlayer) { m_pPlayer = _pPlayer; }

	// ���� ���� �ȵǰ� ���� �׷� �� ��ü�� �ֵ��� �� (����) + �������� const
	const vector<CObject*>& GetGroupObject(GROUP_TYPE _eType) { return m_arrObj[(UINT)_eType]; }
	void DeleteGroup(GROUP_TYPE _eTarget); // �׷� ���� �� �ش� �׷��� ��� ���� ����
	void DeleteAll();
	const vector<CObject*>* GetObjectArr() { return m_arrObj; }
	
	vector<CObject*>& GetUIVector() { return m_arrObj[(UINT)GROUP_TYPE::UI]; }

	// ���μ��� �� �޾Ƽ� Ÿ�� ������ִ� �Լ�
	void CreateTile(UINT _iXCount, UINT _iYCount);
	void ChangeTile(Vec2 _vMousePos, int _idx);

	// �� ��ü�� ����� �Լ�
	void GenerateWall(Vec2 vMousePos, WALL_DIR _eSelectedWallDir);
	void GenerateCorner(Vec2 vMousePos, WALL_DIR _eSelectedWallDir);
	void GenerateTileWall(Vec2 vMousePos, int _idx);
	//void CreateWall(Vec2 vMousePos);
	void CreateWall(Vec2 vMousePos, WALL_DIR _eSelectedWallDir, WALL_TYPE _eWallType, int _idx);
	void CreateWall(Vec2 vMousePos, WALL_DIR _eSelectedWallDir1, WALL_DIR _eSelectedWallDir2);
	void DeleteWall(Vec2 vMousePos, WALL_TYPE _eType);
	//void DeleteCorner(Vec2 vMousePos);

	// Tool Scene���� ������ ���̺��ϰ�, ��(�� ��ӹ��� �ٸ� ����)�� �װ��� load�Ѵ�
	void LoadTile(const wstring& _sttRelativePath, FILE* pFile);

	void LoadWall(const wstring& _strRelativePath, FILE* pFile);

	void LoadScene(const wstring& _strRelativePath);

public:
	CScene();
	virtual ~CScene();
};