#ifndef __CENTRALBODY_H
#define __CENTRALBODY_H

#if defined(_MSC_VER) && _MSC_VER < 1300
	#include <iostream.h>
	#include <fstream.h>
	#include <math.h>
#else
	#include <fstream>
	#include <iostream>
	#include <cmath>
	using namespace std;
#endif

#include "Config.h"
#include "Msise00.h"

#pragma warning(disable:4251)

ORBITDYN_API double SA76(double h);

class ORBITDYN_API CentralBody
{
public:

	//! 引力场系数
	double CLM[71][71],SLM[71][71];

	//! 引力场模型
	enum GRAVITY {WGS84, WGS84_EGM96, EGM96, JGM2, JGM3, GLGM2, LP165P} GrvModel;

	//! 星历模型
	enum EPHEMICS {DE421EPH, DE405EPH , ANALYTICEPH } EphModel;

	CentralBody();
	virtual ~CentralBody();
	
	//! 设置引力场
	virtual void SetGravityField(GRAVITY) = 0;

	//! 获得地心J2000惯性系下的位置
	virtual vec3 GetECI(const CDateTime&) = 0;

	//! 获得地心J2000惯性系下的位置速度
	//virtual void GetECI(const CDateTime&,vec& r,vec& v) = 0;

	inline double GE()const{ return GrvParam; }
	inline double RE()const{ return Equator_Radius; }
protected:
	//! 引力常数
	double GrvParam;
	//! 平均赤道半径常数
	double Equator_Radius;
	//! 数据文件目录
	std::string DataDir;
    //! 读取引力场文件
	void ReadGrvFile(const char* file);
};

//! 太阳
class ORBITDYN_API Sun : public CentralBody
{
public:
	//! 获取本例
	static Sun* Instance();

	void SetGravityField(GRAVITY);

	//! 获得地心J2000惯性系下的位置
	vec3 GetECI(const CDateTime&);
private:
	Sun();
	~Sun(){};
	static Sun* theInstance;
};


//! 地球
class ORBITDYN_API Earth : public CentralBody
{
public:
	//! 常用地球引力场系数
	double A2,A3,A4,J2,J3,J4;

	//! 大气模型
	enum ATMOS { STANDARD1976, MSISE2000, MANUAL } AtmosModel;

	//! 获取本例
	static Earth* Instance();

	//! 计算高度h(km)处的大气密度
	double AtmosRuoH(const CDateTime& t,const vec3& ECFr);
	//! 设置引力场
	void SetGravityField(GRAVITY);

	//! 从eop插值计算极移参数
	void GetPolarMotion(const CDateTime& t,double& Xp,double& Yp);

	//! 从eop插值计算dUT1 = UT1 - UTC
	double GetdUT1(const CDateTime& t);

	//! 获得地心J2000惯性系下的位置
	vec3 GetECI(const CDateTime&);
private:
	Earth();
	~Earth();
	
	//! 本例
	static Earth* theInstance;
	//! 极移、dUT1数据表
	double ** eop;
	//! 数据表行数
    int Num;
	//! 数据表初始MJD
	double mjd1;

	//! 大气密度文件结构
	struct FILE_ATMOS_DEN {
		int nrow; // 行数
		double step; // ??
		double* t; // 时间(MJD表示)
		double* rou; // 大气密度
	} file_atmos_den;

public:
	//! 设置大气模型
	void SetAtmosphereModel(ATMOS);
};

//! 月球
class ORBITDYN_API Moon : public CentralBody
{
public:
	//! 获取本例
	static Moon* Instance();

	void SetGravityField(GRAVITY);

	//! 获得地心J2000惯性系下的位置
	vec3 GetECI(const CDateTime&);
private:
	Moon();
	~Moon(){};
	static Moon* theInstance;
};

class ORBITDYN_API CentralBodyException : public BaseException
{
public:
	CentralBodyException(const std::string& details = "") 
		: BaseException("CCentralBody exception:",details)
	{
	}
	virtual ~CentralBodyException()
	{
	}
	CentralBodyException(const CentralBodyException& cdte)
		:BaseException(cdte)
	{
	}
};

#endif // __CENTRALBODY_H
