#ifndef __SATELLITE_H
#define __SATELLITE_H

// C++ common head file
#if defined(_MSC_VER) && _MSC_VER < 1300
	#include <iostream.h>
	#include <fstream.h>
	#include <math.h>
	#include <float.h>
	#include <list>
	#include <queue>
#else
	#include <fstream>
	#include <iostream>
	using namespace std;
	#include <cmath>
	#include <limits>
	#include <list>
	#include <queue>
	#include <stdexcept>
#endif

#include "constant.h"
using namespace Constant;
#include "config.h"
#include "CEngine.h"
#include "CentralBody.h"
#include "PlanetEphemeris.h"
#include "Coordination.h"
#include "StopCondition.h"
#include "Maneuver.h"
#include "Quaternion.h"
#include "Kepler.h"

// 摄动项设置定义

// 地球卫星
#define ODP_EARTH_ZONAL             0x01  // 地球带谐项
#define ODP_EARTH_TESSERAL          0x02  // 地球田谐项
#define ODP_AIR_DRAG                0x04  // 大气阻力
#define ODP_LUNAR_CENT              0x08  // 月球中心引力(对非月球卫星适用)
#define ODP_SOLAR_CENT              0x10  // 太阳中心引力(对非日心轨道)
#define ODP_SOLAR_PRESSURE          0x20  // 太阳光压
#define ODP_EARTH_ALBEDO            0x40  // 地球反照光压
#define ODP_SOLID_TIDES             0x80  // 地球固体潮
#define ODP_OCEAN_TIDES             0x0100 // 地球海潮
#define ODP_POSTNEWTON              0x0200  // 后牛顿效应
#define ODP_ADDITIONAL              0x0400  // 坐标系附加摄动
#define ODP_THERMAL_RADIATION_PRESSURE 0x0800 // 热辐射压
// 月球卫星
#define ODP_LUNAR_ZONAL        0x1000 // 月球带谐项
#define ODP_LUNAR_TESSERAL     0x2000 // 月球田谐项
#define ODP_LUNAR_NON_SPHERE   0x3000 // 月球非球形引力(带谐项+田谐项)
#define ODP_EARTH_CENT         0x4000 // 地球中心引力(对非地球卫星适用)
#define ODP_LUNAR_TIDES        0x8000 // 月球固体潮
#define ODP_LUNAR_LIBRATION    0x010000 // 月球物理天平动
// 深空探测器
#define ODP_VENUS_CENT         0x100000 // 金星中心引力
#define ODP_MARS_CENT          0x200000 // 火星中心引力
#define ODP_JUPITER_CENT       0x400000 // 木星中心引力
#define ODP_SATURN_CENT        0x800000 // 土星中心引力
// 常用集合
#define ODP_EARTH_ALL  0x0FFF // 除月球非球形引力之外的所有摄动力
#define ODP_GEO        0x0FFB // 同步轨道卫星通常考虑的摄动力
#define ODP_LEO        0x0007 // 低轨道卫星通常考虑的摄动力
#define ODP_LUNAR_SAT  0x1F030 // 环月卫星

/*!姿态模式
\enum AttitudeMode
EarthPoint: 正常对地定向姿态\n
SunPoint: 对日定向姿态\n
InertiallyFixed: 惯性定向姿态\n
Custom: 自己设置
*/
enum AttitudeMode{ EarthPoint = 0, SunPoint , InertiallyFixed , Custom };

