#pragma once
#include "CObject.h"

class CGravity
{
private:
	CObject* m_pOwner;
	bool m_bGround;

public:
	void finalUpdate();
	void SetGround(bool _b);
	bool GetGround() { return m_bGround; }

public:
	CGravity();
	~CGravity();

	friend class CObject;
};

