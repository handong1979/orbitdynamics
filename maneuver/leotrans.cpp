#include <OrbitDyn.h>
#include <PerfTimer.h>

using namespace Constant;

//! 测试轨道机动
void leotrans()
{
	CSatellite target, chaser;

	chaser.SetForce(6, ODP_LEO); chaser.Name = "chaser"; chaser.SetAutoSave();
	target.SetForce(6, ODP_LEO); target.Name = "target"; target.SetAutoSave();

	double elem[6] = { 7051.1057, 0.003005,	  98.5051*RAD,	  202.2820*RAD,  43.0580*RAD,	  211.4238*RAD };
	double elem2[6] = { 7157.7833, 0.001215,	  98.5160*RAD,	  202.2820*RAD,  143.8963*RAD,    122.135*RAD };

	//	double elem[6]  = {7058.8484, 0.0016837,	  98.5003*RAD,	  202.2850*RAD,  22.1320*RAD,	  232.2209*RAD};
	//	double elem2[6] = {7166.7356, 0.0014711,	  98.5106*RAD,	  202.2817*RAD,  218.6563*RAD,    47.3680*RAD};

	chaser.Initialize(CDateTime(2006, 8, 13, 12, 0, 0.0), elem);
	target.Initialize(CDateTime(2006, 8, 13, 12, 0, 0.0), elem2);

	cout << "Chaser's Mean:" << chaser.MedianElement() << endl;
	cout << "Target's Mean:" << target.MedianElement() << endl << endl;

	vec3 dv;
	dv[0] = 0.001;
	dv[1] = 0.0;
	dv[2] = 0.0;
	chaser.ImpluseManeuver(dv);
	cout << "Chaser's Mean:" << chaser.MedianElement() << endl;


	vec3 p, v;
	RIC(chaser.GetOrbitElements(), target.GetOrbitElements(), p, v);
	cout << "RelPos:" << p << "\t\tDis:" << norm(p, 2) << endl;

	Maneuver m1, m2;
	InitPlaneJet(chaser.MedianElement(), target.MedianElement(), m1, m2, 0.8);
	chaser.Propagate(60, 18000);
	target.Propagate(60, 18000);

	InitPlaneJet(chaser.MedianElement(), target.MedianElement(), m1, m2);
	chaser.Propagate(60, 18000);
	target.Propagate(60, 18000);

	cout << endl << chaser.t() << endl;
	cout << "chaser:" << chaser << endl;
	cout << "target:" << target << endl;
	InitNormalJet(chaser.MedianElement(), target.MedianElement(), m1, m2);
	chaser.Propagate(60, 18000);
	target.Propagate(60, 18000);

	RIC(chaser.GetOrbitElements(), target.GetOrbitElements(), p, v);
	cout << endl << "Time:" << chaser.t() << endl;
	cout << "RelPos:" << p << "\t\tDis:" << norm(p, 2) << endl;

	// 输出执行过的DV
	// 	list<Maneuver>::iterator iter;
	// 	for(iter=chaser.ManeuverList.begin();iter!=chaser.ManeuverList.end();iter++)
	// 		cout<<"StartTime:"<<(*iter).StartTime<<"     EndTime:"<<(*iter).EndTime<<"     DV="<<(*iter).DV<<endl;
}

int main(int argc, char* argv[])
{
	//CPerfTimer timer;
	//timer.Start();

	try
	{
		cout.precision(12);

		rendezvous();
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