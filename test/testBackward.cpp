/*!
\file test.cpp
\author HanDle
测试程序
*/
#include <OrbitDyn.h>
//#include <PerfTimer.h>

using namespace Constant;

void testBackward()
{
	CDateTime t0(2006,3,29,8,00,00);
	Kepler elem0(6926.1676442, 0.0006523, 98.432910*RAD,94.138780*RAD	,7.074640*RAD	,89.368170*RAD);
	CSatellite sat1;
	sat1.SetForce(4,ODP_EARTH_ALL);
	sat1.SetAutoSave(true);
	sat1.Initialize(t0,elem0);

	//CPerfTimer timer1;
	//timer1.Start();
	sat1.PropagateBackward(-10,-5000);
	//timer1.Stop();
	//double t1 = timer1.Elapsed()/500*1000;
	//cout<<"Timer Backward:"<<t1<<"ms"<<endl;

	sat1.Propagate(10,5000);
}

int main(int argc, char* argv[])
{
	//CPerfTimer timer;
	//timer.Start();

	try
	{
		cout.precision(12);

		testBackward();
	}
	catch(BaseException& e)
	{
		cerr << e.GetFullMessage() << endl;
	}
 	catch(exception* e)
 	{
 		cerr << ((BaseException*)e)->what() << endl;
 	}

	//timer.Stop();
	//cout<<"Total Timer:"<<timer.Elapsed()<<"s"<<endl;
	return 0;
}
