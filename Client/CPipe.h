#pragma once
#include "CMelee.h"
class CPipe :
	public CMelee
{
public:
	CPipe();
	~CPipe();

	CLONE(CPipe);
public:
	virtual void Attack();

private:
	//irtual void CreateDamageArea();
};

