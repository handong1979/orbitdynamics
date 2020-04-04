// GTO2GEO.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <iostream>

#include <orbitdyn.h>
using namespace Constant;

int verbout = 0;
struct paramout {
	double T1, T2, T3;
	double tof;
	double dm1, dm2, dm3;
	double dm;
} out;

bool inshadow(CDateTime t, vec3 r)
{
	vec3 rs = DE421::Instance()->Sun(t);
	double vf = Shadow(r, rs, Constant::Rs);
	if (vf < 0.1)
		return true;
	else
		return false;
}

mat33 getunw(Kepler kp)
{
	mat33 coi = GetCoi(kp);
	double f = kp.f();
	double beta = atan2(kp.e*sin(f), 1 + kp.e*cos(f)); //飞行路径角
	return mat33(RotationY(beta)*coi);
}

double LowThrustTransfer(CDateTime epoch, Kepler kp, double PHI1, double PHI2,
	double Force, double isp, double mass0)
{
	CSatellite sat;
	sat.Initialize(epoch, kp);
	sat.Mass0 = mass0;
	sat.SetForce(4, ODP_EARTH_ZONAL | ODP_EARTH_TESSERAL | ODP_AIR_DRAG | ODP_LUNAR_CENT | ODP_SOLAR_CENT);

	int  fstop = 0;
	double step;
	double dm[3] = { 0.0 };
	mat33 cbi, coi, cui;
	CQuaternion qbi;
	sat.SetEngine(isp, Force);
	sat.ThrustDirection = "0,0,1";
	Kepler km; // 平根数

	vec3 si, so, su;

#if _DEBUG 
	int lastt = 0;
#endif

	CDateTime t0 = sat.CurrentEpoch();
	double m0 = sat.Mass();

	if (verbout)
	{
		sat.SetAutoSave();
	}
	//////////////////////////////////////////////////////////////////////////
	///////////////////////////  phase 1: hp to 1000km  ////////////////////// 
	//////////////////////////////////////////////////////////////////////////
	while (!fstop) {
		// thrust direction / attitude
		cui = getunw(sat.GetOrbitElements());
		si = DE421::Instance()->Sun(sat.CurrentEpoch());
		si = si / norm(si, 2);
		su = cui * si;
		cbi = mat33(RotationY(PI / 2)*cui);
		sat.qbi = cbi;
		// step size
		double h = norm(sat.Pos(), 2) - Re;
		if (h > 10000)
			step = 600;
		else
			step = 120;
		// shadow
		bool insh = inshadow(sat.CurrentEpoch(), sat.Pos());
		if (insh)
			sat.ThrustIsOn = false;
		else
			sat.ThrustIsOn = true;
		// prop
		sat.Propagate(step, step);

		// hp>1000?
		double hp = sat.a*(1.0 - sat.e) - Re;
		if (hp > 1000)
			fstop = true;

#if _DEBUG 
		if ((sat.t() / 86400 - lastt) >= 1 || fstop)
		{
			mat33 cbo = cbi * cui.t();
			cout << "KP = " << sat.t() / 86400 << TAB
				<< sat.GetOrbitElements()
				<< "   Zbu = " << cbo.row(2) << endl;
			lastt = sat.t() / 86400;
		}
#endif

	}
	double T1 = (sat.CurrentEpoch() - t0) / 86400;
	dm[0] = m0 - sat.Mass();

#if _DEBUG
	cout << "PHASE 1 :::::::::::: T1 = " << T1
		<< "    dm1 = " << dm[0] << endl;
#endif
	//////////////////////////////////////////////////////////////////////////
	///////////////////////   phase 2: a to 42164  /////////////////////////// 
	//////////////////////////////////////////////////////////////////////////
	fstop = 0;
	while (!fstop) {
		// thrust direction / attitude
		cui = getunw(sat.GetOrbitElements());
		if (sat.u > PI / 2 && sat.u < 1.5*PI)
			cbi = mat33(RotationY(PI / 2)*RotationZ(-PHI1)*cui);
		else
			cbi = mat33(RotationY(PI / 2)*RotationZ(PHI1)*cui);
		sat.qbi = cbi;
		// step size
		double h = norm(sat.Pos(), 2) - Re;

		if (h > 10000)
			step = 600;
		else
			step = 120;
		// shadow
		bool insh = inshadow(sat.CurrentEpoch(), sat.Pos());
		if (insh)
			sat.ThrustIsOn = false;
		else
			sat.ThrustIsOn = true;
		// prop
		sat.Propagate(step, step);
		// a>42164?
		km = Mean(sat.GetOrbitElements());
		if (km.a > 42100)
			fstop = true;

#if _DEBUG 
		if ((sat.t() / 86400 - lastt) >= 1 || fstop)
		{
			mat33 cbo = cbi * cui.t();
			cout << "KP = " << sat.t() / 86400 << TAB
				<< km << "   Zbu = " << cbo.row(2) << endl;
			lastt = sat.t() / 86400;
		}
#endif

	}
	double T2 = (sat.CurrentEpoch() - t0) / 86400;
	dm[1] = m0 - sat.Mass() - dm[0];

#if _DEBUG
	cout << "PHASE 2 :::::::::::: T2 = " << T2
		<< "     dm2 = " << dm[1] << endl;
#endif
	//////////////////////////////////////////////////////////////////////////
	//////////////////   phase 3  e to 0  and i to 0   ///////////////////////
	//////////////////////////////////////////////////////////////////////////
	fstop = 0;
	while (!fstop) {
		// thrust direction / attitude
		coi = GetCoi(sat.GetOrbitElements());
		if (sat.e > 0.001 && sat.i*DEG > 0.01)
		{
			if (sat.u > PI / 2 && sat.u < 1.5*PI)
				cbi = mat33(RotationX(PHI2)*RotationY(sat.f - PI / 2)*coi);
			else
				cbi = mat33(RotationX(-PHI2)*RotationY(sat.f - PI / 2)*coi);
		}
		else if (sat.e > 0.001) // only eccentrity
		{
			cbi = mat33(RotationY(sat.f - PI / 2)*coi);
		}
		else // only inclination
		{
			if (sat.u > PI / 2 && sat.u < 1.5*PI)
				cbi = mat33(RotationX(PI / 2)*coi);
			else
				cbi = mat33(RotationX(-PI / 2)*coi);
		}
		sat.qbi = cbi;
		// step size
		double h = norm(sat.Pos(), 2) - Re;
		if (h > 10000)
			step = 600;
		else
			step = 120;
		// shadow
		bool insh = inshadow(sat.CurrentEpoch(), sat.Pos());
		if (insh)
			sat.ThrustIsOn = false;
		else
			sat.ThrustIsOn = true;
		// prop
		sat.Propagate(step, step);
		// e<0.01 and i<0.01deg?
		km = Mean(sat.GetOrbitElements());
		if (sat.e < 0.001 && sat.i*DEG < 0.01)
			fstop = true;

#if _DEBUG 
		if ((sat.t() / 86400 - lastt) >= 1 || fstop)
		{
			cout << "KP = " << sat.t() / 86400 << TAB
				<< km << "   Zbi = " << cbi.row(2) << endl;
			lastt = sat.t() / 86400;
		}
#endif
	}
	double T3 = (sat.CurrentEpoch() - t0) / 86400;
	dm[2] = m0 - sat.Mass() - dm[0] - dm[1];

#if _DEBUG
	cout << "PHASE 3 :::::::::::: T3 = " << T3 << "    dm3 = " << dm[2] << endl;
#endif

	if (verbout)
	{
		out.dm1 = dm[0];
		out.dm2 = dm[1];
		out.dm3 = dm[2];
		out.T1 = T1;
		out.T2 = T2 - T1;
		out.T3 = T3 - T2 - T1;
		out.tof = T3;
		out.dm = m0 - sat.Mass();
	}

	return T3;
}

