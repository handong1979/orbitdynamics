/*!
\file test.cpp
\author HanDle
测试程序
*/
#include "stdafx.h"
//#include "..\EarthOrbit/EarthOrbit.h"
//#pragma comment(lib,"../proj/release/EarthOrbit.lib")
#include <OrbitDyn.h>
#include <PerfTimer.h>
#include <sofa.h>
#pragma comment(lib,"sofa.lib")

using namespace Constant;

//! 测试地月转移轨道
void test_earth_lunar_transfer()
{
	fstream sat("satECI.dat",ios::out);
	sat.precision(15);
//	fstream moon("moonECI.dat",ios::out);
//	moon.precision(15);

	CDateTime end(2007,4,28,16,42,22.77);
	//CDateTime end(2007,4,28,16,42,23.78);
	Kepler Ep3(212859.603,   0.9670074, 30.983*RAD,179.984*RAD,   180.483*RAD,   0);
	//Kepler Ep3(212860,   0.96720074, 30.983*RAD, 180.483*RAD, 179.984*RAD,  0);

	CSatellite CE1;
	CE1.Name = "CE1";
	CE1.SetAutoSave();
	CE1.Initialize(CDateTime(2007,4,23,22,41,32),Ep3, 'i');
	CE1.SetForce(8,ODP_EARTH_TESSERAL|ODP_LUNAR_CENT|ODP_SOLAR_CENT);

	double Step = 360;
	vec3 Pos,Vel; // 月心惯性系位置速度
	CDateTime time = CE1.CurrentEpoch();

	Kepler elem;
	//ECI_MCI(time,CE1.Pos(),CE1.Vel(),Pos,Vel);
	//elem = Cartesian_Kepler(Pos,Vel,GM);
	//sat<<0<<TAB<<CE1.Pos()<<TAB<<CE1.Vel()<<TAB<<Pos<<TAB<<Vel<<TAB<<elem<<endl;

    while(time < end)
	{
		if( (time+=Step) > end){
			Step -= time-end;
			time = end;
		}
		CE1.Propagate(Step,Step);
		ECI_MCI(time,CE1.Pos(),CE1.Vel(),Pos,Vel);
		if(norm(Pos,2)<LunarGrvSph/2){
            elem = Cartesian_Kepler(Pos,Vel,GM);
        	sat<<CE1.t()<<TAB<<CE1.Pos()<<TAB<<CE1.Vel()<<TAB<<Pos<<TAB<<Vel<<TAB<<elem<<endl;
		}
	}
	cout<<time<<endl<<Pos<<TAB<<Vel<<endl;
}

//! 测试环月轨道外推
void test_circle_moon()
{
	CMoonSat CE;
	CE.Name = "CE";
	//double EE = fmod(2*atan(sqrt((1-0.005)/(1+0.005))*tan(20*RAD/2.0))+PI2,PI2);
	//double M = EE-0.005*sin(EE);
	Kepler elem(1947.0, 0.001, 90.0*RAD, 50*RAD, 60.0*RAD, 70*RAD);
	//CE.SetAutoSave(true);
	CE.SetForce(30,0x00);
	CE.Initialize(CDateTime(2006,9,11,0,0,0.0),elem);
	fstream ff("cirerr.dat",ios::out); ff.precision(12);
	fstream stk("stk\\CirLunar.dat",ios::in);
	vec3 stkr,stkv;
	Kepler stke;
	stk>>stkr(0)>>stkr(1)>>stkr(2)>>stkv(0)>>stkv(1)>>stkv(2);
	stke = Cartesian_Kepler(stkr,stkv,GM);
	for(double t = 0;t<=86400*1;t+=60)
	{
		ff << CE.t()/*<<TAB<<CE.a<<TAB<<CE.e<<TAB<<CE.i*DEG<<TAB
			<<CE.Omega*DEG<<TAB<<CE.w*DEG<<TAB<<CE.u*DEG*/<<TAB
			<< CE.Pos() << TAB << CE.Vel() << endl;
		
		CE.Propagate(60,60);
		stk>>stkr(0)>>stkr(1)>>stkr(2)>>stkv(0)>>stkv(1)>>stkv(2);
		stke = Cartesian_Kepler(stkr,stkv,GM);
	}
}


