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
		printf("  leodeorbit orbit.txt 12.34 out.txt\n");
		printf("  第一个文件为轨道参数文件\n");
		printf("  第二个参数为卫星剩余的速度增量，单位为m/s\n");
		printf("  第三个文件为计算结果输出文件\n");		
		return 0;
	}
	
	try {
		orbitfilename = string(argv[1]);
		double dv = atof(argv[2]);		

		CSatellite sat;
		InitSat(sat, orbitfilename);
		
		// 如果是椭圆轨道，外推到远地点
		if(sat.e>0.0001)
			sat.Propagate2Apogee();
		CDateTime ep = sat.CurrentEpoch();

		// 离轨后远地点速度
		double va = sat.v - dv/1000.0;
		// 远地点高度
		double ra = sat.r;
		// 根据 v^2/miu = 2/r - 1/a 计算半长轴
		double a = 1 / (2 / ra - va * va / GE);
		double hp = (a*2.0 - ra) - Re;
		
		fstream fp(string(argv[3]), ios::out);
		fp << " 近地点高度 = " << hp << endl;
		fp << " 远地点高度 = " << ra - Re << endl;
		fp << " 远地点时刻刻 = " << ep + 3600*8.0 << endl;		
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
