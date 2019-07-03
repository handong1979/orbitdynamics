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

//! ����ʱ�������
class ORBITDYN_API CDateTimeException:public BaseException
{
public:
	CDateTimeException(const std::string& details = "") 
		: BaseException("UTCG exception:",details)
	{
	}
	virtual ~CDateTimeException()
	{
	}
	CDateTimeException(const CDateTimeException& cdte)
		:BaseException(cdte)
	{
	}
};

enum CDateTimeFormat{YMD = 0, STK, BJT, BACCTIME, NUMTIME};

//!����ʱ����
class ORBITDYN_API UTCG
{
public:
	static CDateTimeFormat format;
  	//! Ĭ�Ϲ��캯��
	UTCG();
	// �ü�Լ�����չ���
	// Ӧ������double��CDateTime��Ĭ������ת����ͨ�������׵��¸���ʱ��ϵͳ�Ļ���
	//UTCG(double mjd):UTC(mjd) { }
	//! ���ƹ���
	UTCG(const UTCG& t);

	//! ������ʱ�乹��
	UTCG(int y,int mon,int d,int h,int min,double s);

	//! �����鹹��
	UTCG(double ar[6]);

	~UTCG(){}

	//! Э������ʱ(��)���Լ�Լ�����ձ�ʾ,MJD=JD-2400000.5
	double GetMJD() const;
	
	//! Э������ʱ(��)���Լ�Լ�����ձ�ʾ,MJD=JD-2400000.5
	double GetUTC() const;
	
	//! ������ѧʱ(��),���������˶��ľ���ʱ��s�߶ȣ��Լ�Լ�����ձ�ʾ,MJD=JD-2400000.5
	double GetTDT() const;
	
	//! ����ʱUT1=UT0+���Ƹ���(��)
	double GetUT1() const;
	
	//! ����ѧʱ��J2000.0������������
	double GetTDTCentNum() const;
	
	//! UT1ʱ��J2000.0������������
	double GetUT1CentNum() const;
	
	//! day of year
	int day_of_year() const;
	
	//! seconds in day
	double seconds_in_day() const;
	
	//! �õ�ʱ�䣬����1��6��������
	void GetTime(double time[]);
	
	//! ����ַ�
	void ToChar(char* a) const;
	
	//! ����ʱ�䣬������ʱ����
	void SetTime(int y,int mon,int d,int h,int min,double s);

	//! ֱ������UTC
	void SetMJD(double mjd);
	
	//! ����ʱ�䣬MJD��ʾ��UTC
	void SetTime(double utc);

	//! ����dt��ʱ��(����)
	UTCG& operator+=(double dt);
	
	//����dt��ʱ��(����)
	UTCG& operator-=(double dt);

	//���nday
	int GetDay() const{	return nday; }

	//���sec
	double GetSec() const { return sec; }

private:
	//! Э������ʱ(��)���Լ�Լ�����ռ�ȥCDATETIME_BIAS��ʾ,MJD=JD-2400000.5
	// ��UTC����ֵ�ϴ�ʱ������CDateTime�Ķ������ۼ�ʱ�䣬����ɽض������ۼƣ�
	// ��˴洢ʱӦ��ȥCDATETIME_BIAS�Լ�СUTC�ľ���ֵ
	// ��CDATETIME_BIAS��Ϊ53736.0(��2006��1��1����ʱ��MJD)ʱ��
	// ��60�벽��ʹ��+=���㣬�ۼ�5�죬���Ϊ10^-6�����������Խ���
	int nday; // ��Լ������������ (��λ��day)
	double sec; // ����1������� (��λ: s)	

	//! ����ʱ������dTAI=TAI-UTC
	int GetdTAI() const;  
	
	//! ���õ�dUT1=UT1-UTC
	double GetdUT1() const;

	//! ��һ��Ϊ���죫��������
	void Normalize();
};

// ������ԭ�汾�ļ�����
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
	// ������ʱ����
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