//! 测试地面站计算
void test_facility()
{
	CDateTime t(2012,5,31,4,0,0);
	double lg;
	lg = GetSG(t);
	/************************************************************************/
	/* 预报地面站在惯性系的位置速度                                         */
	/************************************************************************/
	double xyz[3];
	double dxyz[3];

	double R0=6378140.0;	// 米
	double We0=7.292115e-5;
	double lam0 = 118*RAD;
	double phi0 = 33*RAD;

	xyz[0] =  R0 * cos(phi0) * cos(lg+lam0+We0*0 );// 与原公式不同TimeSysN.DynStep
	xyz[1] =  R0 * cos(phi0) * cos(lg+lam0+We0*0 - 90.0*RAD);
	xyz[2] =  R0 * sin(phi0);
	dxyz[0]= -We0* xyz[1];
	dxyz[1]=  We0* xyz[0];
	dxyz[2]=  0.0;

	CFacility fac(118,33,0);
    vec3 eci = fac.ECIPos(t);
}
//! 测试轨道机动
void test_orbit_maneuver()
{
	CSatellite target,chaser;

	chaser.SetForce(6,ODP_LEO); chaser.Name = "chaser"; chaser.SetAutoSave();
	target.SetForce(6,ODP_LEO); target.Name = "target"; target.SetAutoSave();

	double elem[6]  = {7051.1057, 0.003005,	  98.5051*RAD,	  202.2820*RAD,  43.0580*RAD,	  211.4238*RAD};
	double elem2[6] = {7157.7833, 0.001215,	  98.5160*RAD,	  202.2820*RAD,  143.8963*RAD,    122.135*RAD};

//	double elem[6]  = {7058.8484, 0.0016837,	  98.5003*RAD,	  202.2850*RAD,  22.1320*RAD,	  232.2209*RAD};
//	double elem2[6] = {7166.7356, 0.0014711,	  98.5106*RAD,	  202.2817*RAD,  218.6563*RAD,    47.3680*RAD};

    chaser.Initialize(CDateTime(2006,8,13,12,0,0.0),elem,'m');
	target.Initialize(CDateTime(2006,8,13,12,0,0.0),elem2,'m');

	cout<<"Chaser's Mean:"<<chaser.MedianElement()<<endl;
	cout<<"Target's Mean:"<<target.MedianElement()<<endl<<endl;

	vec3 dv;
	dv[0] = 0.001;
	dv[1] = 0.0;
	dv[2] = 0.0;
	chaser.ImpluseManeuver(dv);
	cout<<"Chaser's Mean:"<<chaser.MedianElement()<<endl;


	vec3 p,v;
	RIC(chaser.GetOrbitElements(),target.GetOrbitElements(),p,v);
	cout<<"RelPos:"<<p<<"\t\tDis:"<<norm(p,2)<<endl;

	Maneuver m1,m2;
	InitPlaneJet(chaser.MedianElement(),target.MedianElement(),m1,m2,0.8);
	chaser.Propagate(60,18000);
	target.Propagate(60,18000);

	InitPlaneJet(chaser.MedianElement(),target.MedianElement(),m1,m2);
	chaser.Propagate(60,18000);
	target.Propagate(60,18000);

	cout<<endl<<chaser.t()<<endl;
	cout<<"chaser:"<<chaser<<endl;
	cout<<"target:"<<target<<endl;
	InitNormalJet(chaser.MedianElement(),target.MedianElement(),m1,m2);
	chaser.Propagate(60,18000);
	target.Propagate(60,18000);

	RIC(chaser.GetOrbitElements(),target.GetOrbitElements(),p,v);
	cout<<endl<<"Time:"<<chaser.t()<<endl;
	cout<<"RelPos:"<<p<<"\t\tDis:"<<norm(p,2)<<endl;

	// 输出执行过的DV
// 	list<Maneuver>::iterator iter;
// 	for(iter=chaser.ManeuverList.begin();iter!=chaser.ManeuverList.end();iter++)
// 		cout<<"StartTime:"<<(*iter).StartTime<<"     EndTime:"<<(*iter).EndTime<<"     DV="<<(*iter).DV<<endl;
}
//! 测试摄动加速度的计算
void test_acceleration()
{
	CDateTime StartEpoch(2005,1,1,0,0,0);

	vec3 r,stkECF,stkacc,ECFr,FG,F12;

	fstream stk("STK\\WGS84_EGM96_acceleration.txt",ios::in);
	fstream err("accerr.dat",ios::out);
	for(int i=0;i<12961;i++)
	{
		stk>>r(0)>>r(1)>>r(2);
		stk>>stkECF(0)>>stkECF(1)>>stkECF(2);
		stk>>stkacc(0)>>stkacc(1)>>stkacc(2);

		CDateTime tt = StartEpoch + i*60.0;
		mat33 HG = ECI_ECF(tt);
		ECFr = HG * r;
		FG = PZonalHarmonic(20,ECFr) + PTesseralHarmonic(20,ECFr);
		F12 = HG.t() * FG;

		//F12 += PThirdBodyGrv( r , MoonECI(TDT) , GM );
		//F12 += PThirdBodyGrv( r , SunECI(TDT) , GS );
		//F12 += PLightPressure(0.02,r,SunECI(TDT));

		double R = norm(r,2);
		double miurrr = GE/R/R/R;
        err<<ECFr-stkECF<<TAB<<-miurrr*r+F12-stkacc<<endl;
	}
}
//!
void test_ECI_ECF()
{
/*	CDateTime t(2003,1,13,0,0,0);
	fstream eci("stk\\eci20030113.dat",ios::in);
	fstream ecf("ecf20030113.dat",ios::out);
	ecf.precision(16);
	vec3 I,Iv,J,Jv;
	mat33 HG;
	double MJD;
	vec3 W(0,0,We);
	for(int i=0;i<1441;i++)
	{
		eci>>MJD>>I.x>>I.y>>I.z>>Iv.x>>Iv.y>>Iv.z;
		CDateTime tt;
		tt.SetMJD(MJD);
		HG = ECI_ECF(tt);
		J = HG * I;
		Jv = HG * Iv;
		Jv -= Cross(W,J);
		//J = J.RotationZ(-0.064*RAD);
		ecf<<MJD<<TAB<<J<<TAB<<Jv<<endl;
		t += 60.0;
	}*/
	CDateTime t0(2003,5,15,0,0,0),t;
	fstream fecf("stk\\20030515ECF.txt",ios::in);
	fstream ftod("stk\\20030515TOD.txt",ios::in);
	fstream fj2k("stk\\20030515J2K.txt",ios::in);
	fstream err("ECFTODJ2K.dat",ios::out);
	vec3 ecf,tod,j2k;
	for(int i=0;i<=86400*2;i+=60)
	{
		t = t0 + i;
		fecf>>ecf(0)>>ecf(1)>>ecf(2);
		fj2k>>j2k(0)>>j2k(1)>>j2k(2);
		ftod>>tod(0)>>tod(1)>>tod(2);
		mat33 PN = Nutation(t)*Precession(t);
		mat33 EP = PolarMotion(t)*EarthRotation(t);
		err<<EP*PN*j2k-ecf<<TAB<<PN*j2k-tod<<TAB<<EP*tod-ecf<<endl;
	}
}

