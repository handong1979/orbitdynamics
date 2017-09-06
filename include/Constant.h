/////////////////////////////////////////////////////////////////////////
//File:   Constant.h
//Author: HanDle
//Date:   2004-8-31
//Usage:  Define some constant
////////////////////////////////////////////////////////////////////////

#ifndef __CONSTANT_H
#define __CONSTANT_H

namespace Constant
{
	//数学常量
	const double PI  = 3.141592653589793;
	const double PI2 = 6.283185307179586;
	const double RAD = 0.0174532925199433;   //degree to radian
	const double DEG = 57.2957795130823;     //radian to degree
	
	//时间常量
	const double MJD2000 = 51544.5; // J2000.0的MJD(2000-1-1.5)
	const double MJD1950 = 33281.9234; // B1950.0的MJD(1949-12-31 22:09:42UT)
	const double MJD1900 = 15019.5;   // J1900.0的MJD(1900-1-1.5)
	const double MJD2JD = 2400000.5;   // MJD转换为JD的差
	
	//物理常量
	const double G = 6.67259e-20;        //gravitational constant(Km^3/Kg/s^2)
	const double LightVel = 299792.458;   //光速(km/s)
	const double AU = 1.49597870691e8;     //天文单位(km)
	const double SolarDay = 86400.0;   // 太阳日秒数
	const double StellarDay = 86164.09; // 恒星日秒数
	
	//地球相关常数
	const double GE = 398600.4418;     //WGS84  geocentric gravitational constant (km^3/s^2)
	const double Re = 6378.137;	       //WGS84  earth equatorial mean radius(km)
	const double Oblate = 0.00335281066433155; //WGS84  earth oblate(1/298.2572236)
	const double We = 7.2921159e-5;    //earth ratation rate(Rad/s)
	const double EarthMass = 5.974242e24;  // earth mass(kg)
	const double J2 = 0.0010826266835531513;
	const double J3 = -2.5326564853322355e-006;
	const double J4 = -1.6196215913670001e-006;
	const double A2 = 0.0016239400253297270;
	const double A3 = 2.5326564853322355e-006;
	const double A4 = 7.0858444622306256e-006;
	const double g = 9.80665;    // 地球表面的引力加速度常数(m/s^2)
	
	//太阳相关常数
	const double GS = 1.32712440018e+11;  //heliocentric gravitational constant (km^3/s^2)
	const double SunMass = 1.9889e30;  //太阳质量(kg)
	const double Rs = 695990.0;        //太阳半径(光球层?)(km)
	const double M_sunD = 1.99096875e-7; //太阳平近点角变化率(rad/s)
	
	//月球相关常数
	const double GM = 4902.801056;     //selenocentric gravitational constant (km^3/s^2)
	const double Rm = 1738.0;     // 月球平均赤道半径
	const double LunarGrvSph = 66200;	//月球影响球半径(km)
};

#endif  // __CONSTANT_H
