/*!
\file test.cpp
\author HanDle
测试程序
*/
#include <OrbitDyn.h>
#include <PerfTimer.h>

using namespace Constant;

//! 测试环月轨道外推
void test_circle_moon()
{
	CMoonSat CE;
	//CE.Name = "CE";
	//double EE = fmod(2*atan(sqrt((1-0.005)/(1+0.005))*tan(20*RAD/2.0))+PI2,PI2);
	//double M = EE-0.005*sin(EE);
	Kepler elem(1947.0, 0.001, 90.0*RAD, 50*RAD, 60.0*RAD, 70*RAD);
	CE.SetAutoSave(true);
	CE.SetForce(30,0x00);
	CE.Initialize(CDateTime(2006,9,11,0,0,0.0),elem);
	fstream ff("cirerr.dat",ios::out); ff.precision(12);
	for(double t = 0;t<=86400*1;t+=60)
	{
		cout << CE.t()<<TAB<<CE.a<<TAB<<CE.e<<TAB<<CE.i*DEG<<TAB
			<<CE.Omega*DEG<<TAB<<CE.w*DEG<<TAB<<CE.u*DEG<<TAB
			<< CE.Pos() << TAB << CE.Vel() << endl;
		
		CE.Propagate(60,60);
	}
}

int main(int argc, char* argv[])
{
	CPerfTimer timer;
	timer.Start();

	try
	{
		cout.precision(12);

		test_circle_moon();
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