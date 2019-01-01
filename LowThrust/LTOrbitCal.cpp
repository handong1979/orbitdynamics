// LTOrbitCal.cpp: 定义控制台应用程序的入口点。
//

#include <iostream>
#include <string>
using namespace std;

#include "OrbitDyn.h"
using namespace Constant;

//初始化函数：void OrbitCal_Init(struct OCalInit)
//运行一步函数：void OrbitCal_Step(double steptime, struct OCalIn, struct OCalOut)
struct OCalInit
{
	string epoch;
	double a;	//半长轴
	double 	e;	//偏心率
	double 	i;	//倾角
	double 	Omega;	//升交点赤径
	double 	w;	//近地点幅角
	double 	M;	//平近点角
	double 	Mass;	//卫星初始质量
	double  Area;  // 大气阻力迎风面积
};

CSatellite sat;

void OrbitCal_Init(struct OCalInit ocal)
{
	//char c[100] = "2019-01-01 00:00:00";
	int y, m, d, h, min;
	double sec;
	sscanf_s(ocal.epoch.c_str(), "%d-%d-%d %d:%d:%lf", &y, &m, &d, &h, &min, &sec);
	//CDateTime ep = StrToDateTime(c);
	//CDateTime ep = StrToDateTime(ocal.epoch.c_str());
	CDateTime ep(y,m,d,h,min,sec);
	Kepler kp;
	kp.a = ocal.a;
	kp.e = ocal.e;
	kp.i = ocal.i*RAD;
	kp.o = ocal.Omega*RAD;
	kp.w = ocal.w*RAD;
	kp.M = ocal.M*RAD;
	sat.Initialize(ep, kp);
	sat.Mass0 = ocal.Mass;
	sat.AirDragArea = ocal.Area;
}
struct OCalIn {
	double Fx;	//推力在轨道坐标系的X分量
	double Fy;	//推力在轨道坐标系的Y分量
	double Fz;	//推力在轨道坐标系的Z分量
	double Isp;	//推力器比冲
};

struct OCalOut {
	string t;		//初始轨道参数对应的北京时间（如：2018 - 01 - 01 00:00 : 00)
	double a;		//半长轴
	double e;		//偏心率
	double i;		//倾角
	double Omega;	//升交点赤径
	double w;		//近地点幅角
	double M;		//平近点角
	double Mass;	//卫星初始质量
	double x;		//卫星在惯性系中的位置X分量
	double y;		//卫星在惯性系中的位置Y分量
	double z;		//卫星在惯性系中的位置Z分量
	double vx;		//卫星在惯性系中的速度X分量
	double vy;		//卫星在惯性系中的速度Y分量
	double vz;		//卫星在惯性系中的速度Z分量
	double Lon;		//地心经度
	double Lat;		//地心纬度
	double Alt;		//高度
};

void OrbitCal_Step(double steptime, struct OCalIn oci, struct OCalOut& oco)
{
	double Fv = sqrt(oci.Fx *oci.Fx + oci.Fy * oci.Fy + oci.Fz * oci.Fz);
	sat.SetEngine(oci.Isp, Fv);
	sat.ThrustIsOn = true;
	sat.qbi = GetCoi(sat.GetOrbitElements());
	sat.ThrustDirection(0) = oci.Fx / Fv;
	sat.ThrustDirection(1) = oci.Fy / Fv;
	sat.ThrustDirection(2) = oci.Fz / Fv;
	sat.Propagate(steptime, steptime);

	CDateTime t = sat.CurrentEpoch();
	char tc[100];
	t.ToChar(tc);
	oco.t = tc;
	oco.a = sat.a;
	oco.e = sat.e;
	oco.i = sat.i*DEG;
	oco.Omega = sat.Omega*DEG;
	oco.w = sat.w*DEG;
	oco.M = sat.M*DEG;
	oco.Mass = sat.Mass();
	oco.x = sat.Pos()(0);
	oco.y = sat.Pos()(1);
	oco.z = sat.Pos()(2);
	oco.vx = sat.Vel()(0);
	oco.vy = sat.Vel()(1);
	oco.vz = sat.Vel()(2);
	CSpherical lla = sat.GetLLA();
	oco.Lon = lla.Longitude;
	oco.Lat = lla.Latitude;
	oco.Alt = lla.Altitude;
}

