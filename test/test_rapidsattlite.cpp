/*!
\file test.cpp
\author HanDle
≤‚ ‘≥Ã–Ú
*/
#include <OrbitDyn.h>
#include <PerfTimer.h>

using namespace Constant;

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

int main(int argc, char* argv[])
{
	CPerfTimer timer;
	timer.Start();

	try
	{
		cout.precision(12);

		test_rapidsattlite();
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