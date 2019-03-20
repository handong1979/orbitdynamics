// 单轴太阳帆板指向计算
#include "mandef.h"
using namespace Constant;

string orbitfilename;
CDateTime epoch_start; // 开始时刻
CDateTime epoch_stop; // 停止时间
double step; // 步长

void LoadSAPFile(string filename)
{
	fstream file(filename, ios::in);
	if (!file.is_open())
		throw (string("Can't open file") + filename);
	std::string name, value;
	while (!file.eof()) {		
		if (ReadLine(&file, name, value)) {
			if (name == "step")
				sscanf(value.c_str(), "%lf", &step);
			else if (name == "epoch_stop")
				epoch_stop = string2epoch(value);
			else if (name == "epoch_start")
				epoch_start = string2epoch(value);

		}
	}
}

int main(int argc, char* argv[])
{
	if (argc<4)
	{
		printf("单轴帆板转角计算\n");
		printf("请使用以下指令调用：\n");
		printf("  sap1 orbit.txt sap.txt sap1out.txt\n");
		printf("  第一个文件为轨道参数文件\n");
		printf("  第二个文件为帆板指向控制设置文件\n");
		printf("  第三个文件为计算结果输出文件,各列分别为时间、帆板转角、太阳在轨道坐标系的方向\n");
		return 0;
	}
	
	try {
		orbitfilename = string(argv[1]);

		LoadSAPFile(string(argv[2]));

		fstream fp(string(argv[3]), ios::out);

		CSatellite sat;
		InitSat(sat, orbitfilename);
		sat.Propagate2Epoch(epoch_start);
		vec3 si,vs,vo;
		mat coi;
		double beta;
		for (double t = 0;epoch_start + t <= epoch_stop;t += step) {
			si = Sun::Instance()->GetECI(epoch_start + t);
			vs = si - sat.Pos();
			vs = vs / norm(vs, 2);
			coi = GetCoi(sat.GetOrbitElements());
			vo = coi * vs;
			beta = atan2(-vo(2), vo(0))*DEG;
			if (beta < 0)
				beta += 360;
			fp << epoch_start + t << TAB << beta << TAB << vo.t();

			sat.Propagate(10, step);
		}

		
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
