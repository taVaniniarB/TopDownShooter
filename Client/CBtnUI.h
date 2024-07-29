#pragma once
#include "CUI.h"

#include "CScene.h"
#include "CObject.h"

typedef void (*BTN_FUNC) (DWORD_PTR, DWORD_PTR);

// CScene���� �Ļ��Ǵ� ��� Ŭ������ ����Լ��� ���� �� �ִ�
// Scene�� ������ ���ڰ� void�� ��ȯ���� void�� ����Լ� Ÿ���� '*SCENE_MEMFUNC'�� �����Ѵ�.
typedef void (CScene:: *SCENE_MEMFUNC) (void);
// typedef void (CObject:: *PLAYER_MEMFUNC) (void);

class CBtnUI :
    public CUI
{
private:
    BTN_FUNC        m_pFunc;
    DWORD_PTR       m_param1;
    DWORD_PTR       m_param2; 

    SCENE_MEMFUNC   m_pSceneFunc; // �Լ� ������
    CScene*         m_pSceneInst; // �Լ��� ȣ���� ��ü

public:
    virtual void MouseOn(); // ���콺 ���ٴ�
    virtual void MouseLbtnDown(); // ���� ��ư ������ ��
    virtual void MousebtnUp(); // ���콺 ������ ��
    virtual void MouseLbtnClicked();

    // �Լ� ������ + �־��� ���� �޾Ƽ� ������ ����� ����
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

