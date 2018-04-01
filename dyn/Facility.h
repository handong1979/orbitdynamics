/*!
\file  Facility.cpp
\author HanDle
地面站惯性系位置
*/

#ifndef __FACILITY_H
#define __FACILITY_H

#include "Coordination.h"
#include "Utility.h"

/*!地面站
*/
class ORBITDYN_API CFacility
{
public:
	//! 最小仰角
	double Elevation;

	//! 地理坐标
	double Longitude,Latitude,Altitude;

	//! 地固系位置
	vec3 ECFPosition;

	//! 默认构造函数
	CFacility()
		:Elevation(0.08726646),
		Longitude(116.388),
		Latitude(39.9),
		Altitude(0.2),
		ECFPosition("0 0 0")
	{
		CalculateECF();
	}

	//! 地理经纬度构造函数
	CFacility(double lon,double lat,double alt,double elev = 5.0);

	//! 析构
	~CFacility(){ }

	//! 设置经纬度
	void SetGeodetic(double Longitude,double Latitude,double Altitude);

	//! 惯性系位置速度
	void GetECI(const CDateTime time,vec3 & Position,vec3 & Velocity);

	//! J2000.0惯性系位置
	vec3 ECIPos(const CDateTime time);

private:

	//! 计算惯性系位置速度
	void CalculateECI(CDateTime time);

	//! 计算地固系位置
	void CalculateECF();
};

// 地固系到地面站坐标系的转换矩阵
ORBITDYN_API mat33 VehicleCoord(double Longitude,double Latitude);

#endif  // __FACILITY_H