/*! 卫星的虚拟基类
\author 韩冬
CSatelliteBase用作所有卫星的虚拟基类

包含卫星的基本特征：
- 时间(相对仿真初始历元StartEpoch)的时间(秒数)
- 相对中心体惯性系的位置速度,地心惯性系定义为J2000.0,月心惯性系定义为基本平面为月球赤道面,x为J2000.0在月球赤道上的投影
- 卫星名称，用作描述卫星和存储卫星星历的文件名
- 卫星的轨道根数，相对中心天体
- 卫星动力学特性：面质比、质量、储箱、发动机等

类中已实现的函数主要为取值输出函数和RKF78积分算法，虚拟函数为卫星动力学方程和轨道根数的计算

摄动源的选择使用SetForce(int l,unsigned char PTB)函数实现，其中l为引力场阶数，PTB为摄动源选择变量，采用如下宏定义：
\code
#define ODP_ZONAL      0x80   // 带谐项
#define ODP_TESSERAL   0xC0   // 带谐项+田谐项
#define ODP_LUNAR      0x20   // 月球引力
#define ODP_SOLAR      0x10   // 太阳引力
#define ODP_LIGHT      0x08   // 太阳光压
#define ODP_AIR        0x04   // 大气阻力
#define ODP_POSTNEWTON 0x02   // 后牛顿效应
#define ODP_LUNAR_NON_SPHERE  0x01  // 月球非球形引力

#define ODP_ALL        0xFE   // 除月球非球形引力之外的所有摄动力
#define ODP_GEO        0xF4   // 同步轨道卫星通常考虑的摄动力：田谐项、日月引力、光压、后牛顿效应
#define ODP_LEO        0x08   // 低轨道卫星通常考虑的摄动力：大气阻力
\endcode
PTB为一个8bit标志字，其8位分别表示：\n
||1||2||3||4||5||6||7||8
|Zonal带谐项|Zonal+Tesseral带谐项＋田谐项|Lunar月球引力|Solar太阳引力|Light太阳光压|Air大气阻力|PostN后牛顿效应|Lunar_nonsphere月球非球形引力
*/
class ORBITDYN_API CSatelliteBase
{
protected:
	//! 起始历元
	CDateTime Epoch; //TODO: 这里应该用TDT时间，不要用UTC!!
	//! 相对起始历元的时间
	double ElapsedTime;
	//!中心体惯性系位置
	vec3 Position;
	//!中心体惯性系速度
	vec3 Velocity;
	//!中心体
	PLANET center;
public:
	//!卫星名称，用作储存数据，默认为a、b、c......
	std::string Name;
	//!初始轨道根数
	Kepler Status0;
	//!卫星起始重量(单位:kg)(默认1000kg)
	double Mass0;
	//! 消耗燃料质量(单位:kg)(初值0kg)
	double BurnedFuel;
	//!大气阻力系数
	double Cd;
	//!迎风面积
	double AirDragArea;
	//!太阳光压反射率
	double Cr;
	//!光压等效面积
	double LightPressArea;

	//!发动机状态标志(true:开机,false:关机)
	bool ThrustIsOn;
	//! 推力在本体系的方向(一定为单位矢量)
	vec3 ThrustDirection;
	//! 惯性姿态四元数
	CQuaternion qbi;
	//! 惯性姿态角速度
	vec3 wbi;

	// 状态:
	//!瞬时根数的半长轴(unit：km)
	double a;
	//!瞬时根数的偏心率
	double e;
	//!瞬时根数的轨道倾角(unit:rad)
	double i;
	//!瞬时根数升交点赤经(unit:rad)
	double Omega;
	//!瞬时根数近地点幅角(unit:rad)
	double w;
	//!瞬时根数平近点角(unit:rad)
	double M;

	//!瞬时根数、平根数真近点角(unit:rad)
	double f;
	//!瞬时根数、平根数偏近点角(unit:rad)
	double E;
	//!瞬时轨道卫星幅角u=f+w     (unit:rad)
	double u;
	//!平均角速度n=sqrt(GE/a/a/a)   (unit:rad/s)
	double n;
	//! 矢径
	double r;
	//! 速度
	double v;
	//! 轨道角速度
	double w0;
	//! 惯性系到轨道系转换矩阵
	mat33 Coi;
	//! 加速度
	vec3 acc;

/////// 数据输出函数 //////////////////////////

	//! 卫星初始历元
	inline CDateTime StartEpoch() const { return Epoch; }
	
	//! 卫星当前历元
	inline CDateTime CurrentEpoch() const { return Epoch+ElapsedTime; }

	//! 卫星的相对时间，即从Epoch开始算起的秒数
	inline double t() const { return ElapsedTime; }

	//! 卫星的位置(单位:km)
	inline vec3 Pos() const { return Position; }

	//! 卫星的速度(单位:km/s)
	inline vec3 Vel() const { return Velocity; }
	
	//! 设置卫星位置(单位:km)、速度(单位:km/s)
	inline void SetPosVel(double t,const vec3& p,const vec3& v) 
	{
		ElapsedTime = t;
		Position = p;
		Velocity = v;
		RefreshStatus();
	}	

	//! 卫星的轨道根数，返回一个Kepler
	inline Kepler GetOrbitElements()const {	return Kepler(a,e,i,Omega,w,M);	}
	
