#include <OrbitDyn.h>
#include "mandef.h"

using namespace Constant;

string outfilename, lanfilename, orbitfilename;
double Lon, Lonerr, F;
vector<double> lonlist;


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

void geoEWsk()
{
	CSatellite sat;
	InitSat(sat,orbitfilename);

	CSpherical lla;

	// 外推，拟合出经度偏差漂移率
	const int ndays = 10;
	CDateTime ep0 = sat.CurrentEpoch();

	double lmda; // 平经度漂移加速度
	double L22 = -0.26056, L31 = 0.12161, L33 = 0.36641;  // 田谐项地理经度
	double Klmd = 0.003209, KD = 0.000707;  // 平经度滤波系数
	// 平经度漂移加速度
	lmda = 0.00170072*sin(2 * (Lon*RAD - L22))
		- 0.00007826*sin(Lon*RAD - L31)
		+ 0.0023526*sin(3 * (Lon*RAD - L33));
	lla = sat.GetLLA();
	double lamdap = lla.Longitude;
	double lmde, D = 0;// .01;
	double Tsk = 600.0 / 86400.0;
	fstream fp; 
	fp.open("EWsktraj.txt", ios::out);
	for (int i = 0;i < ndays*144;i++)
	{
		sat.Propagate(600,600);
		lla = sat.GetLLA();
				
		lamdap = lamdap + D * Tsk + 0.5*lmda*Tsk*Tsk;
		lmde = lla.Longitude - lamdap;
		lamdap = lamdap + Klmd * lmde;
		D = D + KD * lmde;	

		//fp << sat.CurrentEpoch() + 8 * 3600.0 << TAB
		//	<< lamdap << TAB << D << TAB << lla.Longitude << endl;
	}
	//fp.close();
	//return;

	double lamdap0 = lamdap + D*(-ndays) + 0.5*lmda*(-ndays)*(-ndays);
	double D0 = D + lmda * (-ndays);

	//  预报和控制计算
	InitSat(sat,orbitfilename);
	lamdap = lamdap0;
	D = D0;
	double dlr = 0;

	
	bool ewsk = true;
	int newsk = 0;
	int cnt = 0;
	bool readymaneuver = false;
	CDateTime EWTime;
	double Da, dVx, Tx;
	do {
		sat.Propagate(600, 600);
		lla = sat.GetLLA();

		lamdap = lamdap + D * Tsk + 0.5*lmda*Tsk*Tsk;
		lmde = lla.Longitude - lamdap;
		lamdap = lamdap + Klmd * lmde;
		D = D + KD * lmde;

		cnt++;

		dlr = lamdap - Lon;

		//cout << sat.CurrentEpoch() << "\t经度:" << lla.Longitude
		//	<< "\t经度偏差:" << lla.Longitude - Lon
		//	<< "\t平经度:" << lamdap
		//	<< "\t平经度漂移率:" << D << endl;

		// TODO: control
		if (dlr < -Lonerr && !readymaneuver && cnt>144*4)
		{
			//ewsk = false;
			Da = sqrt(fabs(2.0*lmda*(Lonerr - dlr)));
			dVx = (D - Da) / 0.352;
			Tx = fabs(dVx)*sat.Mass() / F;
			
			double ex = sat.e*cos(sat.Omega + sat.w);
			double ey = sat.e*sin(sat.Omega + sat.w);
			double dalf = atan2(ey, ex) - mod(sat.u + sat.Omega + 4 * PI, 2 * PI);
			if (dalf < 0)
				dalf += 2 * PI;
			EWTime = sat.CurrentEpoch() + dalf / We - Tx / 2;
			readymaneuver = true;

			fstream fout;
			fout.open(outfilename, ios::out);
			fout << "开机时刻 = " << EWTime + 8*3600.0 << endl;
			fout << "开机时长(s) = " << Tx << endl;
			fout << "速度增量(m/s) = " << dVx << endl;
			fout << "位保前平经度漂移率(度/天） = " << D << endl;
			fout << "位保后平经度漂移率(度/天） = " << Da << endl;
			fout.close();
		}
		if (readymaneuver && sat.CurrentEpoch() > EWTime)
		{
			readymaneuver = false;
			cnt = 0;
			vec3 dv;
			dv(0) = dVx / 1000.0;
			dv(1) = 0;
			dv(2) = 0;
			sat.ImpluseManeuver(dv / 1000.0);
			D = Da;
			if(newsk++>10)
				ewsk = false;
		}

		fp << sat.CurrentEpoch() + 8 * 3600.0 << TAB 
			<< lamdap << TAB << D << TAB << lla.Longitude << endl;
	} while (ewsk);
	fp.close();
}

void loadlon(string lanfilename) {
	fstream flan(lanfilename, ios::in);
	string name, value;
	while (!flan.eof()) {
		//# 定点地理经度
		//Lon = 120
		//# 地理经度保持范围
		//Lonerr = 0.1
		//# 东西位保推力
		//F = 18
		if (ReadLine(&flan, name, value)) {
			if (name == "Lon")
				sscanf(value.c_str(), "%lf", &Lon);
			else if (name == "Lonerr")
				sscanf(value.c_str(), "%lf", &Lonerr);
			else if (name == "F")
				sscanf(value.c_str(), "%lf", &F);
		}
	}
}

int main(int argc, char* argv[])
{
	if (argc < 4)
	{
		printf("错误：未指定输入文件!!\n\n");
		printf("请使用以下指令调用：\n");
		printf("  geoEWsk geoorbit.txt EWsk.txt geosk.txt\n");
		printf("  第一个文件为轨道参数文件\n");
		printf("  第二个文件为东西位保设置文件\n");
		printf("  第三个文件为东西位保控制输出文件\n");
		printf("  运行结束后会生成一个EWsktraj.txt文件，保存画曲线的数据，包括时间、平经度、平经度漂移率\n");
		return 0;
	}

	try {
		orbitfilename = string(argv[1]);
		lanfilename = string(argv[2]);
		outfilename = string(argv[3]);

		loadlon(lanfilename);
		geoEWsk();
	}
	catch (BaseException& e)
	{
		cerr << e.GetFullMessage() << endl;
	}
	catch (exception* e)
	{
		cerr << ((BaseException*)e)->what() << endl;
	}
	catch (string * e)
	{
		cerr << *e << endl;
	}



	
	
	
	return 0;
}