int main()
{
	// 初始化参数
	struct OCalInit ocal;
	// 输入输出参数
	struct OCalIn oci;
	struct OCalOut oco;

	//// 轨道初值
	//ocal.epoch = "2019-01-01 00:00:00";
	//ocal.a     = 7000;
	//ocal.e     = 0.001;
	//ocal.i     = 98*RAD;
	//ocal.Omega = 0;
	//ocal.w     = 0;
	//ocal.M     = 0;
	//ocal.Mass  = 60;
	//ocal.Area  = 0.16;
	//// 推力大小、比冲
	//oci.Fx = 0.1;
	//oci.Fy = 0;
	//oci.Fz = 0;
	//oci.Isp = 1000;
	
	// 从文件读入数据，会覆盖上面的赋值
	BEGIN_READ("LTOC.txt")
	READSTR(ocal.epoch)
	READVAR(ocal.a,    "%lf")
	READVAR(ocal.e,    "%lf")
	READVAR(ocal.i,    "%lf")
	READVAR(ocal.Omega,"%lf")
	READVAR(ocal.w,    "%lf")
	READVAR(ocal.M,    "%lf")
	READVAR(ocal.Mass, "%lf")
	READVAR(ocal.Area, "%lf")
	READVAR(oci.Fx,    "%lf")
	READVAR(oci.Fy,    "%lf")
	READVAR(oci.Fz,    "%lf")
	READVAR(oci.Isp,   "%lf")
	END_READ

	// 初始化
	OrbitCal_Init(ocal);

	// 外推步长
	double step = 60; 
	// 外推总步数
	int len = 1440 * 10;

	// 输出STK星历文件的头部
	fstream fstk("OrbitCal.e", ios::out);
	fstk << "stk.v.4.3" << endl;
	fstk << "BEGIN Ephemeris" << endl;
	fstk << "NumberOfEphemerisPoints " << len << endl;
	fstk << "ScenarioEpoch           " << ocal.epoch << endl; //1 Jun 2002 12:00 : 00.000000000
	fstk << "InterpolationMethod     Lagrange" << endl;
	fstk << "InterpolationOrder      5" << endl;
	fstk << "CentralBody             Earth" << endl;
	fstk << "CoordinateSystem        J2000" << endl << endl;
	fstk << "EphemerisTimePosVel" << endl;
	fstk.precision(12);

	
	// 保存数据文件
	fstream fo("OrbitCal.txt", ios::out);
	fo.precision(12);

	for (int KK = 0;KK < len;KK++)
	{
		


		// 外推一步
		OrbitCal_Step(step, oci, oco);
		
		// 保存数据文件
		fo << KK*step << "\t" << oco.a << "\t" << oco.e << "\t" << oco.i << "\t" << oco.Omega
			<< "\t" << oco.w << "\t" << oco.M << "\t"
			<< oco.x << "\t" << oco.y << "\t" << oco.z << "\t"
			<< oco.vx << "\t" << oco.vy << "\t" << oco.vz << "\t"
			<< oco.Lon << "\t" << oco.Lat << "\t" << oco.Alt << "\t"
			<< oco.Mass << "\n";
		
		// 输出STK星历文件
		fstk << KK * step << "\t" << oco.x * 1000 << "\t" << oco.y * 1000 << "\t" << oco.z * 1000 << "\t"
			<< oco.vx * 1000 << "\t" << oco.vy * 1000 << "\t" << oco.vz * 1000 << "\n";

		// 控制台提示
		printf("a = %lf mass = %lf\n",oco.a, oco.Mass);
	}

	fo.close();

	fstk << "END Ephemeris" << endl;
	fstk.close();

    return 0;
}

