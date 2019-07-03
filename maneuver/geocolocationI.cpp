#include <OrbitDyn.h>
#include "mandef.h"

using namespace Constant;

string outfilename, lanfilename, orbitfilename;
double  Ti,To,Laterr, F;

Kepler ECIkp2ECFkp(CDateTime ep, vec3 p, vec3 v)
{
	mat33 cfi = ECI_ECF(ep);
	vec3 pf, vf;
	pf = cfi * p;
	vf = cfi * v;
	return Cartesian_Kepler(pf, vf);
}

void geoEWsk()
{
	CSatellite sat;
	InitSat(sat, orbitfilename);

	// 地固系轨道根数
	Kepler kf;
	// 地固系倾角矢量
	double ix, iy;

	//  预报和控制计算
	fstream fp;
	fp.open("NSsktraj.txt", ios::out);
	double di;
	int i = 0;
	double itx, ity, ixe, iye;
	bool nssk = true;
	do {
		sat.Propagate(600, 600);
		kf = ECIkp2ECFkp(sat.CurrentEpoch(), sat.Pos(), sat.Vel());

		ix = kf.i * DEG * cos(kf.o);
		iy = kf.i * DEG * sin(kf.o);
		itx = Ti * cos(To*RAD);
		ity = Ti * sin(To*RAD);
		ixe = ix - itx;
		iye = iy - ity;
		di = sqrt(ixe * ixe + iye * iye);

		if (mod(i++, 144) == 0) {
			cout << sat.CurrentEpoch() + 8 * 3600.0 << "\t倾角:" << kf.i*DEG
				<< "\t倾角矢量X:" << ix
				<< "\t倾角矢量Y:" << iy << endl;
		}

		// control
		if (di > Laterr)
		{
			double T = (sat.CurrentEpoch().GetMJD() - MJD2000) / 36525;
			double orb_Omega_l = 2.182439f - 33.757f*T;
			double Omegaix = -3.5e-4*sin(orb_Omega_l);
			double Omegaiy = 22.79e-4 + 2.59e-4*cos(orb_Omega_l);
			double Omegat = atan2(Omegaiy, Omegaix) + PI;
			double it = Laterr;
			double dix = ixe - it * cos(Omegat);
			double diy = iye - it * sin(Omegat);
			double dVy = 3074.6*sqrt(dix*dix + diy * diy)*RAD;
			double NSFireDuration = dVy * sat.Mass() / F;

			double dalf = atan2(diy, dix) - mod(sat.u + sat.Omega + 4 * PI, 2 * PI);
			if (dalf < 0)
				dalf += PI2;
			int autoSKbTDS;
			if (dalf < 2.3562 || dalf > 5.4978)
			{
				autoSKbTDS = 0xF0;
			}
			else {
				dalf += PI;
				if (dalf > 2 * PI)
					dalf -= 2 * PI;
				autoSKbTDS = 0x00;
			}
			CDateTime NSTime = sat.CurrentEpoch() + dalf / We - NSFireDuration / 2;

			fstream fout;
			fout.open(outfilename, ios::out);
			fout << "开机时刻 = " << NSTime << endl;
			fout << "开机时长(s) = " << NSFireDuration << endl;
			fout << "速度增量(m/s) = " << dVy << endl;
			fout << "位保方向 = ";
			if (autoSKbTDS == 0xF0)
			{
				fout << "向北位保" << endl;
			}
			else
			{
				fout << "向南位保" << endl;
			}
			fout << "位保前倾角(度) = " << kf.i << endl;
			fout << "位保前升交点赤经(度) = " << kf.o*DEG << endl;

			double ifx = Ti * cos(To*RAD) + it * cos(Omegat);
			double ify = Ti * sin(To*RAD) + it * sin(Omegat);

			fout << "位保后倾角(度) = " << sqrt(ifx*ifx+ify*ify) << endl;
			fout << "位保后升交点赤经(度) = " << atan2(ify,ifx) * DEG << endl;
			fout.close();

			nssk = false;
			// 超出边界
		}
		fp << sat.CurrentEpoch() + 8 * 3600.0 << TAB << ix << TAB << iy << endl;
	} while (nssk);
	fp.close();
}

void loadNSsk(string lanfilename) {
	fstream flan(lanfilename, ios::in);
	string name, value;
	while (!flan.eof()) {
		//#目标倾角
		//Ti = 0.3
		//#目标升交点赤经
		//To = 40
		//# 地理纬度保持范围
		//Laterr = 0.1
		//# 南北位保推力
		//F = 19
		if (ReadLine(&flan, name, value)) {
			if (name == "Laterr")
				sscanf(value.c_str(), "%lf", &Laterr);
			else if (name == "F")
				sscanf(value.c_str(), "%lf", &F);
			else if (name == "Ti")
				sscanf(value.c_str(), "%lf", &Ti);
			else if (name == "To")
				sscanf(value.c_str(), "%lf", &To);
		}
	}
}

int main(int argc, char* argv[])
{
	if (argc < 4)
	{
		printf("错误：未指定输入文件!!\n\n");
		printf("请使用以下指令调用：\n");
		printf("  geocolocationI geoorbit.txt NScosk.txt geocosk.txt\n");
		printf("  第一个文件为轨道参数文件\n");
		printf("  第二个文件为南北位保设置文件\n");
		printf("  第三个文件为南北位保控制输出文件\n");
		printf("  运行结束后会生成一个NSsktraj.txt文件，保存画曲线的数据，包括时间、倾角矢量x、倾角矢量y\n");
		return 0;
	}

	try {
		orbitfilename = string(argv[1]);
		lanfilename = string(argv[2]);
		outfilename = string(argv[3]);

		loadNSsk(lanfilename);
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