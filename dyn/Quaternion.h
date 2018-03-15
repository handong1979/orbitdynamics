#ifndef __QUATERNION_H
#define __QUATERNION_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if defined _MSC_VER && _MSC_VER < 1300
	#include <iostream.h>
	#include <fstream.h>
	#include <math.h>
	#include <stdexcpt.h>
#else
	#include <iostream>
	#include <cmath>
	#include <stdexcept>
	using namespace std;
#endif // _MSC_VER

#include "Config.h"
#include "BaseException.h"
#include "armadillo_BLAS_LAPACK.h"
using namespace arma;

class ORBITDYN_API CQuaternionException : public BaseException
{
public:
	CQuaternionException(const std::string& details = "") 
		: BaseException("CEuler exception:",details)
	{
	}
	virtual ~CQuaternionException() _NOEXCEPT
	{
	}
	CQuaternionException(const CQuaternionException& cdte)
		:BaseException(cdte)
	{
	}
};

/*!��Ԫ��
*/
class ORBITDYN_API CQuaternion
{
public:
	double qx,qy,qz,qs;

	/*!Ĭ�Ϲ��캯��*/
	CQuaternion();

	/*!���캯��*/
	CQuaternion(double x,double y,double z,double s);
	
	/*!��ת�ᡢת�ǹ���*/
	CQuaternion(const vec3& e,double phi);

	/*!�����鹹��*/
	CQuaternion(double *);

	/*!�÷�����������*/
	CQuaternion(const mat& c);

	/*!��������*/
	~CQuaternion(){}

	/*!��Ԫ���ķ���*/
	double Norm()const;

	/*! ������[]ȡֵ */
	double & operator[](int i);

	/*! ������[]��ֵ */
	double operator[](int i) const;

	/*!��Ԫ���淶��*/
	void Normalize();
	
	/*!��һ��ת���µ�ŷ����*/
	void EulerAngles(char* seq,double& phi,double& theta,double& ksi) const;
	
	/*!��Ԫ����Ӧ�ķ���������*/
	mat33 C() const;

	/*!ʸ������*/
	vec3 v() const;
	
	/*!Omega����*/
	mat Eq() const;
	
	void QInt(const vec3& wbi,double dt);

	vec4 qv() const;
};

/*!Elements wise operator*/
ORBITDYN_API const CQuaternion operator+(const CQuaternion& q1,const CQuaternion& q2);
ORBITDYN_API const CQuaternion operator-(const CQuaternion& q1,const CQuaternion& q2);
ORBITDYN_API const CQuaternion operator*(const CQuaternion& q1,double& val);

/*!Quaternion operator*/
ORBITDYN_API const CQuaternion operator-(const CQuaternion &q);
ORBITDYN_API const CQuaternion operator*(const CQuaternion& q1,const CQuaternion& q2);
ORBITDYN_API const CQuaternion operator/(const CQuaternion& q2,const CQuaternion& q1);
ORBITDYN_API const CQuaternion Qm(const CQuaternion& q1,const CQuaternion& q2);
ORBITDYN_API const CQuaternion Qim(const CQuaternion& q1,const CQuaternion& q2);
ORBITDYN_API const vec3 operator*(const CQuaternion& q,const vec3& v);

/*!stream*/
ORBITDYN_API ostream & operator<<(ostream & os, const CQuaternion& q);
ORBITDYN_API istream & operator>>(istream & is, CQuaternion& q);

#endif // __QUATERNION_H
