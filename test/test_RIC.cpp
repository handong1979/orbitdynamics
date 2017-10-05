/*!
\file test.cpp
\author HanDle
≤‚ ‘≥Ã–Ú
*/
#include <OrbitDyn.h>
#include <PerfTimer.h>

using namespace Constant;

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

int main(int argc, char* argv[])
{
	CPerfTimer timer;
	timer.Start();

	try
	{
		cout.precision(12);

		test_RIC();
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