//! 测试RKF78
void test_rkf78()
{
	CSatellite sat;
	sat.Name = "sat";
	sat.SetForce(1,0x00);
	double ZY_Orbit[6] = {7157,			0.01,		98.5*RAD,	60*RAD,			50*RAD,		60*RAD};
	sat.Initialize(CDateTime(2004,1,1,0,0,0),ZY_Orbit,'i');
	sat.SetAutoSave();
	sat.Propagate(60,86400);
}

//! 测试轨道外推
void test_orbit_propagator()
{
	enum SATELLITE{ZY,SZ,DFH,BD};
	SATELLITE satclass = BD;

	CSatellite sat;
	
	CDateTime epoch(2006,5,1,0,0,0);
	double ZY_Orbit[6] = {7157,			0.001,		98.5*RAD,	60*RAD,			90*RAD,		60*RAD};
	double SZ_Orbit[6] = {6778,			0.007,		42*RAD,		60*RAD,			90*RAD,		60*RAD};
	double DFH_Orbit[6]= {42164.169637,	0.00001,	0.02*RAD,	138.703448*RAD,	180*RAD,	20*RAD};
	double BD_Orbit[6] = {26610,		0.0022,		55*RAD,		189*RAD,		180*RAD,	109*RAD};

	sat.Mass0 = 1000;  // use default value
	sat.Cr = 1.0;
	sat.LightPressArea = 20.0;
	sat.Cd = 2.2;
	sat.AirDragArea = 20.0;

	mat33 codep = "1e-12,0,0;1e-12,0,0;1e-12,0,0";

	switch(satclass)
	{
	case ZY:
		sat.Initialize(epoch,ZY_Orbit,'i');
		sat.Name = "ZY";
		sat.SetForce(21,ODP_EARTH_ZONAL|ODP_EARTH_TESSERAL|ODP_AIR_DRAG|ODP_SOLAR_CENT|ODP_SOLAR_PRESSURE|ODP_LUNAR_CENT|ODP_POSTNEWTON);
		break;
	case SZ:
		sat.Initialize(epoch,SZ_Orbit,'i');
		sat.SetForce(21,ODP_EARTH_ZONAL|ODP_EARTH_TESSERAL|ODP_AIR_DRAG|ODP_SOLAR_CENT|ODP_SOLAR_PRESSURE|ODP_LUNAR_CENT|ODP_POSTNEWTON);
		sat.Name = "SZ";
		break;
	case DFH:
		sat.Initialize(epoch,DFH_Orbit,'i');
		sat.SetForce(21,ODP_EARTH_ZONAL|ODP_EARTH_TESSERAL|ODP_AIR_DRAG|ODP_SOLAR_CENT|ODP_SOLAR_PRESSURE|ODP_LUNAR_CENT|ODP_POSTNEWTON);
		sat.Name = "DFH";
		break;
	case BD:
		sat.Initialize(epoch,BD_Orbit,'i');
		sat.SetForce(21,ODP_EARTH_ZONAL|ODP_EARTH_TESSERAL|ODP_AIR_DRAG|ODP_SOLAR_CENT|ODP_SOLAR_PRESSURE|ODP_LUNAR_CENT|ODP_POSTNEWTON);
		sat.Name = "BD";
		sat.SetSRPCODE(codep);
		break;
	default:
		break;
	}

	fstream ff("LLA.dat",ios::out);
	fstream xyz("posvel.dat",ios::out);
	ff.precision(12);
	xyz.precision(12);
	CSpherical LLA = sat.GetLLA();
	sat.SetAutoSave();
	double t;
	for(t=0; t<86400; t+=60)
	{
		LLA = sat.GetLLA();
		ff<<t<<TAB<<LLA.Longitude<<TAB<<LLA.Latitude<<TAB<<LLA.Altitude<<endl;
		xyz << t << TAB << sat.Pos().t() << TAB << sat.Vel().t() << endl;
		sat.Propagate(60,60);
	}
	LLA = sat.GetLLA();
	ff<<t<<TAB<<LLA.Longitude<<TAB<<LLA.Latitude<<TAB<<LLA.Altitude<<endl;
	xyz << t << TAB << sat.Pos().t() << TAB << sat.Vel().t() << endl;
}
//! 测试太阳、月球星历
void test_DE405()
{
	CDateTime t0(2000,1,1,0,0,0);
	CDateTime tend(2029,1,1,0,0,0);

	fstream eph("DE405test.eph",ios::out);
	eph.precision(8); eph.setf(ios::scientific);

	fstream eph21("DE421test.eph",ios::out);
	eph21.precision(8); eph21.setf(ios::scientific);

	DE405 * de = DE405::Instance();
	DE421 * de421 = DE421::Instance();
	vec3 MoonPos,MoonVel,SunPos,SunVel;
	vec3 VenusPos,VenusVel,MarsPos,MarsVel,NeptunePos,NeptuneVel;
	for(CDateTime t = t0; t<=tend; t+=86400 )
	{
		de->PlanetEphemeris(t,EARTH,MOON,MoonPos,MoonVel);
		de->PlanetEphemeris(t,SUN,EARTH,SunPos,SunVel);
		de->PlanetEphemeris(t,SUN,VENUS,VenusPos,VenusVel);
		de->PlanetEphemeris(t,SUN,MARS,MarsPos,MarsVel);
		de->PlanetEphemeris(t,MARS,NEPTUNE,NeptunePos,NeptuneVel);
		eph<<t.GetUTC()<<TAB<<MoonPos<<TAB<<MoonVel<<TAB<<SunPos<<TAB<<SunVel<<TAB
			<<VenusPos<<TAB<<VenusVel<<TAB<<MarsPos<<TAB<<MarsVel<<TAB<<NeptunePos<<TAB<<NeptuneVel;
		eph<<endl;

		de421->PlanetEphemeris(t,EARTH,MOON,MoonPos,MoonVel);
		de421->PlanetEphemeris(t,SUN,EARTH,SunPos,SunVel);
		de421->PlanetEphemeris(t,SUN,VENUS,VenusPos,VenusVel);
		de421->PlanetEphemeris(t,SUN,MARS,MarsPos,MarsVel);
		de421->PlanetEphemeris(t,MARS,NEPTUNE,NeptunePos,NeptuneVel);
		eph21<<t.GetUTC()<<TAB<<MoonPos<<TAB<<MoonVel<<TAB<<SunPos<<TAB<<SunVel<<TAB
			<<VenusPos<<TAB<<VenusVel<<TAB<<MarsPos<<TAB<<MarsVel<<TAB<<NeptunePos<<TAB<<NeptuneVel;
		eph21<<endl;
	}
	eph.close();
	eph21.close();
}