CDateTime string2epoch(string s)
{
	int y, m, d, h, min;
	double sec;
	sscanf_s(s.c_str(), "%d-%d-%d %d:%d:%lf", &y, &m, &d, &h, &min, &sec);
	CDateTime t(y, m, d, h, min, sec);
	return t;
}

int main(int argc, char* argv[])
{
	CDateTime launchtime(2019, 3, 15, 0, 0, 0);

	double Hp = 200;
	double Ha = 35786;
	double mass0 = 100;
	double f = 0.01;
	double isp = 1000;
	double incl = 28.5;
	double Omega = 0;
	double w = 0;
	double M = 0;

	string infilename;
	if (argc < 2) {
		infilename = "input.txt";
	}
	else {
		infilename = string(argv[1]);
	}

	//string Epoch;
	//BEGIN_READ(infilename)
	//READVAR(Hp, "%lf")
	//READVAR(Ha, "%lf")
	//READVAR(incl, "%lf")
	//READVAR(mass0, "%lf")
	//READVAR(f, "%lf")
	//READVAR(isp, "%lf")
	//READSTR(Epoch)
	//READVAR(Omega, "%lf")
	//READVAR(w, "%lf")
	//READVAR(M, "%lf")
	//END_READ

	//launchtime = string2epoch(Epoch);

	double a = (Ha + Hp) / 2 + Re;
	double e = (Ha - Hp) / 2 / a;
	Kepler gto0(a, e, incl*RAD, Omega*RAD, w*RAD, M*RAD);

	double PHI1 = 15;
	double PHI2 = 15;

	double tof;

	// 5400kg,300mN,1800s==>403天
	// 5400kg,320mN,1600s==>384.3天  40  60

////  遍历 ////
	verbout = 0;

	// 大致确定搜索最大最小范围
	double minPHI, maxPHI;
	if (incl < 5)
	{
		minPHI = 5;
		maxPHI = 31;
	}
	else if (incl < 10)
	{
		minPHI = 10;
		maxPHI = 36;
	}
	else
	{
		minPHI = 20;
		maxPHI = 51;
	}
	// 搜索变量
	double list[100][3];
	int idx = 0, minidx = 0;
	double mintof = 1e4;
	// 搜索循环
	for (PHI1 = minPHI; PHI1 < maxPHI; PHI1 += 5)
	{
		for (PHI2 = minPHI; PHI2 < maxPHI; PHI2 += 5)
		{
			tof = LowThrustTransfer(launchtime, gto0, PHI1*RAD, PHI2*RAD, f, isp, mass0);
			list[idx][0] = PHI1;
			list[idx][1] = PHI2;
			list[idx][2] = tof;
			// 查找最小值		
			if (list[idx][2] < mintof)
			{
				mintof = list[idx][2];
				minidx = idx;
			}

			idx++;
			cout << "PHI1 = " << PHI1 << " PHI2 = " << PHI2
				<< " tof = " << tof << endl;
		}
	}

	// opt:
	cout << "*** opt PHI1 = " << list[minidx][0] << " PHI2 = "
		<< list[minidx][1] << " tof = " << list[minidx][2] << endl;
	PHI1 = list[minidx][0];
	PHI2 = list[minidx][1];

	//// 单纯形搜索算法 ////


	//// 单次计算  ////
	verbout = 1;
	tof = LowThrustTransfer(launchtime, gto0, PHI1*RAD, PHI2*RAD, f, isp, mass0);
	cout << "TOF = " << tof << endl;

	// 记录结果
	fstream fopt("transparam.txt", ios::out);
	fopt << "PHI1 = " << PHI1 << endl;
	fopt << "PHI2 = " << PHI2 << endl;
	fopt << "TOF = " << tof << endl;
	fopt << "dm  = " << out.dm << endl;
	fopt << " T1 = " << out.T1 << endl;
	fopt << " T2 = " << out.T2 << endl;
	fopt << " T3 = " << out.T3 << endl;
	fopt << "dm1 = " << out.dm1 << endl;
	fopt << "dm2 = " << out.dm2 << endl;
	fopt << "dm3 = " << out.dm3 << endl;
	fopt.close();

	return 0;
}