	//! 卫星的轨道根数，返回一个Kepler
	inline void GetOrbitElements(double oe[6])const { memcpy(&oe,&a,sizeof(double)*6); }
	
	//! 卫星质量
	inline double Mass(){ return Mass0 - BurnedFuel; }
	
	//! 卫星发动机推力
	inline double Force(){ return Engine->F; }

	CSatelliteBase();
	CSatelliteBase(std::string name);
	virtual ~CSatelliteBase();
	void Initialize();
	void Initialize(const CDateTime& t,const vec3& r,const vec3& v);
	void Initialize( const char* filename );
	void SetAutoSave(bool save = true);
	void SetForce(int l,unsigned int PTB);
	void SetEngine(double I,double f);

// 外推函数
	void Propagate(double Step,const double Duration);
	void Propagate2Perigee();
	void Propagate2Apogee();
	void Propagate2AscendingNode();
	void Propagate2DescendingNode();
	void Propagate2Epoch(const CDateTime epoch);
	// 向后外推
	void PropagateBackward(double h,const double dt);

	//! 瞬时脉冲机动
	void ImpluseManeuver(vec3 Dv,Coordination d = VVLH );

	//! 关闭自动保存文件
	void CloseFile(){FileInstElem.close();FirstTimeOutput = true;}
private:
	//! CSatelliteBase类对象的计数器，用作自动保存文件的命名
	static unsigned int Counter;

protected:
	//! 岁差矩阵
	mat33 PR;
	//! 章动矩阵
	mat33 NU;
	//! 极移矩阵
	mat33 PW;
	//! 岁差章动极移矩阵对应的时间
	double PNTime;
	//! 刷新岁差章动等矩阵
	void RefreshMatrix(const CDateTime t); 
	
	
	//! 引力场带谐项和田谐项的阶数
	int LL;
	
	//! 摄动源选择标志
	unsigned int Perturbation;
	
	//! 发动机
	CEngine* Engine;

/////// 文件操作  ///////////////

	//! 输出文件
	fstream FileInstElem;
	
	//! 是否第一次写文件标志
	bool FirstTimeOutput;
	
	//! 是否自动保存标志
	bool Save;
	
	virtual void SaveElem() = 0;

	virtual int OneStep(double StepSize,double MaxCutError = 1,CStopCondition* SC = NULL);
	virtual void RefreshStatus() = 0;
	virtual void DynFunction(const double t,const vec& x,vec& y) = 0;

	ORBITDYN_API friend double RKF78(CSatelliteBase& s,double hh,vec& x,vec& xh);
	ORBITDYN_API friend double Gill4(CSatelliteBase& s,double hh,vec& x,vec& xh);
};

ORBITDYN_API ostream & operator<<(ostream & os, const CSatelliteBase& sat);

/*!
\class CSatellite
卫星CSatelliteBase的基本实现
一般用作地球卫星的轨道计算，也可以计算地月转移轨道和秤动点轨道
\example examples/basic.cpp
*/
class ORBITDYN_API CSatellite:public CSatelliteBase
{
public:
	//! 构造及析构函数
	CSatellite(){}
	~CSatellite(){}

	//! 初始化及设置函数
	void Initialize(const CDateTime& t,const Kepler elem,const char flag = 'i');
	
	//! 输出卫星的初始值到文件中
	void ReportInitial();

	//! 计算地固系ECF位置
	const vec3 ECFPos();

	//! 卫星在地固的位置速度
	void GetECF(vec3& ECFr,vec3& ECFv);
	
	//! 计算卫星的地理经纬度和星下点的地理经纬度
	CSpherical SubSatPoint(CSpherical & Geodetic,CSpherical & SSP);
	
	//! 计算平均轨道根数
	Kepler MedianElement() const;
	
	//! 地理坐标
	CSpherical GetLLA();
	
	//double FiniteManeuver(Direction , StopCondition);

private:
	void DynFunction(const double t,const vec& x,vec& y);
	void RefreshStatus();

protected:
	void SaveElem();
};

/*!
加快计算速度的轨道外推
使用Gill4积分算法，积分过程的动力学方程中不更新岁差章动、日月位置、摄动力等参数，
而是每隔一段时间更新

为了保证长时间仿真中时间累计的精度，使用整数的毫秒数ElapsedTimems作为计时变量，原有的ElapsedTime作为辅助变量
*/
class ORBITDYN_API CRapidSatellite : public CSatellite
{
public:
	CRapidSatellite():ElapsedTimems(0)
		,m_lastpert(0)
		,PeriodCoord(1800000)
		,PeriodPert(50)
	{}
	~CRapidSatellite(){}

