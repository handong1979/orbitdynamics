////////////////////////////////////////////////////////////////////////////////
//File:   UTCG.h
//Author: HanDle
//Date:   2004-9-1
//Usage:  Definition of UTCG
///////////////////////////////////////////////////////////////////////////////
#ifdef _MSC_VER
#pragma once
#endif

#ifndef __DATETIME_H
#define __DATETIME_H

#include "Config.h"
#include "BaseException.h"

#if defined _MSC_VER && _MSC_VER < 1300
	#include <stdio.h>
	#include <stdlib.h>
	#include <iostream.h>
	#include <math.h>
	#include <string.h>
#else
	#include <iostream>
	using namespace std;
	#include <cmath>
#endif

#include <time.h>
#include "Constant.h"
using namespace Constant;

ORBITDYN_API double MJD(int year,int month,int day,int hour,int minute,double second);
ORBITDYN_API void GetCalendar(double mjd,int &Y,int &M,int &D,int &h,int &min,double &s);

//! 日期时间错误类
class ORBITDYN_API CDateTimeException:public BaseException
{
public:
	CDateTimeException(const std::string& details = "") 
		: BaseException("UTCG exception:",details)
	{
	}
	virtual ~CDateTimeException() _NOEXCEPT
	{
	}
	CDateTimeException(const CDateTimeException& cdte)
		:BaseException(cdte)
	{
	}
};

enum CDateTimeFormat{YMD = 0, STK, BJT, BACCTIME, NUMTIME};

//!日期时间类
class ORBITDYN_API UTCG
{
public:
	static CDateTimeFormat format;
  	//! 默认构造函数
	UTCG();
	// 用简约儒略日构造
	// 应当避免double到CDateTime的默认类型转换，通常这样易导致各个时间系统的混淆
	//UTCG(double mjd):UTC(mjd) { }
	//! 复制构造
	UTCG(const UTCG& t);

	//! 用日期时间构造
	UTCG(int y,int mon,int d,int h,int min,double s);

	//! 用数组构造
	UTCG(double ar[6]);

	~UTCG(){}

	//! 协调世界时(天)，以简约儒勒日表示,MJD=JD-2400000.5
	double GetMJD() const;
	
	//! 协调世界时(天)，以简约儒勒日表示,MJD=JD-2400000.5
	double GetUTC() const;
	
	//! 地球动力学时(天),衡量卫星运动的均匀时间s尺度，以简约儒勒日表示,MJD=JD-2400000.5
	double GetTDT() const;
	
	//! 世界时UT1=UT0+极移改正(天)
	double GetUT1() const;
	
	//! 动力学时距J2000.0的儒勒世纪数
	double GetTDTCentNum() const;
	
	//! UT1时距J2000.0的儒勒世纪数
	double GetUT1CentNum() const;
	
	//! day of year
	int day_of_year() const;
	
	//! seconds in day
	double seconds_in_day() const;
	
	//! 得到时间，放在1×6的数组中
	void GetTime(double time[]);
	
	//! 变成字符
	void ToChar(char* a) const;
	
	//! 设置时间，年月日时分秒
	void SetTime(int y,int mon,int d,int h,int min,double s);

	//! 直接设置UTC
	void SetMJD(double mjd);
	
	//! 设置时间，MJD表示的UTC
	void SetTime(double utc);

	//! 增加dt的时间(秒数)
	UTCG& operator+=(double dt);
	
	//减少dt的时间(秒数)
	UTCG& operator-=(double dt);

	//获得nday
	int GetDay() const{	return nday; }

	//获得sec
	double GetSec() const { return sec; }

private:
	//! 协调世界时(天)，以简约儒勒日减去CDATETIME_BIAS表示,MJD=JD-2400000.5
	// 当UTC本身值较大时，当用CDateTime的对象来累计时间，会造成截断误差的累计，
	// 因此存储时应减去CDATETIME_BIAS以减小UTC的绝对值
	// 当CDATETIME_BIAS设为53736.0(即2006年1月1日零时的MJD)时，
	// 以60秒步长使用+=运算，累计5天，误差为10^-6秒量级，可以接受
	int nday; // 简约儒略日整天数 (单位：day)
	double sec; // 不足1天的秒数 (单位: s)	

	//! 根据时间查表找dTAI=TAI-UTC
	int GetdTAI() const;  
	
	//! 查表得到dUT1=UT1-UTC
	double GetdUT1() const;

	//! 归一化为整天＋当天秒数
	void Normalize();
};

// 保持与原版本的兼容性
typedef UTCG CDateTime;

ORBITDYN_API std::ostream& operator<<(std::ostream& ostrm,const UTCG& t);
ORBITDYN_API std::istream& operator>>(std::istream& istrm,UTCG& t);
ORBITDYN_API UTCG operator+(const UTCG & t,double sec);
ORBITDYN_API UTCG operator-(const UTCG& t,double sec);
ORBITDYN_API double operator-(const UTCG& t1,const UTCG& t0);
ORBITDYN_API bool operator>(const UTCG& t1,const UTCG& t0);
ORBITDYN_API bool operator<(const UTCG& t1,const UTCG& t0);
ORBITDYN_API bool operator>=(const UTCG& t1,const UTCG& t0);
ORBITDYN_API bool operator<=(const UTCG& t1,const UTCG& t0);

class ORBITDYN_API TDT
{
public:
	TDT():_t(0){}
	TDT(const UTCG& u):_t(u.GetTDT()){}
	TDT(const TDT& tdt):_t(tdt._t){}
	TDT& operator=(const TDT& right){
		_t = right._t;
		return *this;
	}
		
private:
	double _t;
};


ORBITDYN_API double MJD(int year,int month,int day,int hour,int minute,double second);
ORBITDYN_API void GetCalendar(double mjd,int &Y,int &M,int &D,int &h,int &min,double &s);
ORBITDYN_API UTCG StrToDateTime(const char* str);

class ORBITDYN_API Date
{
public:
	Date():year(2009),month(12),day(6),hour(17),minute(11),second(40){}
	Date(int y,int m,int d,int h,int min,double s):year(y),month(m),day(d),hour(h),minute(min),second(s){}
	Date(const Date& d):year(d.year),month(d.month),day(d.day),hour(d.hour),minute(d.minute),second(d.second){}
	Date& operator=(const Date& d){
		year = d.year;
		month = d.month;
		day = d.day;
		hour = d.hour;
		minute = d.minute;
		second = d.second;
		return *this;
	}
public:
	// 年月日时分秒
	int year;
	int month;
	int day;
	int hour;
	int minute;
	double second;
};

class ORBITDYN_API BJT
{
public:
	BJT(){}
private:
	Date _date;	
};
		

class ORBITDYN_API CDateTimeView : public UTCG
{
public:
	CDateTimeView();
	CDateTimeView(const UTCG& t);
	CDateTimeView(const CDateTimeView& t){	_m = t._m;	}
	CDateTimeView& operator=(const CDateTimeView &t){	_m = t._m; return *this;	}
	~CDateTimeView();
private:
	Date _m;
};

#endif