//! 测试平根数和瞬根数的计算
void test_mean_inst()
{
	FILE * cla = fopen("Mean\\cla.txt","r");
	FILE * BLL = fopen("Mean\\Brouwer-Lyd Mean Long.txt","r");
	FILE * BLS = fopen("Mean\\Brouwer-Lyd Mean Short.txt","r");
	FILE * KI = fopen("Mean\\Kozai-Izsak Mean.txt","r");

	fstream result("Mean\\MY_Mean.txt",ios::out);
	double elem[7];
	Kepler inst,STK_KI,STK_BLL,STK_BLS;
	while(!feof(cla))
	{
		fscanf(cla,"%lf%lf%lf%lf%lf%lf%lf",elem,elem+1,elem+2,elem+3,elem+4,elem+5,elem+6);
		elem[3] *= RAD;
		elem[4] *= RAD;
		elem[5] *= RAD;
		elem[6] *= RAD;
		inst.SetElements(elem+1);
		result<<elem[0]<<"\t"<<Mean(inst)<<endl;

//		fscanf(BLL,"lf%lf%lf%lf%lf%lf%lf%",elem,elem+1,elem+2,elem+3,elem+4,elem+5,elem+6);
//		STK_BLL.SetElements(elem+1);
//
//		fscanf(BLS,"lf%lf%lf%lf%lf%lf%lf%",elem,elem+1,elem+2,elem+3,elem+4,elem+5,elem+6);
//		STK_BLS.SetElements(elem+1);
//
//		fscanf(KI,"lf%lf%lf%lf%lf%lf%lf%",elem,elem+1,elem+2,elem+3,elem+4,elem+5,elem+6);
//		STK_KI.SetElements(elem+1);
	}
}