	//! 初始化及设置函数
	void Initialize(const CDateTime& t,const Kepler elem,const char flag);

	// h单位为ms
	void Propagate(int h);

private:
	// StepSize单位为ms
	int OneStep(int StepSize,double MaxCutError = 1,CStopCondition* StopCon = NULL );
	void DynFunction(const double t,const vec& x,vec& y);
	void RefreshStatus();
	void RefreshPert(const CDateTime t);// 刷新摄动力
#if _MSC_VER < 1300
	unsigned long ElapsedTimems;
#else
	//! 相对初始历元的时间毫秒数
	unsigned long long ElapsedTimems;
#endif

public:

	// 为了加快计算速度，增加以下变量，用作RKF调用动力学方程时，以下变量可不用重复计算
	// 坐标参数更新周期
	long PeriodCoord;   // 摄动力的更新周期，默认值1,800,000ms

private:
	//! J2000-->ECI转换矩阵
	mat33 HG;
	//! 月球位置
	vec3 rm;
	//! 太阳位置
	vec3 rs;

public:
	// 记录当前的摄动加速度，在RKF每次调用动力学方程时，可以认为右端摄动加速度不
	long PeriodPert;   // 摄动力的更新周期，默认值50ms

private:
#if _MSC_VER < 1300
	unsigned long m_lastpert;
#else
	unsigned long long  m_lastpert; // 上次计算摄动的时间，摄动每PeriodPert(ms)更新一次
#endif
	vec3 F_zonal;     // 带谐项
	vec3 F_tesseral;  // 田谐项
	vec3 F_moon;      // 月球引力
	vec3 F_sun;       // 太阳引力
	vec3 F_air;       // 大气阻力
	vec3 F_light;     // 光压
	vec3 F_ppn;       // 后牛顿
	vec3 F_Thrust;    // 推力
};

/*!
\class CLEO
地球低轨道卫星
使用小偏心率轨道根数作为状态变量进行积分
一般不使用，仅仅为了保留轨道根数的积分，为以后进行精度比较保留代码
*/
class ORBITDYN_API CLEO:public CSatellite
{
public:
	CLEO(){}
	~CLEO(){}
	void Propagate(double h,const double dt);
private:
	int OneStep(double StepSize,double MaxCutError = 1.0,CStopCondition* SC = NULL);
	void DynFunction(const double t,const vec& x,vec& y);
	void RefreshStatus();
};
/*!
\class CGEO
地球同步轨道卫星
增加地球同步轨道常用的轨道根数，还没有具体实现
*/
class ORBITDYN_API CGEO:public CSatellite
{
public:
	double D;      //! 相对漂移率
	double ex,ey;  //! 偏心率矢量
	double ix,iy;  //! 倾角矢量
	double L;      //! L=Omega+w+M
};
/*!
\class CMoonSat
月球卫星的轨道计算
计算绕月飞行的卫星的轨道
这里
*/
class ORBITDYN_API CMoonSat: public CSatelliteBase
{
private:
	// 对动力学方程和轨道根数的计算重新定义
	void DynFunction(const double t,const vec& x,vec& y);
	void RefreshStatus();
	void SaveElem();
public:
	CMoonSat(){
		// 这里借用ODP_LUNAR表示是否考虑地球引力
		Perturbation = ODP_LUNAR_SAT;
		LL = 70;
		LightPressArea = 15.076;
		Mass0 = 1460.0;
		Cr = 1.44;
	}
	//! 使用月心惯性系轨道根数初始化
	void Initialize(const CDateTime& t,const Kepler elem);
	//! 使用月心惯性系位置速度初始化
	void Initialize(const CDateTime& t,const vec3 p,const vec3 v);
};

class ORBITDYN_API SatelliteException : public BaseException
{
public:
	SatelliteException(const std::string& details = "") 
		: BaseException("CSatellite exception:",details)
	{
	}
	virtual ~SatelliteException()
	{
	}
	SatelliteException(const SatelliteException& cdte)
		:BaseException(cdte)
	{
	}
};

#endif  // __SATELLITE_H
