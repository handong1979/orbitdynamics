#include "stdio.h"
#include "process.h"

void main()
{
	// 时间(世界时)
	char* Epoch = "2019-01-01 00:00:00";
	// 近地点高度(km)
	double Hp  =   17120;
	// 远地点高度(km)
	double Ha  =   35786;
	// 倾角(度)
	double incl  =  4.8;
	// 初始质量(kg)
	double mass0 = 1600;
	// 轨控推力(N)
	double f = 0.48;
	// 比冲(s)
	double isp = 2450;
	// 升交点赤经(度)
	double Omega = 0;
	// 近地点幅角(度)
	double w = 180;
	// 平近点角(度)
	double M = 0;

	// 写配置文件
	FILE* fp = fopen("in.txt","w");
	fprintf(fp,"Epoch = %s\n",Epoch);
	fprintf(fp,"Hp = %lf\n",Hp);
	fprintf(fp,"Ha = %lf\n",Ha);
	fprintf(fp,"incl = %lf\n",incl);
	fprintf(fp,"f = %lf\n",f);
	fprintf(fp,"isp = %lf\n",isp);
	fprintf(fp,"Omega = %lf\n",Omega);
	fprintf(fp,"w = %lf\n",w);
	fprintf(fp,"M = %lf\n",M);
	fclose(fp);

	// 调用优化程序
	printf("start lt.exe ...\n");
	//_execl("lt.exe","lt.exe","in.txt");
	system("lt in.txt");

	//读取计算结果
	FILE* result = fopen("transparam.txt","r");
	double tof,dm,t1,t2,t3,dm1,dm2,dm3,PHI1,PHI2;
	char* c1 = "cccccccc";
	char* c2 = "=";
	fscanf(result,"%s %s %lf",&c1,&c2,&PHI1);
	fscanf(result,"%s %s %lf",&c1,&c2,&PHI2);
	fscanf(result,"%s %s %lf",&c1,&c2,&tof);
	fscanf(result,"%s %s %lf",&c1,&c2,&dm);
	fscanf(result,"%s %s %lf",&c1,&c2,&t1);
	fscanf(result,"%s %s %lf",&c1,&c2,&t2);
	fscanf(result,"%s %s %lf",&c1,&c2,&t3);
	fscanf(result,"%s %s %lf",&c1,&c2,&dm1);
	fscanf(result,"%s %s %lf",&c1,&c2,&dm2);
	fscanf(result,"%s %s %lf",&c1,&c2,&dm3);
	fclose(result);
}