void test_Solar_eph()
{
	typedef float BASE;
//	const BASE es = 0.01670529110116;  // 太阳轨道偏心率
//	const BASE is = 0.40907487; // 黄赤交角
//	const BASE ws = -1.3426259; // 太阳近地点幅角
//	const BASE dMs = 1.99096875e-7; //太阳平近点角变化率
	BASE es = (BASE)0.016712;  // 太阳轨道偏心率
	BASE is = (BASE)0.409093; // 黄赤交角
	BASE ws = (BASE)-1.347398; // 太阳近地点幅角
	BASE dMs = (BASE)1.991e-7; //太阳平近点角变化率

	CDateTime Epoch(2005,11,4,0,0,0);

	is = (BASE)Ecliptic_Equator_Angle(Epoch); // 黄赤交角
	ws = (BASE)Sun_Argument_Perigee(Epoch);

	double sis = sin(is);
	double cis = cos(is);

	int Step = 60;

	Kepler Sun_Kepler;
	double Ms,us,Ms0;
	vec3 Sr,Sv;

	Ms0 = Sun_Mean_Anomaly(Epoch);
	DE405 * de = DE405::Instance();
	fstream ss("suninjet.dat",ios::out);  ss.precision(12);
	for(int t = 0;t<=86400*1;t+=Step)
	{
		de->PlanetEphemeris(Epoch+t,EARTH,SUN,Sr,Sv);
		mat33 PR = Precession(Epoch+t);
		mat33 NR = Nutation(Epoch+t);
		Sr = NR*PR*Sr;
		Sr = Sr/norm(Sr,2);
		//Sun_Kepler = Cartesian_Kepler(Sr,Sv,GS);

		/****太阳星历计算*****/
		Ms = Ms0 + dMs*t;
		us = Ms + 2.0*es*sin(Ms) + 1.25*es*es*sin(2.0*Ms) + ws - PI2;

		vec3 Si;
		Si(0) = cos(us);
		Si(1) = cis*sin(us);
		Si(2) = sis*sin(us);

		//ss<<t/Step<<Sun_Kepler<<TAB<<Sun_Kepler.u()*DEG<<TAB<<us*DEG+360<<endl;
		ss<<Sr<<TAB<<Si<<endl;
	}
}

void test_Lunar_eph()
{
	CDateTime epoch0(2014,1,1,0,0,0);
	double mjd0 = epoch0.GetMJD();
	fstream fm("lunar_eph.dat",ios::out);
	fm.precision(12);
	vec3 rm;
	DE405* de = DE405::Instance();
	for(double t=0;t<86400*30;t+=60)
	{
		rm = de->Moon(epoch0+t);
		fm << mjd0 + t/86400 << TAB << rm << endl;
	}
}

void test_nutation()
{
	double du1,du2,dksi,deps;
	CDateTime t(2002,1,1,0,0,0);
	DE405 * de = DE405::Instance();
	for(double i=0;i<86400*365*5;i+=8640)
	{
		de->Nutations(t+i,dksi,deps);
		nutation_angle(t+i,du1,du2);
		cout<<dksi<<TAB<<deps<<TAB<<dksi-du1<<TAB<<deps-du2<<endl;
	}

	mat33 MF = MCI_MCF(t); // test over,OK!
	vec3 r("1936 25 19");
	vec3 MCFr = MF*r;
	cout<<t<<endl;
	cout<<r<<endl<<MCFr<<endl;
}

void test_Legendre_polynomial()
{
	double gsl=0;
	double px=0,dpx=0;
	double x = -0.2;

	double Plx[71]={0};
	double Plx2[71][71]={0};	
	Legendre_sphPl(70,x,Plx);
	Legendre_sphPlm(70,x,Plx2/*,dPlx2*/);
	
	for(int l=0;l<7;l++)
	{
		for(int m=0;m<=l;m++)
		{
			double dlt = m==0?1:2;
			//gsl = sqrt(4*PI*dlt) * gsl_sf_legendre_sphPlm(l,m,x);
			if(m==0)
				printf("[%2d][%2d]= %10.6f  ",l,m,Plx[l]);
			else
				printf("[%2d][%2d]= %10.6f  ",l,m,Plx2[l][m]);
		printf("\n");
	}
	}
	// 本程序和gsl的结果相比差一个(-1)^m，本程序结果与matlab一致，但matlab的公式中含(-1)^m
}

