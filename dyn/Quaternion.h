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

#include "config.h"
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
	virtual ~CQuaternionException()
	{
	}
	CQuaternionException(const CQuaternionException& cdte)
		:BaseException(cdte)
	{
	}
};

/*!四元数
*/
class ORBITDYN_API CQuaternion
{
public:
	double qx,qy,qz,qs;

	/*!默认构造函数*/
	CQuaternion();

	/*!构造函数*/
	CQuaternion(double x,double y,double z,double s);
	
	/*!用转轴、转角构造*/
	CQuaternion(const vec3& e,double phi);

	/*!用数组构造*/
	CQuaternion(double *);

	/*!用方向余弦阵构造*/
	CQuaternion(const mat& c);

	/*!析构函数*/
	~CQuaternion(){}

	/*!四元数的范数*/
	double Norm()const;

	/*! 操作符[]取值 */
	double & operator[](int i);

	/*! 操作符[]赋值 */
	double operator[](int i) const;

	/*!四元数规范化*/
	void Normalize();
	
	/*!求一定转序下的欧拉角*/
	void EulerAngles(char* seq,double& phi,double& theta,double& ksi) const;
	
	/*!四元数对应的方向余弦阵*/
	mat33 C() const;

	/*!矢量部分*/
	vec3 v() const;
	
	/*!Omega矩阵*/
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
