
#include "CBtnUI.h"
class CTileBtnUI :
    public CBtnUI
{
public:
    CTileBtnUI();
    ~CTileBtnUI();

private:
    int m_iIdx;

public:
    void render(HDC _dc);

public:
    void SetIdx(int _iIdx) { m_iIdx = _iIdx; }
};
