/*!
\file CEngine.h
\author HanDle
*/

#ifndef __TANK_H
#define __TANK_H

#include "config.h"
#include "armadillo_BLAS_LAPACK.h"
using namespace arma;

class ORBITDYN_API CEngine
{
public:
	//!发动机比冲
	double Isp;
	//!推力
	double F;

	CEngine(){}
	~CEngine(){};

	// 质量流速
	double dm();

	void UpdateStatus();
};

//! 常比冲、常推力的发动机
class ORBITDYN_API CEngine_const:public CEngine
{
public:
	CEngine_const()
	{
		Isp = 305.0;
		F = 490.0;
	}
	CEngine_const(double I,double f)
	{
		Isp = I;
		F = f;
	}
	~CEngine_const(){}

	bool dv_dt(double SatMass,const double dv,double &halfdt) const;
	void UpdateStatus(){}
};

//!星上的储箱
class ORBITDYN_API CEngine_Poly:public CEngine
{
public:
	//!燃料充满时质量
	double FullFuelMass;
	//!加注的推进剂总质量
	double FuelMass0;
	//!推进剂质量
	double M;
	//!初始温度
	double Temperature0;
	//!当前温度
	double Temperature;
	//!初始压力
	double Press0;
	//!当前压力
	double Press;
	//!燃料密度
	double FuelDensity;
	//!贮箱拟和参数
	double D0,D1,D2,D3,D4,G0,G1,G2,G3;
	//!发动机最大连续工作时间
	double MaxManeuverTime;
	CEngine_Poly();
	~CEngine_Poly(){}
	bool dv_dt(double SatMass,const double dv,double &halfdt1) const;
	bool dv_dt(double SatMass,const double dv1,double &halfdt1,
		const double dv2,double &halfdt2) const;
	double GetPower();
	void UpdateStatus(double dt);
};

//! 推力器组
class ORBITDYN_API CThrusterSet
{
	//! 喷管数
	int Num;
	//! 推力安装阵
	mat33 CF;
	//! 力矩安装阵
	mat33 CT;
public:
	CThrusterSet();

	~CThrusterSet();

	void Install(int n,mat& f,mat& t);
};

#endif // __TANK_H
