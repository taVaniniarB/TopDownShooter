
#include "CBtnUI.h"
class CTileBtnUI :
    public CBtnUI
{
public:
    CTileBtnUI();
    ~CTileBtnUI();

private:
    int m_iIdx;
    bool m_bSelected;

public:
    void render(HDC _dc);

public:
    void SetIdx(int _iIdx) { m_iIdx = _iIdx; }
    void SetSelected(bool _b) { m_bSelected = _b; }
};
