#include <OrbitDyn.h>
#include "mandef.h"

using namespace Constant;

string outfilename, lanfilename, orbitfilename;
double etx, ety, ee, F;

void geoEWsk()
{
	CSatellite sat;
	InitSat(sat, orbitfilename);

	CDateTime ep0 = sat.CurrentEpoch();
	double Vs = sqrt(GE / 42164.2);

	fstream fp;
	fp.open("EWcosktraj.txt", ios::out);
	double ex, ey;
	bool checke = true;
	do {
		sat.Propagate(600, 600);

		ex = sat.e*cos(sat.Omega + sat.w);
		ey = sat.e*sin(sat.Omega + sat.w);

		double eerrx = ex - etx;
		double eerry = ey - ety;
		double eerr = sqrt(eerrx*eerrx + eerry * eerry);

		// TODO: control
		if (eerr > ee)
		{
			double lsat = sat.Omega + sat.w + sat.f;
			while (lsat > PI2) lsat -= PI2;
			double dv1 = 0.25*Vs*eerr*1000;
			double duration1 = abs(dv1) * sat.Mass() / F;
			double lb = atan2(eerry, eerrx);
			double delta_l = lb - lsat;
			while (delta_l < 0) delta_l += PI2;
			CDateTime t1 = sat.CurrentEpoch() + delta_l / We;

			double dv2 = -dv1;
			CDateTime t2 = t1 + 43200.0;
			double duration2 = abs(dv2) * sat.Mass() / F;


			fstream fout;
			fout.open(outfilename, ios::out);
			fout << "第一次控制开机时刻 = " << t1 + 8*3600.0 << endl;
			fout << "第一次控制开机时长 = " << duration1 << endl;
			fout << "第一次控制速度增量 = " << dv1 << endl;
			fout << "第二次控制开机时刻 = " << t2 + 8 * 3600.0 << endl;
			fout << "第二次控制开机时长 = " << duration2 << endl;
			fout << "第二次控制速度增量 = " << dv2 << endl;
			fout.close();

			checke = false;
		}
		fp << sat.CurrentEpoch() + 8 * 3600.0 << TAB << ex << TAB << ey << endl;
	} while (checke);
	fp.close();
}

void loadlon(string lanfilename) {
	fstream flan(lanfilename, ios::in);
	string name, value;
	while (!flan.eof()) {
		//#目标偏心率矢量X分量
		//ex = 0.00005
		//#目标偏心率矢量Y分量
		//ey = -0.00005
		//# 东西位保推力
		//F = 18
		if (ReadLine(&flan, name, value)) {
			if (name == "ex")
				sscanf(value.c_str(), "%lf", &etx);
			else if (name == "ey")
				sscanf(value.c_str(), "%lf", &ety);
			else if (name == "ee")
				sscanf(value.c_str(), "%lf", &ee);
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
		printf("  geocolocationE geoorbit.txt EWcosk.txt geosk.txt\n");
		printf("  第一个文件为轨道参数文件\n");
		printf("  第二个文件为东西共位偏心率参数设置文件\n");
		printf("  第三个文件为东西位保控制输出文件\n");
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