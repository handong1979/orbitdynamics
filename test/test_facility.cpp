#include <OrbitDyn.h>
#include <PerfTimer.h>

using namespace Constant;

//! 测试地面站计算
void test_facility()
{
	CDateTime t(2012, 5, 31, 4, 0, 0);
	double lg;
	lg = GetSG(t);
	/************************************************************************/
	/* 预报地面站在惯性系的位置速度                                         */
	/************************************************************************/
	double xyz[3];
	double dxyz[3];

	double R0 = 6378140.0;	// 米
	double We0 = 7.292115e-5;
	double lam0 = 118 * RAD;
	double phi0 = 33 * RAD;

	xyz[0] = R0 * cos(phi0) * cos(lg + lam0 + We0 * 0);// 与原公式不同TimeSysN.DynStep
	xyz[1] = R0 * cos(phi0) * cos(lg + lam0 + We0 * 0 - 90.0*RAD);
	xyz[2] = R0 * sin(phi0);
	dxyz[0] = -We0 * xyz[1];
	dxyz[1] = We0 * xyz[0];
	dxyz[2] = 0.0;

	CFacility fac(118, 33, 0);
	vec3 eci = fac.ECIPos(t);
}


int main(int argc, char* argv[])
{
	//CPerfTimer timer;
	//timer.Start();

	try
	{
		cout.precision(12);

		test_facility();
	}
	catch (BaseException& e)
	{
		cerr << e.GetFullMessage() << endl;
	}
	catch (exception* e)
	{
		cerr << ((BaseException*)e)->what() << endl;
	}

	//timer.Stop();
	//cout<<"Total Timer:"<<timer.Elapsed()<<"s"<<endl;
	return 0;
}