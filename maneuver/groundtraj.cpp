#include <OrbitDyn.h>
#include <PerfTimer.h>

using namespace Constant;
// fit x-y as a palabolic:
// y = a + b*x + c*x^2
void parabolafit(double* x, double* y, int n, double& a, double& b, double& c)
{
	double matrix[3][4], ratio, aa;
	double sum_x = 0, sum_y = 0, sum_x2 = 0, sum_x3 = 0, sum_x4 = 0, sum_xy = 0, sum_x2y = 0;
	int i, j, k;
	for (i = 0; i < n; i++) {
		sum_x += x[i];
		sum_y += y[i];
		sum_x2 += pow(x[i], 2);
		sum_x3 += pow(x[i], 3);
		sum_x4 += pow(x[i], 4);
		sum_xy += x[i] * y[i];
		sum_x2y += pow(x[i], 2) * y[i];
	}
	matrix[0][0] = n;
	matrix[0][1] = sum_x;
	matrix[0][2] = sum_x2;
	matrix[0][3] = sum_y;
	matrix[1][0] = sum_x;
	matrix[1][1] = sum_x2;
	matrix[1][2] = sum_x3;
	matrix[1][3] = sum_xy;
	matrix[2][0] = sum_x2;
	matrix[2][1] = sum_x3;
	matrix[2][2] = sum_x4;
	matrix[2][3] = sum_x2y;
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			if (i != j) {
				ratio = matrix[j][i] / matrix[i][i];
				for (k = 0; k < 4; k++) {
					matrix[j][k] -= ratio * matrix[i][k];
				}
			}
		}
	}
	for (i = 0; i < 3; i++) {
		aa = matrix[i][i];
		for (j = 0; j < 4; j++) {
			matrix[i][j] /= aa;
		}
	}
	a = matrix[0][3];
	b = matrix[1][3];
	c = matrix[2][3];
}

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
	Kepler tm = Mean(Kepler(elem));

	cout << "Chaser's Mean:" << chaser.MedianElement() << endl;
	cout << "Target's Mean:" << target.MedianElement() << endl << endl;

	CSpherical lla, llat;
	FILE* flla;
	flla = fopen("groundtraj.txt", "w");

	// 外推，拟合出经度偏差漂移率和漂移加速度
	const int nrev = 200;
	double dlarray[nrev] = { 0 }, time[nrev] = { 0 }, da[nrev] = { 0 };
	CDateTime ep0 = chaser.CurrentEpoch();
	for (int i = 0;i < nrev;i++)
	{
		chaser.Propagate2EquatorAscNode();
		lla = chaser.GetLLA();
		
		target.Propagate2EquatorAscNode();		
		llat = target.GetLLA();

		double dl = lla.Longitude - llat.Longitude;
		if (dl > 350) dl -= 360;
		else if (dl < -350) dl += 360;
		dl = dl * RAD * 6378;

		dlarray[i] = dl;
		time[i] = chaser.CurrentEpoch() - ep0;
		Kepler cm = Mean(chaser.GetOrbitElements());
		da[i] = cm.a - tm.a;

		cout << "Chaser's Lon:" << lla.Longitude 
			<< " Lat: " << lla.Latitude
			<< "\tLon err:" << dl*RAD*6378 << endl;
		fprintf(flla,"%e\t%e\t%e\n",time[i], dlarray[i],da[i]);
	}
	fclose(flla);

	// 拟合
	double L0, dotL, AccL;
	parabolafit(time, dlarray, nrev, L0, dotL, AccL);
	if(1)
	{
		printf("L0 = %lf\n", L0);  // init lon error
		printf("dL = %lf\n", dotL * 86400 );  // lon error rate
		printf("ddL = %lf\n", AccL * 86400 * 86400 ); // lon error accelerate
	}

	// 预报和控制计算
	chaser.Initialize(CDateTime(2016, 8, 13, 12, 0, 0.0), elem);
	target.Initialize(CDateTime(2016, 8, 13, 12, 0, 0.0), elem);
	double dLANmax = 10, dlr = 0;
	do {
		chaser.Propagate2EquatorAscNode();
		lla = chaser.GetLLA();

		target.Propagate2EquatorAscNode();
		llat = target.GetLLA();

		double dlr = lla.Longitude - llat.Longitude;
		if (dlr > 350) dlr -= 360;
		else if (dlr < -350) dlr += 360;
		dlr = dlr * RAD * 6378;

		// control
		if (dlr < -dLANmax)
		{
			// 超出西边界
		}
		else if (dlr > dLANmax)
		{
			// 超出东边界
		}
	} while (fabs(dlr) * 2 < dLANmax);
	
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
	//RepeatOrbit(10, 157, 98.5);

	groundtraj();
	
	
	return 0;
}