void test_shadow()
{
	CMoonSat sat;
	sat.SetForce(4,ODP_EARTH_ZONAL);
	double Orbit[6] = {1938,0.001,90.0*RAD,	300*RAD,	90*RAD,	60*RAD};
	sat.Initialize(CDateTime(2007,8,28,8,37,0.0),Orbit);
	sat.Name = "shadow";
	double vfm=1.0,pvfm=0.0,  vfe=1.0,pvfe=0.0;
	vec3 rs,rm;
	CDateTime epoch;
	double h = 1;
	DE405 * de = DE405::Instance();
	for(double t=0; t<3600*4; t+=h)
	{
		sat.Propagate(h,h);
		epoch = sat.CurrentEpoch();
		rs = de->Sun(epoch);
		rm = de->Moon(epoch);
		vfm = Shadow(sat.Pos(),rs-rm,Rm);
		vfe = Shadow(sat.Pos()+rm,rs);
		if( ((vfm==0.0 || vfm==1.0) && vfm!=pvfm) || ((vfe==0.0 || vfe==1.0) && vfe!=pvfe) )
		{
            cout<<epoch<<"   "<<vfm<<TAB<<vfe<<endl;			
		}
		pvfm = vfm;
		pvfe = vfe;
	}
}

void test_Lunar_libration()
{
	CDateTime t(2007,1,1,0,0,0);
	CDateTime e(2009,1,1,0,0,0);
	double Omega,i,u;
	DE405 * de = DE405::Instance();
    while(t<e)
	{
		de->Librations(t,Omega,i,u);
		cout<<Omega<<TAB<<i<<TAB<<fmod(u,PI2)<<endl;
		t += 86400.0;
	}
}

void test_SZ6G()
{
	CDateTime t0(2006,3,29,8,00,00);

	Kepler elem0(6726.1676442, 0.0006523, 42.432910*RAD,94.138780*RAD	,7.074640*RAD	,89.368170*RAD);
	CSatellite SZ6G;
	SZ6G.SetForce(21,ODP_LEO);
	SZ6G.Mass0 = 1957.0;
	SZ6G.AirDragArea = 12.24+5.26;
	SZ6G.Cd = 2.1;

	SZ6G.Initialize(CDateTime(2006,3,26,14,55,00),elem0,'i');
	SZ6G.Propagate(60,t0-SZ6G.StartEpoch());

	double vfm=1.0,pvfm=0.0,  vfe=1.0,pvfe=0.0;
	CDateTime epoch;
	vec3 rs,rm;
	DE405 * de = DE405::Instance();
	for(double t=0;t<3600*4;t+=1)
	{
		epoch = SZ6G.CurrentEpoch();
		rs = de->Sun(epoch);
		rm = de->Moon(epoch);
		vfm = Shadow(SZ6G.Pos()-rm,rs,Rm);
		vfe = Shadow(SZ6G.Pos()   ,rs);
		if( ((vfm==0.0 || vfm==1.0) && vfm!=pvfm) || ((vfe==0.0 || vfe==1.0) && vfe!=pvfe) )
		{
			cout<<epoch<<"   "<<vfm<<TAB<<vfe<<endl;			
		}
		pvfm = vfm;
		pvfe = vfe;

		SZ6G.Propagate(1,1);
	}
}

void test_libration()
{
	CDateTime t0(2013,9,1,0,0,0);
// 	vec3 r,v;
// 	DE405.PlanetEphemeris(t,SUN,EARTH,r,v);
// 	cout<<r/AU<<TAB<<v/AU*86400<<endl;
// 	DE405.PlanetEphemeris(t,JUPITER,SUN,r,v);
// 	cout<<r/AU<<TAB<<v/AU*86400<<endl;
	double omega,i,u;
	DE405 * de = DE405::Instance();
	for(double t = 0;t<=86400;t+=60)
	{
		de->Librations(t0+t,omega,i,u);
		u = fmod(u,PI2);
		cout << t0+t << TAB << i << TAB << omega << TAB << u << endl;
	}
}

void test_exception2()
{
	CDateTime abc(1995,1,32,0,0,0);
}

void test_exception()
{
	try{
		test_exception2();
	}
	catch(exception &e){
		cerr << e.what() << endl;
		cerr << typeid(e).name() << endl;
	}

	vec3 a;
	a.ones();	
}

void test_msise00()
{
	CDateTime t(2012,5,1,0,0,0);
	CDateTimeView tv(t);
	double lon = 120*RAD;
	double lat = 40*RAD;
	vec3 r;
	r(0) = cos(lat)*cos(lon);
	r(1) = cos(lat)*sin(lon);
	r(2) = sin(lat);
	double rho;
	double h = 200;
	Msise.input.f107 = 70;
	Msise.input.f107A = 70;
	rho = Msise.Density( t, r*(6371.0+h) );
	cout << t <<TAB<< rho <<TAB<< SA76(h) << endl;

	//for(double dt = 0;dt<24;dt++)
	//{
	//	rho = Msise.Density( t+dt*3600, r*(6371.0+h) );
	//	cout << t+dt*3600 <<TAB<< rho <<TAB<< SA76(h) << endl;
	//}
}

