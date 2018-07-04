/*!
\file Engine.cpp
\author HanDle
*/

#include <cmath>
#include "CEngine.h"
#include "constant.h"

double CEngine::dm()
{
	return -F/Isp/Constant::g;
}

bool CEngine_const::dv_dt(double SatMass,const double dv,double &halfdt) const
{

	return true;
}


CEngine_Poly::CEngine_Poly(void)
{
//拟和参数
	D0 = -0.97f;
	D1 = 28.64f;
	D2 = -6.09f;
	D3 = 0.5f;
	D4 = 0.0f;
	G0 = 1888.8f;
	G1 = 635.1f;
	G2 = -403.6f;
	G3 = 89.61f;
//贮箱属性
	FullFuelMass=120.0f;
	FuelMass0=100.0f;
	FuelDensity=1008.0f;
	Temperature0=273.0f;
	Press0=1.7f;
//贮箱状态
	Temperature=Temperature0;
	M=FuelMass0;
	Press=Press0;
//发动机最大连续工作时间(秒)
	MaxManeuverTime=2400.0f;
}

/*!
冲量dv需要的前后段时间、压降和燃料消耗\n
SatMass：卫星干重     dv：需要的速度变化量\n
halfdt1：在变轨点前的时间\n
只输出前半段的时间，函数内不改变maneuver和tank的值\n
\return 返回true时，表示求得halfdt的值；返回false时，表示需要的发动机工作时间超过了最长连续工作时间，只能工作最长时间的80％(可调，太长会导致变轨误差增大)
*/
bool CEngine_Poly::dv_dt(double SatMass,const double dv,double &halfdt) const
{
	double p1,p2,m,F,I;
	double h=4.0;
	double dvt=0;
	double t=0.0;
	p1=Press;
	m=M;
	while(dvt<dv)
	{
		F=D0+p1*(D1+p1*(D2+p1*(D3+p1*D4)));
		I=G0+p1*(G1+p1*(G2+p1*G3));
		p2=p1/(1.0f+(F*h/I/(FullFuelMass-m)));
		dvt+=F*h/(SatMass+m);
		if(dvt<dv)
		{
            p1=p2;
            m=FullFuelMass-(FullFuelMass-FuelMass0)*Temperature/Temperature0*Press0/p2;
			t+=h;
		}
		else
		{
            dvt-=F*h/(SatMass+m);
			if((dv-dvt)<1e-4) break;
			h=h/2.0f;
		}
	}
    halfdt=t/2;
	if(halfdt>MaxManeuverTime/2.0f)  //超出发动机最大连续工作时间
	{
		//halfdt=MaxManeuverTime/2.0;
		return false;
	}
	return true;
}  //end of dv_dt

/*!冲量dv1和dv2需要的前后段时间、压降和燃料消耗\n
SatMass：卫星干重     dv1,dv2：需要的速度变化量\n
halfdt1,halfdt2：在变轨点前的时间\n
只输出前半段的时间，函数内不改变maneuver和tank的值\n
\return 返回true时，表示求得halfdt的值；返回false时，表示需要的发动机工作时间超过了最长连续工作时间，只能工作最长时间的80％(可调，太长会导致变轨误差增大)
*/
bool CEngine_Poly::dv_dt(double SatMass,const double dv1,double &halfdt1,const double dv2,double &halfdt2) const
{
	double p1,p2,m,F,I;
	double h=1.0f;
	double dvt1=0.0f,dvt2=0.0f;
	double t1=0.0f,t2=0.0f;
	p1=Press;
	m=M;
	while(dvt1<dv1)
	{
		F=D0+p1*(D1+p1*(D2+p1*(D3+p1*D4)));
		I=G0+p1*(G1+p1*(G2+p1*G3));
		p2=p1/(1.0f+(F*h/I/(FullFuelMass-m)));
		dvt1+=F*h/(SatMass+m);
		if(dvt1<dv1)
		{
			p1=p2;
			m=FullFuelMass-(FullFuelMass-FuelMass0)*Temperature/Temperature0*Press0/p2;
			t1+=h;
		}
		else
		{
			dvt1-=F*h/(SatMass+m);
			if((dv1-dvt1)<1e-4) break;
			h=h/2.0f;
		}
	}
	h=1.0;
	while(dvt2<dv2)
	{
		F=D0+p1*(D1+p1*(D2+p1*(D3+p1*D4)));
		I=G0+p1*(G1+p1*(G2+p1*G3));
		p2=p1/(1.0f+(F*h/I/(FullFuelMass-m)));
		dvt2+=F*h/(SatMass+m);
		if(dvt2<dv2)
		{
			p1=p2;
			m=FullFuelMass-(FullFuelMass-FuelMass0)*Temperature/Temperature0*Press0/p2;
			t2+=h;
		}
		else
		{
			dvt2-=F*h/(SatMass+m);
			if((dv2-dvt2)<1e-4) break;
			h=h/2.0f;
		}
	}

	halfdt1=t1/2;
	halfdt2=t2/2;

	if(halfdt1>MaxManeuverTime/2.0 ||halfdt2>MaxManeuverTime)  //超出发动机最大连续工作时间
	{
		//halfdt=MaxManeuverTime/2.0;
		return false;
	}
	return true;
}
//! 根据当前储箱压力计算发动机推力
double CEngine_Poly::GetPower()
{
	return F = D0 + Press * (D1 + Press * (D2 + Press * (D3 + Press * D4) ) );
}
//! 更新储箱的压力和剩余燃料质量
void CEngine_Poly::UpdateStatus(double dt)
{
	Isp = G0 + Press * (G1 + Press * (G2 + Press * G3));
	double Pt = Press/(1 + F*dt/(FullFuelMass-M)/Isp);
	Press = Pt;
	M = FullFuelMass-(FullFuelMass-FuelMass0)*(Temperature/Temperature0)*(Press0/Pt);
}

/*! 默认为三个喷管
*/
CThrusterSet::CThrusterSet():Num(3)
{
	CT.set_size(3,3);
	CF.set_size(3,3);	
}

CThrusterSet::~CThrusterSet()
{

}

void CThrusterSet::Install(int n,mat& f,mat& t)
{
	Num = n;
	CF.set_size(3,Num);
	CF = f;
	CT.set_size(3,Num);
	CT = t;
}
