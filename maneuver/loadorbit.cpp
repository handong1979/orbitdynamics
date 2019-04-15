#include "mandef.h"

CDateTime string2epoch(string s)
{
	int y, m, d, h, min;
	double sec;
	sscanf_s(s.c_str(), "%d-%d-%d %d:%d:%lf", &y, &m, &d, &h, &min, &sec);
	CDateTime t(y, m, d, h, min, sec);
	return t;
}

OrbitParam LoadOrbitFile(string filename)
{
	fstream file(filename,ios::in);
	if(!file.is_open())
		throw (string("Can't open file") + filename);
	std::string name,value;
	OrbitParam op;
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
			else if (name == "Epoch")
			{
				op.epoch = string2epoch(value);
				//cout << "Epoch = " << op.epoch << endl;
			}
		}
	}
	return op;
}

void InitSat(CSatellite& sat, string filename)
{	
	OrbitParam op;
	op = LoadOrbitFile(filename);	
	op.kp.i *= RAD;
	op.kp.o *= RAD;
	op.kp.w *= RAD;
	op.kp.M *= RAD;
	sat.Initialize(op.epoch - 3600.0*8.0, op.kp);
	sat.Mass0 = op.Mass;
	sat.AirDragArea = op.AirDragArea;
}

//! 近圆轨道的半长轴机动
double sma_nearcircle(double a, double deltaa)
{
	double n = sqrt(GE / a / a / a);
	return n / 2 * deltaa;
}

double sma_ellipse(double a, double e, double f, double deltaa)
{
	double r = a * (1 - e * e) / (1 + e * cos(f));
	double v1 = sqrt(GE*(2 / r - 1 / a));
	double v2 = sqrt(GE*(2 / r - 1 / (a + deltaa)));
	return v2 - v1; // TODO: 有俯仰角？
}

//! LEO轨道倾角捕获
// 在升交点改变倾角，近圆轨道，小修正量
// a 轨道半长轴，km
// i0 初始轨道倾角，度
// it 目标轨道倾角，度
// ascordes = 1 在升交点
// ascordes = 0 在降交点
// 返回：轨道Y方向的速度增量m/s
double leo_inclination(double a, double i0, double it, int ascordes)
{
	double v = sqrt(GE / a);
	if (ascordes == 1) // 升交点
		return v * (i0 - it)*RAD * 1000;
	else
		return v * (it - i0)*RAD * 1000;
}

//! LEO轨道倾角捕获
// 在升降交点改变倾角，近圆轨道，大倾角改变
// a 轨道半长轴，km
// i0 初始轨道倾角，度
// it 目标轨道倾角，度
// ascordes = 1 在升交点
// ascordes = 0 在降交点
// 返回：轨道系的速度增量m/s
vec3 leo_Binc(double a, double i0, double it, int ascordes)
{
	double v = sqrt(GE / a);
	double dv;
	vec3 vo;
	if (ascordes == 1) // 升交点
	{
		dv = v * sin((it - i0)*RAD / 2) * 2 * 1000;
		vo(0) = -dv * sin((it - i0) * RAD / 2);
		vo(1) = -dv * cos((it - i0) * RAD / 2) * sign(it - i0);
	}
	else
	{
		dv = v * sin((it - i0)*RAD / 2) * 2 * 1000;
		vo(0) = -dv * sin((it - i0) * RAD / 2);
		vo(1) = dv * cos((it - i0) * RAD / 2) * sign(it - i0);
	}
	vo(2) = 0;

	return vo;
}