#include <OrbitDyn.h>
#include <PerfTimer.h>

using namespace Constant;

//! 测试轨道机动
void hohmantrans(double a1,double a2,double& dv1,double& dv2)
{
	double at = (a1 + a2) / 2;
	dv1 = sqrt(GE*(2 / a1 - 1 / at)) - sqrt(GE / a1);
	dv2 = sqrt(GE / a2) - sqrt(GE*(2 / a2 - 1 / at));
}

int main(int argc, char* argv[])
{	
	if (argc<3)
	{
		printf("错误：输入参数不足!!\n\n");
		printf("请使用以下指令调用：\n");
		printf("  leotrans a1 a2 out.txt \n");
		printf("  第一个参数为初始轨道半长轴\n");
		printf("  第二个参数为目标轨道半长轴\n");
		printf("  第三个文件名为计算结果输出文件\n");
		return 0;
	}

	double a1, a2;
	double dv1, dv2;
	sscanf(argv[1], "%lf", &a1);
	sscanf(argv[2], "%lf", &a2);
	hohmantrans(a1, a2, dv1, dv2);
	FILE* fp;
	fp = fopen(argv[3], "w");
	fprintf(fp, "dv1 = %lf\n", dv1 * 1000);
	fprintf(fp, "dv2 = %lf\n", dv2 * 1000);
	fclose(fp);

	return 0;
}