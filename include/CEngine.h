/*!
\file CEngine.h
\author HanDle
*/

#ifndef __TANK_H
#define __TANK_H

#include "Config.h"
#include "armadillo_BLAS_LAPACK.h"
using namespace arma;

class ORBITDYN_API CEngine
{
public:
	//!�������ȳ�
	double Isp;
	//!����
	double F;

	CEngine(){}
	~CEngine(){};

	// ��������
	double dm();

	void UpdateStatus();
};

//! ���ȳ塢�������ķ�����
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

//!���ϵĴ���
class ORBITDYN_API CEngine_Poly:public CEngine
{
public:
	//!ȼ�ϳ���ʱ����
	double FullFuelMass;
	//!��ע���ƽ���������
	double FuelMass0;
	//!�ƽ�������
	double M;
	//!��ʼ�¶�
	double Temperature0;
	//!��ǰ�¶�
	double Temperature;
	//!��ʼѹ��
	double Press0;
	//!��ǰѹ��
	double Press;
	//!ȼ���ܶ�
	double FuelDensity;
	//!������Ͳ���
	double D0,D1,D2,D3,D4,G0,G1,G2,G3;
	//!�����������������ʱ��
	double MaxManeuverTime;
	CEngine_Poly();
	~CEngine_Poly(){}
	bool dv_dt(double SatMass,const double dv,double &halfdt1) const;
	bool dv_dt(double SatMass,const double dv1,double &halfdt1,
		const double dv2,double &halfdt2) const;
	double GetPower();
	void UpdateStatus(double dt);
};

//! ��������
class ORBITDYN_API CThrusterSet
{
	//! �����
	int Num;
	//! ������װ��
	mat33 CF;
	//! ���ذ�װ��
	mat33 CT;
public:
	CThrusterSet();

	~CThrusterSet();

	void Install(int n,mat& f,mat& t);
};

#endif // __TANK_H