void test_sg()
{
	CDateTime t(2006,9,13,1,2,3);

	double dpsi,deps;
	nutation_angle(t,dpsi,deps); // dpsi:黄经章动
	double eps = Ecliptic_Equator_Angle(t);
	double zeta,theta,Z;
	precession_angle(t,zeta,theta,Z);

	double dt = t.GetUT1CentNum()*36525.0; // + Earth.GetdUT1(t)/86400.0; // 在 t.GetUT1CentNum()中考虑dUT1
	double sgm = fmod((280.4606184 + 360.985647365*dt + 0.2908e-12*dt*dt)*RAD,PI2);
	double sgs =  fmod(dpsi*cos(eps) + sgm, PI2);
	double sgj2000 = sgm - zeta - Z;

	cout << "格林威治平恒星时：" << sgm << "rad    " << sgm*DEG << "deg" << endl;
	cout << "格林威治真恒星时：" << sgs << "rad    " << sgs*DEG << "deg" << endl;
	cout << "J2000格林威治平恒星时：" << sgj2000 << "rad    " << sgj2000*DEG << "deg" << endl;
}

void test_RIC()
{
	CDateTime t;
	Kepler ka(7157.7833, 0.001215,	  98.5160*RAD,	  202.2820*RAD,  143.8963*RAD,    122.135*RAD);
	Kepler kb(7157.7633, 0.001215,	  98.5140*RAD,	  202.2220*RAD,  144.8963*RAD,    121.035*RAD);
	CSatellite a,b;
	a.Initialize(t,ka,'i');
	vec3 r,v;
	RIC(ka,kb,r,v);
	Kepler kk = RIC(kb,r,v);
}

void test_rapidsattlite()
{
	bool writefile = false;
	CDateTime t0(2006,3,29,8,00,00);
	Kepler elem0(6878.1676442,0.0006523,89.432910*RAD,94.138780*RAD,7.074640*RAD,89.368170*RAD);
	CSatellite sat1;
	sat1.SetForce(21,ODP_EARTH_ALL);
	sat1.SetAutoSave(writefile);
	sat1.Initialize(t0,elem0,'i');

 	CPerfTimer timer1;
 	timer1.Start();
	//sat1.Propagate2Equator();
 	sat1.Propagate(10,5000);
 	timer1.Stop();
	double t1 = timer1.Elapsedms()/500;
 	cout<<"Timer1:"<<t1<<"ms"<<endl;

	CRapidSatellite sat2;
	sat2.SetForce(6,ODP_EARTH_ZONAL|ODP_EARTH_TESSERAL);
	sat2.SetAutoSave(writefile);
	sat2.Initialize(t0,elem0,'i');

	CPerfTimer timer2;
	timer2.Start();
	fstream fraps("rapidsat.dat",ios::out);
	for(int i=0;i<100000;i++)
	{
		sat2.Propagate(50);
		if(i%1000==0)
			fraps << i/4.0 << TAB << sat2.GetOrbitElements() 
			        << TAB << Mean(sat2.GetOrbitElements()) << endl;
	}
	timer2.Stop();
	double t2 = timer2.Elapsedms()/100000;
	cout<<"Timer2:"<<t2<<"ms"<<endl;

	cout << "Speed up " << t1/t2 << " times\n";
}

void testBackward()
{
	CDateTime t0(2006,3,29,8,00,00);
	Kepler elem0(6926.1676442, 0.0006523, 98.432910*RAD,94.138780*RAD	,7.074640*RAD	,89.368170*RAD);
	CSatellite sat1;
	sat1.SetForce(4,ODP_EARTH_ALL);
	sat1.SetAutoSave(true);
	sat1.Initialize(t0,elem0,'i');

	CPerfTimer timer1;
	timer1.Start();
	sat1.PropagateBackward(-10,-5000);
	timer1.Stop();
	double t1 = timer1.Elapsed()/500*1000;
	cout<<"Timer Backward:"<<t1<<"ms"<<endl;

	sat1.Propagate(10,5000);
}

//void testEarthOrbtiDll()
//{
//	CEarthOrbit a,b;
//	struct EARTHORBITPARAM ai,bi;
//	ai.year = 2012;
//	ai.month = 11;
//	ai.day = 15;
//	ai.hour = 0;
//	ai.minute = 0;
//	ai.second = 0;
//	ai.a = 7044.1010;
//	ai.e = 0.001;
//	ai.i = 98.055/57.3;
//	ai.Omega = 331.33/57.3;
//	ai.w = 0;
//	ai.M = 6.267/57.3;
//	ai.mass = 730;
//	ai.airdragarea = 1.8;
//
//	bi = ai;
//	bi.M = 6.283/57.3;
//	bi.mass = 67;
//	bi.airdragarea = 0.2;
//
//	a.Init(ai);
//	b.Init(bi);
//
//	int simtime; // 仿真时间(毫秒)
//	int step=10; // 仿真步长(毫秒)
//
//	EARTHORBITDYNINPUT input;
//	EARTHORBITDYNOUTPUT output;
//	for(simtime=0;simtime<20*1000;simtime+=step)
//	{
//		input.stepsize = step;
//		input.f[0] = 5;
//		input.f[1] = 0;
//		input.f[2] = 0;
//		input.massdot = 0.00001;
//		input.q[0] = 0;
//		input.q[1] = 0;
//		input.q[2] = 0;
//		input.q[3] = 1;
//		a.Update(input,output);
//		printf("a.a=%f, ",output.a);
//		
//		input.f[0] = 0;
//		b.Update(input,output);
//		printf("b.a=%f\n",output.a);
//	}	
//}

