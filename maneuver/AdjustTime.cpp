// adjusttime.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

// satellite time adjust

#include "OrbitDyn.h"

using namespace Constant;

CFacility fac(120,40,0.2);

typedef struct{
	CDateTime epoch;
	Kepler kp;
	double AirDragArea;
	double Mass;
}OrbitParam;
OrbitParam op;

double delaysat;
double delaygrd;
double GLon,GLat,GAlt;

typedef struct {
	CDateTime ts;
	CDateTime re;
	double dt;
} timepair;
vector<timepair> tl;

CDateTime string2epoch(string s)
{
	int y, m, d, h, min;
	double sec;
	sscanf_s(s.c_str(), "%d-%d-%d %d:%d:%lf", &y, &m, &d, &h, &min, &sec);
	CDateTime t(y, m, d, h, min, sec);
	return t;
}

void LoadOrbitFile(string filename)
{
	fstream file(filename,ios::in);
	if(!file.is_open())
		throw (string("Can't open file") + filename);
	std::string name,value;
	while(!file.eof()){
		//Semi_major_axis   =     1932.652806175639
		//Eccentricity      =    0.003606090476565559
		//Inclination       =     88.45177560605939
		//Ra_of_asc_node    =     265.2090045338037
		//Arg_of_pericenter =     120.0
		//Mean_anomaly      =     94.71685115019184
		if(ReadLine(&file,name,value))    {
			if(name == "Semi_major_axis")
				sscanf(value.c_str(),"%lf",&op.kp.a);
			else if(name == "Eccentricity")
				sscanf(value.c_str(),"%lf",&op.kp.e);
			else if(name == "Inclination")
				sscanf(value.c_str(),"%lf",&op.kp.i);
			else if(name == "Ra_of_asc_node")
				sscanf(value.c_str(),"%lf",&op.kp.o);
			else if(name == "Arg_of_pericenter")
				sscanf(value.c_str(),"%lf",&op.kp.w);
			else if(name == "Mean_anomaly")
				sscanf(value.c_str(),"%lf",&op.kp.M);
			else if(name == "AirDragArea")
				sscanf(value.c_str(),"%lf",&op.AirDragArea);
			else if(name == "Mass")
				sscanf(value.c_str(),"%lf",&op.Mass);
			else if(name == "Epoch")
				op.epoch = string2epoch(value);
		}
	}
}

void LoadFacFile(string filename)
{
	fstream file(filename,ios::in);
	if(!file.is_open())
		throw (string("Can't open file") + filename);
	std::string name,value;
	while(!file.eof()){
		//#星上时延
		//Satdelay  = 100
		//#地面时延
		//Grounddelay = 100
		//# 地面站位置信息
		//GLon = 120
		//GLat = 40
		//GAlt = 0.2
		if( ReadLine(&file,name,value) ) {
			if(name == "Satdelay")
			{
				sscanf(value.c_str(),"%lf",&delaysat);
				delaysat /= 1000;
			}
			else if(name == "Grounddelay")
			{
				sscanf(value.c_str(),"%lf",&delaygrd);
				delaygrd /= 1000;
			}
			else if(name == "GLon")
				sscanf(value.c_str(),"%lf",&GLon);
			else if(name == "GLat")
				sscanf(value.c_str(),"%lf",&GLat);
			else if(name == "GAlt")
				sscanf(value.c_str(),"%lf",&GAlt);
		}
	}
	fac.SetGeodetic(GLon, GLat, GAlt);
}

void LoadTimeFile(string filename)
{
	fstream fdat(filename,ios::in);
	if(!fdat.is_open())
		throw string("Can't open adjust dat file");
	string line;
	timepair tp;
	string date1,time1,date2,time2;
	while(!fdat.eof()){
		//#遥测接收时间                                     遥测帧时间
		//2018-10-01 00:30:22.000                  2018-10-01 00:30:20.000
		//2018-10-01 00:30:23.000                  2018-10-01 00:30:21.000
		if( GetLine(&fdat,line) ) {
			stringstream ss(line);
			ss >> date1 >> time1 >> date2 >> time2;
			tp.ts = string2epoch(date1 + " " + time1);
			tp.re = string2epoch(date2 + " " + time2);
			tl.push_back(tp);
		}
	}
}

