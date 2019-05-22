#include <OrbitDyn.h>
#include <PerfTimer.h>

using namespace Constant;

string outfilename, lanfilename, orbitfilename;
//#定点地理经度
double Lon = 120;
//#远地点变轨次数
int Nap = 5;
//#测控的地理经度范围
double minLon = 20;
double maxLon = 150;
//#主发动机推力
double F = 490;
//#主发动机比冲
double Isp = 305;

//! GEO轨道转移
void geotrans()
{
	

	fstream fout;
	fout.open(outfilename, ios::out);
	for (int k = 1; k <= Nap; k++)
	{
		fout << "第" << k << "次点火\n";
		fout << "开机时刻 = " << endl;
		fout << "关机时刻 = " << endl;
		fout << "开机时长(s) = " << endl;
		fout << "速度增量(m/s) = " << endl;
		fout << "点火方向赤经 = " << endl;
		fout << "点火方向赤纬 = " << endl;
		fout << "速度增量 = " << endl;
		fout << "燃料消耗 = " << endl;
		fout << "开机点地理经度 = " << endl;
		fout << "开机点地理纬度 = " << endl;
		fout << "开机点卫星质量 = " << endl;
		fout << "关机点地理经度 = " << endl;
		fout << "关机点地理纬度 = " << endl;
		fout << "开机点卫星质量 = " << endl;
		fout << "开机点轨道六根数 = " << endl;
		fout << "关机点轨道六根数 = " << endl;
		fout << "\n";
	}	
	fout.close();
}

void loadcon(string lanfilename) {
	fstream flan(lanfilename, ios::in);
	double lon;
	string name, value;
	int N = 0, P = 0, F = 0;
	double Omega0, u0;
	while (!flan.eof()) {
		//#定点地理经度
		//Lon = 120
		//#远地点变轨次数
		//Nap = 5
		//#测控的地理经度范围
		//minLon = 20
		//maxLon = 150
		//#主发动机推力
		//F = 490
		//#主发动机比冲
		//Isp = 305
		if (ReadLine(&flan, name, value)) {
			if (name == "Lon")
				sscanf(value.c_str(), "%lf", &Lon);
			else if (name == "Nap")
				sscanf(value.c_str(), "%d", &Nap);
			else if (name == "F")
				sscanf(value.c_str(), "%lf", &F); 
			else if (name == "Isp")
				sscanf(value.c_str(), "%lf", &Isp); 
			else if (name == "minLon")
				sscanf(value.c_str(), "%lf", &minLon);
			else if (name == "maxLon")
				sscanf(value.c_str(), "%lf", &maxLon);
		}
	}
}

int main(int argc, char* argv[])
{
	if (argc < 4)
	{
		printf("错误：未指定输入文件!!\n\n");
		printf("请使用以下指令调用：\n");
		printf("  geotrans gto.txt geotranscfg.txt geotransout.txt\n");
		printf("  第一个文件为轨道参数文件\n");
		printf("  第二个文件为同步轨道转移设置文件\n");
		printf("  第三个文件为同步轨道转移控制输出文件\n");
		return 0;
	}

	try {
		orbitfilename = string(argv[1]);
		lanfilename = string(argv[2]);
		outfilename = string(argv[3]);

		loadcon(lanfilename);
		geotrans();
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