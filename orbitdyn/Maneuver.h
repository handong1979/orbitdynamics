#ifndef __MANEUVER_H
#define __MANEUVER_H

#include "Config.h"
#include <float.h>

//! 变轨机动
class ORBITDYN_API Maneuver
{
public:
	//! 开机时间
	double StartTime;
	//! 关机时间
	double EndTime;
	//! 轨控方向
	vec3 DV;
	//! 轨控点的平近点角
	double M;
	//! 燃料消耗量
	double Fuel;
	Maneuver():DV("0 0 0"){
		StartTime = DBL_MAX;
		EndTime = DBL_MAX;
		Fuel = 0;
	}
	~Maneuver(){}
	//! t时刻是否达到开机时间
	bool Start(double t) const{
		return t>StartTime;
	}
	//! t时刻是否达到关机时间
	bool Finish(double t) const{
		return t>EndTime;
	}
};

////! 变轨机动:时间开关机
//class Maneuver_Duration : public Maneuver
//{
//public:
//	vec DV;          // 轨道机动需要的DV
//	double StartTime;   // 机动开始时间
//	double EndTime;     // 机动结束时间
//	Maneuver_Duration()	{
//	}
//	~Maneuver_Duration(){
//	}
//	bool Start(double t) const{
//		return t>=StartTime;
//	}
//	bool Finish(double t) const{
//		return t>=EndTime;
//	}
//};

////! 变轨机动:速度增量关机
//class Maneuver_DV : public Maneuver
//{
//public:
//	double DV_Need;     // 轨道机动需要的DV
//	double DV_Now;
//	Maneuver_DV(){
//		DV_Need = 0;
//		DV_Now = 0;
//	}
//	~Maneuver_DV(){}
//	bool Finish() const{
//		return DV_Now>=DV_Need ? true : false;
//	}
//};

#endif //__MANEUVER_H
