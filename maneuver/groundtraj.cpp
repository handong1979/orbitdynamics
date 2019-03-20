#include <OrbitDyn.h>
#include <PerfTimer.h>

using namespace Constant;

//! 测试轨道机动
void groundtraj()
{
	CSatellite target, chaser;

	chaser.SetForce(6, ODP_LEO); 
	chaser.SetAutoSave();
	//target.SetForce(4, ODP_EARTH_ZONAL|ODP_EARTH_TESSERAL);
	target.SetForce(2, ODP_EARTH_ZONAL);
	target.SetAutoSave();

	double elem[6] = { 7051.1057, 0.003005,	  98.5051*RAD,	  202.2820*RAD,  0.0580*RAD,	  211.4238*RAD };
	
	chaser.Initialize(CDateTime(2016, 8, 13, 12, 0, 0.0), elem);
	target.Initialize(CDateTime(2016, 8, 13, 12, 0, 0.0), elem);

	cout << "Chaser's Mean:" << chaser.MedianElement() << endl;
	cout << "Target's Mean:" << target.MedianElement() << endl << endl;

	CSpherical lla,llat;
	FILE* flla;
	flla = fopen("groundtraj.txt", "w");
	
	double dLANmax = 10;
	for (int i = 0;i < 100;i++)
	{
		chaser.Propagate2EquatorAscNode();
		target.Propagate2EquatorAscNode();

		lla = chaser.GetLLA();
		llat = target.GetLLA();

		double dl = lla.Longitude - llat.Longitude;
		if (dl > 350) dl -= 360;
		else if (dl < -350) dl += 360;
		dl = dl * RAD * 6378;

		if (dl < -dLANmax)
		{
			// 超出西边界
		}
		else if (dl > dLANmax)
		{
			// 超出东边界

		}

		cout << "Chaser's Lon:" << lla.Longitude 
			<< " Lat: " << lla.Latitude
			<< "\tLon err:" << (lla.Longitude-llat.Longitude)*RAD*6378 << endl;
		fprintf(flla,"%e\t%e\t%e\t%e\n",lla.Longitude , lla.Latitude ,
			llat.Longitude , llat.Latitude );
	}
	fclose(flla);
}

void RepeatOrbit(int days, int revs, double lan0)
{
	double time = days * 86164.09;
	double Tn = time / revs;
	double dlan = Tn * We * DEG;
	FILE *fp;
	fp = fopen("rolan.txt", "w");
	fprintf(fp, "%3d   %11.6lf\n",0, lan0);
	double lan = lan0;
	for (int i = 1;i < revs;i++)
	{
		lan += dlan;
		if (lan > 180)
			lan -= 360;
		fprintf(fp, "%3d   %11.6lf\n",i, lan);
	}
	fclose(fp);
}

//void RepeatOrbit(int days, int revs, double iclination)
//{
//	double time = days * 86164.09;
//	double Tn = time / revs;
//	// node period
//	// Tn = 2*pi/n*(1 + 1.5*J2*(Re/a)^2*(1-4*cos(i)*cos(i)))
//	// So:
//	// (Re/a)^2 = (Tn*n/2/pi - 1) / 1.5 /J2 / (1-4*cos(i)*cos(i))
//	// And:
//	// a = Re / sqrt( (Tn*n/2/pi - 1) / 1.5 /J2 / (1-4*cos(i)*cos(i)) )
//	double n = PI2 / (Tn - 10);
//	double lasta = 0;
//	double a = pow(GE/n/n,1.0/3.0);
//	double Tntmp = 0;
//	for (int i = 0;i < 35;i++) {
//		n = sqrt(GE / a / a / a);
//		Tntmp = PI2 / n * (1.0 + 1.5*J2*(Re / a)*(Re / a) * (1 - 4 * cos(i)*cos(i)));
//		n = PI2 / (PI2 / n - 0.1*(Tntmp - Tn));
//		a = pow(GE / n / n, 1.0 / 3.0);
//		printf("lasta = %lf  a = %lf\n", lasta, a);
//		lasta = a;
//	}
//
//}


int main(int argc, char* argv[])
{
	cout.precision(12);

	// 标称重复轨道的升交点地理经度表
	RepeatOrbit(10, 157, 98.5);

	//groundtraj();
	
	
	return 0;
}