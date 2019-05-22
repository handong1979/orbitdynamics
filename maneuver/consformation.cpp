#include <OrbitDyn.h>
#include <PerfTimer.h>

using namespace Constant;

string outfilename, lanfilename, orbitfilename;
vector<double> lonlist;

int N = 0, P = 0, F = 0;
double Omega0, u0;

//! 星座位置保持
void consform()
{
	
}

//Walker星座的构型码为：N/P/F（卫星数目/轨道平面数/相位因子)
void Walker(int N, int P, int F,double Omega0,double u0)
{
	int S = N / P;
	FILE *fp;
	fp = fopen("walker.txt", "w");
	double Omega, u;
	for (int Pm = 1; Pm <= P; Pm++)
	{
		Omega = Omega0 + 360 / P * (Pm - 1);
		for (int Nm = 1; Nm <= S; Nm++)
		{
			u = u0 + 360 / S * (Nm - 1) + 360 / N * F*(Pm - 1);
			fprintf(fp, "%lf   %lf\n", Omega, u);
		}
	}
	fclose(fp);
}

void loadcon(string lanfilename) {
	fstream flan(lanfilename, ios::in);
	double lon;
	string name, value;
	while (!flan.eof()) {
		//# 卫星数目
		//N = 24
		//# 轨道平面数
		//P = 3
		//# 相位因子
		//F = 2
		//# 第一个轨道面的升交点赤经
		//Omega0 = 10
		//# 第一颗星的纬度幅角
		//u0 = 0
		if (ReadLine(&flan, name, value)) {
			if (name == "N")
				sscanf(value.c_str(), "%d", &N);
			else if (name == "P")
				sscanf(value.c_str(), "%d", &P);
			else if (name == "F")
				sscanf(value.c_str(), "%d", &F); 
			else if (name == "Omega0")
				sscanf(value.c_str(), "%lf", &Omega0);
			else if (name == "u0")
				sscanf(value.c_str(), "%lf", &u0);
		}
	}
	if (N != 0 && P != 0)
	{
		Walker(N,P,F,Omega0,u0);
	}
}

int main(int argc, char* argv[])
{
	if (argc < 4)
	{
		if (argc == 2)
		{
			loadcon(argv[1]);
			return 1;
		}

		printf("错误：未指定输入文件!!\n\n");
		printf("请使用以下指令调用：\n");
		printf("  consformation csorbit.txt conform.txt csout.txt\n");
		printf("  第一个文件为轨道参数文件\n");
		printf("  第二个文件为标称升交点地理经度文件\n");
		printf("  第三个文件为轨道维持精度设置文件\n");
		printf("  第四个文件为轨道维持控制输出文件\n");
		printf(" 用法2： 生成一个星座的标称升交点赤经和纬度幅角表\n");
		printf(" consformation conform.txt\n");
		printf(" 根据文件中的回归轨道参数，生成一个walker.txt文件，保存各个圈次的标称升交点地理经度\n");
		return 0;
	}

	try {
		orbitfilename = string(argv[1]);
		lanfilename = string(argv[2]);
		outfilename = string(argv[3]);

		loadcon(lanfilename);
		consform();
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