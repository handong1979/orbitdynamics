#pragma once

#include "Satellite.h"

void ECI_RLP(const CDateTime& t,Vector& r_GCI,Vector& v_GCI,Vector& r_RLP,Vector& v_RLP);
void RLP_ECI(const CDateTime& t,Vector& r_RLP,Vector& v_RLP,Vector& r_GCI,Vector& v_GCI);

class CLibrationSat:public CSatellite
{
public:

	CLibrationSat(void);
	~CLibrationSat(void);

	//! 初始化及设置函数
	void Initialize(const CDateTime& dt,const Vector& r,const Vector& v);

private:
	void RefreshKepler();
};
