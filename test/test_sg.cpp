/*!
\file test.cpp
\author HanDle
测试程序
*/
#include <OrbitDyn.h>
using namespace Constant;

void test_sg()
{
	CDateTime t(2006,9,13,1,2,3);

	double dpsi,deps;
	nutation_angle(t,dpsi,deps); // dpsi:黄经章动
	double eps = Ecliptic_Equator_Angle(t);
	double zeta,theta,Z;
	precession_angle(t,zeta,theta,Z);

	double dt = t.GetUT1CentNum()*36525.0; // + Earth.GetdUT1(t)/86400.0; // 在 t.GetUT1CentNum()中考虑dUT1
	double sgm = GetSGM(t);
	double sgs =  GetSG(t);
	double sgj2000 = sgm - zeta - Z;

	cout << "格林威治平恒星时：" << sgm << "rad    " << sgm*DEG << "deg" << endl;
	cout << "格林威治真恒星时：" << sgs << "rad    " << sgs*DEG << "deg" << endl;
	cout << "J2000格林威治平恒星时：" << sgj2000 << "rad    " << sgj2000*DEG << "deg" << endl;
}

int main(int argc, char* argv[])
{
    try
	{
		cout.precision(12);

		test_sg();
	}
	catch(BaseException& e)
	{
		cerr << e.GetFullMessage() << endl;
	}
 	catch(exception* e)
 	{
 		cerr << ((BaseException*)e)->what() << endl;
 	}

	return 0;
}
