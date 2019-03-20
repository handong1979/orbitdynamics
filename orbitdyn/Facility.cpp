/*!
\file  Facility.cpp
\author HanDle
 地面站的函数*/
#include "Facility.h"

/*!
构造函数
Longitude,Latitude,Altitude:经度(度)、纬度(度)、高度(km)
*/
CFacility::CFacility(double Lon,double Lat,double Alt,double elev /* = 5 */)
{
	Elevation = elev*RAD;
	Longitude = Lon*RAD;
	Latitude = Lat*RAD;
	Altitude = Alt;
	CalculateECF();
}
/*!
设置地理坐标
lon,lat,alt:经度(度)、纬度(度)、高度(km)
*/
void CFacility::SetGeodetic(double Lon,double Lat,double Alt)
{
	Longitude = Lon*RAD;
	Latitude = Lat*RAD;
	Altitude = Alt;
	CalculateECF();
}
/*!
计算地面站在J2000坐标系中位置和速度
time:时间
pos,vel:位置和速度
*/
void CFacility::GetECI(const CDateTime time,vec3 & pos,vec3 & vel)
{
	vec3 r = trans(PolarMotion(time))*ECFPosition; // 准地固系位置
	vec3 w;
	w.zeros();
	w(2) = We;
	vec3 v = cross(w,r); // 非旋转准地固系速度
	mat33 PN = trans(Precession(time))*trans(Nutation(time))*trans(EarthRotation(time));
	pos = PN*r;
	vel = PN*v;
}

/*!
计算地面站在J2000坐标系中位置和速度
time:时间
*/
vec3 CFacility::ECIPos(const CDateTime time)
{
	return trans(ECI_ECF(time))*ECFPosition;
}

//! 根据地理经纬度计算地固系位置
void CFacility::CalculateECF()
{
//	double GeocentricLatitude = atan(tan(Latitude)*(1.0-Oblate)*(1.0-Oblate));

// 	ECFPosition.x = (Re + Altitude) * cos(GeocentricLatitude) * cos(Longitude);
// 	ECFPosition.y = (Re + Altitude) * cos(GeocentricLatitude) * sin(Longitude);
// 	ECFPosition.z = (Re + Altitude) * sin(GeocentricLatitude);

	// WGS84
	const static double a = 6378.137;
	const static double f = 1.0/298.2572236;
	// ITRF-96
	//const static double a = 6378.13649;
	//const static double f = 1.0/298.25645;

	const static double ee = 1-(1-f)*(1-f);
	// 开始计算
	// Elliott D.Kaplan GPS原理与应用(第二版), 寇艳红 译,p21
	//double fm = sqrt(1+(1-ee)*tan(Latitude)*tan(Latitude));
	//ECFPosition[0] = a*cos(Longitude)/fm + Altitude*cos(Longitude)*cos(Latitude);
	//ECFPosition[1] = a*sin(Longitude)/fm + Altitude*sin(Longitude)*cos(Latitude);
	//ECFPosition[2] = a*(1-ee)*sin(Latitude)/sqrt(1-ee*sin(Latitude)*sin(Latitude)) + Altitude*sin(Latitude);
	
	// 两种计算公式完全等价

	// Xu GuoChang, GPS Theory,Algorithms and Applications,  p4-5
	double N = a/sqrt(1-ee*sin(Latitude)*sin(Latitude));
	ECFPosition[0] = (N+Altitude)*cos(Latitude)*cos(Longitude);
	ECFPosition[1] = (N+Altitude)*cos(Latitude)*sin(Longitude);
	ECFPosition[2] = (N*(1-ee)+Altitude)*sin(Latitude);
}

// 地固系到地面站坐标系的转换矩阵
// 输入：Longitude地理经度(rad)
//       Latitude地心纬度(rad)
ORBITDYN_API mat33 VehicleCoord(double Longitude,double Latitude)
{
	//const static double f = 1.0/298.257;
	double phic = Latitude;//atan((1-f*f)*tan(Latitude));
	mat33 C;
 	C(0,0) = -sin(Longitude);			 C(0,1) = cos(Longitude);			  C(0,2) = 0;
 	C(1,0) = -cos(Longitude)*sin(phic);  C(1,1) = -sin(phic)*sin(Longitude);  C(1,2) = cos(phic);
 	C(2,0) = cos(Longitude)*cos(phic);	 C(2,1) = cos(phic)*sin(Longitude);	  C(2,2) = sin(phic);
	
	//C(0,0) = cos(Longitude)*cos(phic);	 C(0,1) = cos(phic)*sin(Longitude);	  C(0,2) = sin(phic);
	//C(1,0) = -sin(Longitude);			 C(1,1) = cos(Longitude);			  C(1,2) = 0;
	//C(2,0) = -cos(Longitude)*sin(phic);  C(2,1) = -sin(phic)*sin(Longitude);  C(2,2) = cos(phic);
	return C;
}

/* 计算卫星在地面站中方位角、仰角和距离
{
	CFacility fac;
	fac.SetGeodetic(input->v.Longitude,input->v.Latitude,input->v.Altitude);
	CDateTime epoch;
	epoch.SetTime(input->t.year,input->t.month,input->t.day,input->t.hour,input->t.minute,input->t.second);
	epoch = epoch - 28800 + stepsize; //转化为世界时
	Vector u = fac.ECFPosition;

	struct OrbitElements* oe = &input->oe;
	double n = sqrt(GE/oe->a/oe->a/oe->a);
	Kepler kp(oe->a,oe->e,oe->i*RAD,oe->Omega*RAD,oe->w*RAD,oe->M*RAD+n*stepsize);
	input->oe.M = fmod(kp.M,PI2)*DEG;
	Vector sp,sv;
	Kepler_Cartesian(kp,sp,sv);
	Vector R = ECI_ECF(epoch)*sp;

	Matrix C = VehicleCoord(fac.Longitude*RAD,fac.Latitude*RAD);
	Vector s = C*(R-u);

	//输出
	epoch = epoch + 28800;
	GetCalendar(epoch.GetMJD(),output->t.year,output->t.month,output->t.day,output->t.hour,output->t.minute,output->t.second);
	input->t = output->t;
	output->rou = s.Module();
	output->azimuth = atan2(s.y,s.x)*DEG;
	output->elevation = asin(s.z/output->rou)*DEG;
	return 0;
}
*/