double rou(CSatellite &sat,CFacility &fac)
{
	vec3 sp,sv;
	sat.GetECF(sp, sv);
	vec3 fp = fac.ECFPosition;
	return norm(sp-fp,2);
}

//! 校时量的计算
double adjusttime()
{
	CSatellite sat;
	sat.Initialize(op.epoch - 3600.0*8.0, op.kp);
	sat.Mass0 = op.Mass;
	sat.AirDragArea = op.AirDragArea;
	sat.Propagate2Epoch(tl[0].ts);
	int n = tl.size();
	double dt,lastdt;
	double step,sfr;
	// 迭代每一个时间点的传输时延,求星时差
	for(unsigned int i=0;i<n;i++)
	{
		dt = lastdt = 0;
		sat.Propagate2Epoch(tl[i].ts + delaysat);
		do{
			lastdt = dt;
			sfr = rou(sat,fac)/LightVel;
			dt = tl[i].re - (tl[i].ts + delaysat + delaygrd + sfr);
			step = (tl[i].ts + dt + delaysat) - sat.CurrentEpoch();
			if(step>0)
				sat.Propagate(1, step);
			else
				sat.PropagateBackward(-1, step);
		}while(fabs(lastdt-dt)>1e-6);
		tl[i].dt = dt;
	}

	// 求均值、初值和斜率
	double sum = 0;
	for(unsigned int i=0;i<n;i++)
	{
		sum += tl[i].dt;
	}
	double meandt = sum/n; // 集中校时
	double avgdt = 0;
	if(n>1)
		avgdt = (tl[n-1].dt - tl[0].dt)/(tl[n-1].ts - tl[0].ts)*3600;

	fstream ff;
	ff.open("adjusttime.txt",ios::out);
	ff << "#集中校时时差值\n";
	ff << "FocusAdjustTime = " << meandt << "\n";
	ff << "#均匀校时周期\n";
	ff << "UniformAdjustTimePeriod = " << 3600 << "\n";
	ff << "#均匀校时校正量\n";
	ff << "UniformAdjustTime = " << avgdt << endl;
	ff.close();

	return 1;
}

void maketestdata(double timebias,double timerate)
{
	CSatellite sat;
	sat.Initialize(op.epoch - 3600.0*8.0, op.kp);
	sat.Mass0 = op.Mass;
	sat.AirDragArea = op.AirDragArea;
	double step = 60;
	// 迭代每一个时间点的传输时延,求星时差
	double tanstime = 0;
	fstream tlf;
	tlf.open("timelist.txt",ios::out);
	int n = 200;

	CDateTime TmTime; // 遥测打包时间
	for(unsigned int i=0; i<n; i++) {
		TmTime = sat.CurrentEpoch();
		tlf << TmTime + (timebias+timerate*i*step) << TAB;
		sat.Propagate(delaysat, delaysat);
		tanstime = rou(sat,fac)/LightVel;
		tlf << TmTime + delaysat + tanstime + delaygrd << TAB << rou(sat,fac) << endl;
		sat.Propagate(step-delaysat,step-delaysat);
	}
	tlf.close();
}


int _tmain(int argc, char* argv[])
{
	fstream ff;
	ff.open("adjusttime.txt",ios::out);
	ff.close();

	if(argc<4)
	{
		printf("错误：未指定输入文件!!\n\n");
		printf("请使用以下指令调用：\n");
		printf("  adjusttime orbit.txt fac.txt timelist.txt\n");
		printf("  第一个文件为轨道参数文件\n");
		printf("  第二个文件为地面站参数和时延参数文件\n");
		printf("  第三个文件为时标文件\n");
		printf(" 输出文件为adjusttime.txt\n");
		return 0;
	}

	try{
		LoadOrbitFile(string(argv[1]));
		LoadFacFile(string(argv[2]));
		LoadTimeFile(string(argv[3]));

		//LoadOrbitFile("orbit.txt");
		//LoadFacFile("fac.txt");

		//double timebias = -0.02; // 卫星钟差
		//double timerate = -0.001/3600; //卫星钟漂
		//maketestdata(timebias,timerate);
		//LoadTimeFile("timelist.txt");

		adjusttime();
	}
	catch(BaseException& e)
	{
		cerr << e.GetFullMessage() << endl;
	}
 	catch(exception* e)
 	{
 		cerr << ((BaseException*)e)->what() << endl;
 	}

	return 1;
}

