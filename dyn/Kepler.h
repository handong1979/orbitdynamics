#ifndef __KEPLER_H
#define __KEPLER_H

#if defined(_MSC_VER) && _MSC_VER < 1300
	#include <iostream.h>
	#include <fstream.h>
	#include <math.h>
	#include <stdexcpt.h>
#else
	#include <iostream>
	#include <cmath>
	using namespace std;
#endif	// _MSC_VER

#include "Config.h"
#include "Constant.h"
#include "BaseException.h"
using namespace Constant;

ORBITDYN_API  double KeplerFunc(double ee,double MM);
ORBITDYN_API  double KeplerFunc2(double ksi,double eta,double lamda);

inline double feE(double e,double E){
	return atan2(sqrt(1-e*e)*sin(E),cos(E)-e);
}

inline double raeE(double a,double e,double E){
	return a*(1-e*cos(E));
}

//! �����չ������
class ORBITDYN_API Kepler
{
public:
	//! Semi-major Axis(km)
	double a;
	//! Eccentricity
	double e;
	//! Inclination(rad)
	double i;
	//! Right Ascension of the Ascending Node (RAAN)(rad)
	double o;
	//! Argument of Perigee(rad)
	double w;
	//! Mean Anomaly(rad)
	double M;

	//! Ĭ�Ϲ���
	Kepler();

	//! ����ת��
	Kepler(const double oe[6]); 
	
	Kepler(double SemimajorAxis,double Eccentricity,double Inclination,
		double RAAN,double ArgPerigee,double MeanAnomaly);

	~Kepler();

	// output
	double Axis() const;
	double Ecce() const;
	double Incl() const;
	double RAAN() const;
	double AofP() const;
	double MeanA() const;
	double r() const;
	double v() const;
	double p() const;
	double f() const;
	double u() const;
	double E() const;
	double n(double miu = GE) const;
	double ApogeeRadius() const;
	double PerigeeRadius() const;
	double ApogeeAltitude(double r = Re) const;
	double PerigeeAltitude(double r = Re) const;

	double * ToArray(double array[6]);

	void SetElements(double SemimajorAxis,double Eccentricity,double inclination,
		double RAAN,double ArgPerigee,double MeanAnomaly);
	void SetElementsF(double SemimajorAxis,double Eccentricity,double inclination,
		double RAAN,double ArgPerigee,double TrueAnomaly);
	void SetElementsU(double SemimajorAxis,double Eccentricity,double inclination,
		double RAAN,double ArgPerigee,double Argument);
	void SetElements(const double elem[6]);
};

//! ��׼�����,�Ƕȵ�λΪ��
ORBITDYN_API std::ostream & operator << (std::ostream & out,const Kepler & kp);
//! ��׼������,�Ƕȵ�λΪ����
ORBITDYN_API std::istream & operator >> (std::istream & in,Kepler & kp);

class ORBITDYN_API KeplerException : public BaseException
{
public:
	KeplerException(const std::string& details = "") 
		: BaseException("KeplerException exception:",details)
	{
	}
	virtual ~KeplerException() _NOEXCEPT
	{
	}
	KeplerException(const KeplerException& cdte)
		:BaseException(cdte)
	{
	}
};

#endif // __KEPLER_H
