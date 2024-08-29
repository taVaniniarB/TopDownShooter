#pragma once
#include "global.h"

#include "CMonFactory.h"

// 컴파일 속도 올리기 위해 헤더 참조 대신 전방선언
// 헤더참조 했다면, object 변경할 때마다 scene쪽도 매번 확인돼서 느려짐
// 구체적으로는 모르고, 이런 애가 있다~ 정도만 알게되는거임
// 타입에 대한 구체적 정보가 없기 때문에 포인터로밖에 사용이 안 됨
class CObject;

class CScene
{
private:
	// 부모 타입 포인터로 자식타입 다 받을 수 있으니, 이거 하나로 모두관리
	// 오브젝트를 저장 관리할 벡터를 그룹 개수만큼 선언한
	// 오브젝트 주소 받는 벡터의 배열!
	vector<CObject*>	m_arrObj[(UINT)GROUP_TYPE::END];
	wstring				m_strName; // Scene 이름

	UINT				m_iTileX; // 타일 가로 개수
	UINT				m_iTileY; // 타일 세로 개수

	CObject*			m_pPlayer; // Player

public:
	void SetName(const wstring& _strName) { m_strName = _strName; }
	const wstring& GetName() { return m_strName; }


	UINT GetTileX() { return m_iTileX; }
	UINT GetTileY() { return m_iTileY; }

	CObject* GetPlayer() { return m_pPlayer; }


	virtual void update();
	virtual void finalUpdate();
	virtual void render(HDC _dc);

	void render_tile(HDC _dc);

	// 씬 진입 시 초기 작업을 하는 함수. 부모에선 구현 안함.
	// 자식들이 자기만의 기능을 구현하면 되니까 부모 쪽에서 가상함수화 시켜서
	// 실제 객체쪽(본인쪽)의 start가 호출되도록 한거니까
	// 자기 버전으로 구현해야 한다, 라는 걸 알리는 용도니까
	// 순수가상함수가 되어야 함
	// > Scene은 직접 객체 생성이 불가능해진다.
	// > 순가함을 보유하고 있는 클래스는, 
	// 실제 객체화할 수 있는 클래스가 아니라
	// 어떤 함수들이 구현되어야 하는지 알려주는, 추상 클래스화
	virtual void Enter() = 0;
	// 씬에서 빠질 때 호출
	virtual void Exit() = 0;

public:

	void AddObject(CObject* _pObj, GROUP_TYPE _eType) { m_arrObj[(UINT)_eType].push_back(_pObj); }
	void RegisterPlayer(CObject* _pPlayer) { m_pPlayer = _pPlayer; }

	// 벡터 복제 안되게 벡터 그룹 그 자체를 주도록 함 (참조) + 수정방지 const
	const vector<CObject*>& GetGroupObject(GROUP_TYPE _eType) { return m_arrObj[(UINT)_eType]; }
	void DeleteGroup(GROUP_TYPE _eTarget); // 그룹 지정 시 해당 그룹의 모든 옵젝 삭제
	void DeleteAll();
	const vector<CObject*>* GetObjectArr() { return m_arrObj; }
	
	vector<CObject*>& GetUIVector() { return m_arrObj[(UINT)GROUP_TYPE::UI]; }

	// 가로세로 값 받아서 타일 만들어주는 함수
	void CreateTile(UINT _iXCount, UINT _iYCount);
	void ChangeTile(Vec2 _vMousePos, int _idx);

	// Tool Scene에서 파일을 세이브하고, 씬(을 상속받은 다른 씬들)은 그것을 load한다
	void LoadTile(const wstring& _sttRelativePath);

public:
	CScene();
	virtual ~CScene();
};