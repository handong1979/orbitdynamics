// semi-major axis maneuver
#include "mandef.h"
using namespace Constant;

string orbitfilename;
double ta;     // 目标轨道半长轴
CDateTime aepoch; // 半长轴机动时刻
double ti;     // 目标倾角

// out:
CDateTime iepoch; // 倾角机动时间

//! 近圆轨道的半长轴机动
double sma_nearcircle()
{
	CSatellite sat;
	InitSat(sat, orbitfilename);
	sat.Propagate2Epoch(aepoch - 3600.0*8.0);
	Kepler km = Mean(sat.GetOrbitElements());
	double n = sqrt(GE / km.a / km.a / km.a);
	return n / 2 * (ta - km.a) * 1000; // m/s速度增量
}

double i_raan()
{
	CSatellite sat;
	InitSat(sat, orbitfilename);
	if (sat.u > 0.5*PI && sat.u < 1.5*PI)
	{
		sat.Propagate2DescendingNode();
		iepoch = sat.CurrentEpoch();
		return sat.v * (ti * RAD - sat.i) * 1000;
	}
	else
	{
		sat.Propagate2AscendingNode();
		iepoch = sat.CurrentEpoch();
		return sat.v * (sat.i - ti * RAD) * 1000;
	}
}


void LoadLEOCAPFile(string filename)
{
	fstream file(filename, ios::in);
	if (!file.is_open())
		throw (string("Can't open file") + filename);
	std::string name, value;
	while (!file.eof()) {
		if (ReadLine(&file, name, value)) {
			if (name == "Targeta")
				sscanf(value.c_str(), "%lf", &ta);
			else if (name == "Targeti")
				sscanf(value.c_str(), "%lf", &ti);
			else if (name == "aepoch")
				aepoch = string2epoch(value);

		}
	}
}

int main(int argc, char* argv[])
{
	if (argc<4)
	{
		printf("错误：未指定输入文件!!\n\n");
		printf("请使用以下指令调用：\n");
		printf("  leocap orbit.txt leocap.txt out.txt\n");
		printf("  第一个文件为轨道参数文件\n");
		printf("  第二个文件为控制目标设置文件\n");
		printf("  第三个文件为计算结果输出文件\n");		
		return 0;
	}
	
	try {
		orbitfilename = string(argv[1]);

		LoadLEOCAPFile(string(argv[2]));		

		double dva = sma_nearcircle();
		double dvi = i_raan();

		fstream fp(string(argv[3]), ios::out);
		fp << " 半长轴调整所需速度增量 = " << dva << endl;
		fp << " 倾角调整变轨时间 = " << iepoch + 3600 * 8.0 << endl;
		fp << " 倾角调整速度增量 = " << dvi << endl;
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
