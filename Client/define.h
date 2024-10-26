#pragma once


// Singleton ��ũ��
#define SINGLE(type) public:\
						 static type* GetInst() \
						 {\
							 static type mgr;\
							 return &mgr;\
						 }\
					 private:\
						type();\
						~type();
// ������ ������ � Ÿ��(Ŭ����) ��ü�� ���� �� �ּҸ� ��ȯ���ִ�
// GetInst �Լ��� �ۼ��� ��ũ��
//mgr: �Ŵ������ ��
//���� �ٵ� ��ũ�� ���ǿ� ���Եǵ��� �ϰ� �ʹٸ� �� ���� \�� ���̱�

#define fDT CTimeMgr::GetInst()->GetfDT()
#define DT CTimeMgr::GetInst()->GetDT()

#define CLONE(type) type* Clone() { return new type(*this); }

#define KEY_CHECK(key, state) CKeyMgr::GetInst()->GetKeyState(key) == state
#define KEY_HOLD(key) KEY_CHECK(key, KEY_STATE::HOLD)
#define KEY_TAP(key) KEY_CHECK(key, KEY_STATE::TAP)
#define KEY_AWAY(key) KEY_CHECK(key, KEY_STATE::AWAY)
#define KEY_NONE(key) KEY_CHECK(key, KEY_STATE::NONE)
// ������ ��ǥ ��ȯ
#define MOUSE_POS CKeyMgr::GetInst()->GetMouseCursor()

#define PI 3.1415926535f

#define TILE_SIZE 32
#define WALL_THICKNESS 12

#define FADEOUT_TIME 1.f
#define FADEIN_TIME 1.f

#define WM_SELECT_WEAPON (WM_USER + 1)
#define WM_SELECT_SCENE (WM_USER + 2)

enum class GROUP_TYPE
{
	DEFAULT,
	TILE,
	TILE_WALL,
	WALL,
	CORNER,
	MONSTER,
	PLAYER,
	DROPPED_WEAPON,
	PROJ_PLAYER,
	PROJ_MONSTER,
	WEAPON,

	HITBOX_PLAYER,     // �÷��̾� ��Ʈ�ڽ�
	HITBOX_MONSTER,    // ���� ��Ʈ�ڽ�

	SCENE_CHANGER = 29,
	UI = 30,
	MOUSE_POINTER = 31,
	END = 32,
};

enum class SCENE_TYPE
{
	TOOL,
	START,
	STAGE_01,
	STAGE_02,
	BOSS,
	MENU,
	END,
};

enum class BRUSH_TYPE
{
	HOLLOW,
	BLACK,
	END,
};

enum class PEN_TYPE
{
	RED,
	GREEN,
	BLUE,
	YELLOW,
	END,
};

enum class UI_TYPE
{
	TEXT,
	AMMO,
	AMMO_IMAGE,
	COMBO,
	SCORE,
	END,
};

enum class FONT_TYPE
{
	IMPACT,
	SYSTEM,
	GULIM,
	END,
};

enum class EVENT_TYPE
{
	CREATE_OBJECT,
	DELETE_OBJECT,
	SCENE_CHANGE,
	CHANGE_AI_STATE,
	UI_BTN_DOWN,
	UI_BTN_UP,

	END,
};

enum class MON_STATE
{
	IDLE,
	PATROL,
	TRACE,
	ATT,
	RUN,
	DEAD,
	END,
};

enum class WALL_DIR
{
	TOP,
	BOTTOM,
	LEFT,
	RIGHT,
	END,
};

enum class WALL_TYPE
{
	TILE,
	WALL,
	CORNER,
	NONE,
};

enum class MON_TYPE
{
	NORMAL,
	KNIFE_MONSTER,
	PIPE_MONSTER,
	FRIENDLY,
};

enum class WEAPON_TYPE
{
	MELEE,
	GUN,
};

enum class GUN_TYPE
{
	M16,
	SHOTGUN,
	NONE,
};

enum class MELEE_TYPE
{
	KNIFE,
	PIPE,
	NONE,
};

enum class FULL_WEAPON_TYPE
{
	M16,
	SHOTGUN,
	KNIFE,
	PIPE,
	END,
};

enum class COLLIDER_TYPE
{
	DEFAULT,
	HITBOX,
	END,
};