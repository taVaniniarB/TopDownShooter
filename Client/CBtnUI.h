#pragma once
#include "CUI.h"

#include "CScene.h"
#include "CObject.h"

typedef void (*BTN_FUNC) (DWORD_PTR, DWORD_PTR);

// CScene에서 파생되는 모든 클래스의 멤버함수들 받을 수 있다
// Scene에 구현된 인자가 void고 반환형이 void인 멤버함수 타입
typedef void (CScene:: *SCENE_MEMFUNC) (void);
typedef void (CObject::* PLAYER_MEMFUNC) (void);

class CBtnUI :
    public CUI
{
private:
    BTN_FUNC        m_pFunc;
    DWORD_PTR       m_param1;
    DWORD_PTR       m_param2; 

    SCENE_MEMFUNC   m_pSceneFunc; // 함수 포인터
    CScene*         m_pSceneInst; // 함수를 호출할 객체

public:
    virtual void MouseOn(); // 마우스 갖다댐
    virtual void MouseLbtnDown(); // 왼쪽 버튼 눌렸을 때
    virtual void MousebtnUp(); // 마우스 떼졌을 때
    virtual void MouseLbtnClicked();

    // 함수 포인터 + 넣어줄 인자 받아서 포인터 멤버에 세팅
    void SetClickedCallBack(BTN_FUNC _pFunc, DWORD_PTR _param1, DWORD_PTR _param2)
    {
        m_pFunc = _pFunc;
        m_param1 = _param1;
        m_param2 = _param2;
    }

    void SetClickedCallBack(CScene* _pScene, SCENE_MEMFUNC _pSceneFunc)
    {
        m_pSceneInst = _pScene;
        m_pSceneFunc = _pSceneFunc;
    }

    CLONE(CBtnUI);

public:
    CBtnUI();
    ~CBtnUI();
};

