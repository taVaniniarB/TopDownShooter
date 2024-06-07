#pragma once

enum class KEY
{
	LEFT,
	RIGHT,
	UP,
	DOWN,	
	
	Q,
	W,
	E,
	R,
	T,
	Y,
	U,
	I,
	O,
	P,
	A,
	S,
	D,
	F,
	G,
	Z,
	X,
	C,
	V,
	B,

	ALT,
	CTRL,
	LSHIFT,
	SPACE,
	ENTER,
	ESC,

	LBTN,
	RBTN,

	LAST,
};

enum class KEY_STATE
{
	TAP, // 막 누른거
	HOLD, // 누르고 있는
	AWAY, // 막 뗀 시점
	NONE, // 이전에도 안눌렸고 지금도 안눌림
};

struct tKeyInfo
{
	KEY_STATE	eState;// 상태값
	bool		bPrevPush; // 이전 프레임에 키가 눌렸는가?
};

class CKeyMgr
{
	SINGLE(CKeyMgr);

private:
	vector<tKeyInfo> m_vecKey;
	// 벡터 안에 모든 키 값들 enum 순서대로 저장
	// 키값은 인덱스로 접근
	Vec2			m_vCurMousePos;

public:
	void init();
	void update();

public:
	KEY_STATE GetKeyState(KEY _eKey) { return m_vecKey[(int)_eKey].eState; }
	Vec2 GetMouseCursor() { return m_vCurMousePos; }
};