void checkmemoryleak()
{
	// check memory leak
	vector<CSatellite*> vs;
	vector<CSatellite*>::iterator itvsat;
	while(1){
		CSatellite *sat = new CSatellite;
		vs.push_back(sat);

		vs[0]->Initialize(CDateTime(2015,1,1,0,0,0),Kepler(7044,0.001,98.5*RAD,0,0,0));
		vs[0]->Propagate(60,300);
		vs[0]->GetOrbitElements();

		delete vs[0];
		itvsat = vs.begin();
		vs.erase(itvsat);
	}
}

void test_sofa_Gst()
{
	double ut11,ut12;
	iauUtcut1(2400000.5,53736.0,0.3388134,&ut11,&ut12);
	double tai1,tai2;
	iauUtctai(2400000.5,53736.0,&tai1,&tai2);
	double tt1,tt2;
	iauTaitt(tai1,tai2,&tt1,&tt2);
	double gst06a = iauGst06a(ut11,ut12,tt1,tt2);
	double gst00a = iauGst00a(ut11,ut12,tt1,tt2);
	double gst00b = iauGst00b(ut11,ut12);
	double sg = GetSG(CDateTime(2006,1,1,0,0,0));
	cout << "   GetSG  = " << sg << endl;
	cout << "iauGst06a = " << gst06a << endl;
	cout << "iauGst00a = " << gst00a << endl;
	cout << "iauGst00b = " << gst00b << endl;
	cout << (sg - gst06a)*DEG << endl;
	cout << (sg - gst00a)*DEG << endl;
	cout << (sg - gst00b)*DEG << endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	CPerfTimer timer;
	timer.Start();

//	try
//	{
		//double x;
		//for(x=0;x<86400*20;x+=0.050)
		//	continue;

		cout.precision(12);
		//cout<<x<<endl;
		Kepler kp(42164.2,0.2,0,0.5,1,1);
		vec3 r,v;
		Kepler_Cartesian(kp,r,v);
		kp = Cartesian_Kepler(r,v);

		//test_orbit_propagator();		
		//test_earth_lunar_transfer();
		//test_circle_moon();
		test_orbit_propagator();
		//Kepler orbit(21528+6378, 0.001, 55*RAD,120*RAD, 0*RAD, 45*RAD);
		//cout << Mean(orbit) << endl;		//void test_mean_inst();
		//test_orbit_maneuver();
		//test_rkf78();
		//test_Solar_eph();
		//test_Lunar_eph();
		//test_DE405();
		//test_ECI_ECF();
		//test_facility();
		//test_Solar_eph();
		//test_nutation();
		//test_Legendre_polynomial();
		//test_shadow();
		//test_SZ6G()
		//test_Lunar_libration();
		//test_libration();
		//test_msise00();
		//test_exception2();
		//test_sg();
		//test_RIC();	
		//test_rapidsattlite();
		//testEarthOrbtiDll();
		//testBackward();
		//test_sofa_Gst();
		//checkmemoryleak();

		//CDateTime t;
		////t.SetMJD(55197.999999999985);
		//t.SetMJD(55197.9993055555);
		//cout << t << endl;

		// 	mat33 HG = ECI_ECF(t);
// 		cout << HG << endl;
		//int y,m,d,h,min;
		//double s;
		//GetCalendar(t.GetMJD(),y,m,d,h,min,s);

	/*	CDateTime epoch0(2010,10,1,0,0,0);
		Kepler ks0(7044.0,0.001,  98.074*RAD, 265.0*RAD,90.0*RAD, 20.05*RAD);
		CSatellite sat;
		sat.Initialize(epoch0,ks0,'i');
		sat.SetAutoSave();
		sat.SetForce(6,ODP_ZONAL|ODP_TESSERAL);
		sat.Propagate(60,86400);*/

		//CDateTime t(2011,3,1,0,0,0);
		//vec3 rm,vm;
		//
		//DE405::Instance()->PlanetEphemeris(t,EARTH,MOON,rm,vm);
		////cout << rm <<TAB << vm << endl;
		//cout << Cartesian_Kepler(rm,vm) << endl;
//	}
	//catch(BaseException& e)
	//{
	//	cerr << e.GetFullMessage() << endl;
	//}
// 	catch(exception* e)
// 	{
// 		cerr << ((BaseException*)e)->what() /*e.GetFullMessage()*/ << endl;
// 	}

	timer.Stop();
	cout<<"Total Timer:"<<timer.Elapsed()<<"s"<<endl;
	return 0;
}

