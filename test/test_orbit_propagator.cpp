/*!
\file test.cpp
\author HanDle
测试程序
*/
#include <OrbitDyn.h>
#include <PerfTimer.h>

using namespace Constant;

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
		//sat.SetSRPCODE(codep);
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

int main(int argc, char* argv[])
{
	CPerfTimer timer;
	timer.Start();

	try
	{
		cout.precision(12);

		test_orbit_propagator();
	}
	catch(BaseException& e)
	{
		cerr << e.GetFullMessage() << endl;
	}
 	catch(exception* e)
 	{
 		cerr << ((BaseException*)e)->what() << endl;
 	}

	timer.Stop();
	cout<<"Total Timer:"<<timer.Elapsed()<<"s"<<endl;
	return 0;
}