// semi-major axis maneuver
#include "mandef.h"
using namespace Constant;

string orbitfilename;

int main(int argc, char* argv[])
{
	if (argc<4)
	{
		printf("错误：未指定输入或输入不完整!!\n\n");
		printf("请使用以下指令调用：\n");
		printf("  geodeorbit geoorbit.txt 300 20 geodeout.txt\n");
		printf("  第一个文件为轨道参数文件\n");
		printf("  第二个参数为抬高的轨道高度，单位为km\n");
		printf("  第三个参数为离轨的推力大小，单位为N\n"); 
		printf("  第四个文件为计算结果输出文件\n");
		return 0;
	}
	
	try {
		orbitfilename = string(argv[1]);
		double dh = atof(argv[2]);
		double F = atof(argv[3]);

		CSatellite sat;
		InitSat(sat, orbitfilename);
		
		// 外推到远地点或近地点
		double dt;
		if (sat.f < PI)
			dt = (PI - sat.f) / sat.n;
		else
			dt = (PI2 - sat.f) / sat.n;
		sat.Propagate(60, dt);
		CDateTime ep = sat.CurrentEpoch();

		double a1 = sat.a;
		double a2 = sat.a + dh;
		double at = ( a1 + a2 ) / 2;
		double dv1 = (sqrt(GE*(2 / a1 - 1 / at)) - sqrt(GE / a1)) * 1000;
		double dv2 = (sqrt(GE / a2) - sqrt(GE*(2 / a2 - 1 / at))) * 1000;

		fstream fp(string(argv[4]), ios::out);
		fp << "第一次控制开机时刻 " << ep + 8 * 3600.0 << endl;
		fp << "第一次控制开机时长 " << dv1 * sat.Mass() / 20 << endl;
		fp << "第一次控制速度增量 " << dv1 << endl;
		fp << "第二次控制开机时刻 " << ep + 20 * 3600.0 << endl;
		fp << "第二次控制开机时长 " << dv2 * sat.Mass() / 20 << endl;
		fp << "第二次控制速度增量 " << dv2 << endl;
		fp.close();

	}
	catch (BaseException& e)
	{
		cerr << e.GetFullMessage() << endl;
	}
	catch (exception* e)
	{
		cerr << ((BaseException*)e)->what() << endl;
	}
	
	return 0;
}
