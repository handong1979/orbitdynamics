#ifndef __EULERANGLES_H
#define __EULERANGLES_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Config.h"
#include "BaseException.h"
#include "armadillo_BLAS_LAPACK.h"
using namespace arma;

class ORBITDYN_API CEulerException : public BaseException
{
public:
	CEulerException(const std::string& details = "") 
		: BaseException("CEuler exception:",details)
	{
	}
	virtual ~CEulerException()
	{
	}
	CEulerException(const CEulerException& cdte)
		:BaseException(cdte)
	{
	}
};

/*!欧拉角
*/
class ORBITDYN_API CEuler
{
public:
	//! 欧拉角转序
	unsigned int seq;
	//! 欧拉角
	double phi,theta,psi;

	CEuler();
	CEuler(double a,double b,double c,const unsigned int s = 312);
	CEuler(double*, const unsigned int s = 312);
	CEuler(const vec3, const unsigned int s = 312);
	~CEuler();

	vec3 ToVec();
	
	// 由姿态矩阵计算姿态角
	void SetValueFromMatrix312(const mat& m);
	void SetValueFromMatrix123(const mat& m);
	void SetValueFromMatrix231(const mat& m);
	void SetValueFromMatrix132(const mat& m);
	void SetValueFromMatrix213(const mat& m);
	void SetValueFromMatrix321(const mat& m);
	void SetValue(double a,double b,double c);

	//! 按照转序的定义生成余弦阵
	mat33 ToMatrix();
	
	//! 生成小角度下的姿态矩阵
	mat33 SmallAngleMatrix();

	double operator[](const int i) const{
		if(i==0)
			return phi;
		else if(i==1)
			return theta;
		else if(i==2)
			return psi;
		else
			throw CEulerException("index error");
	}

	double& operator[](const int i){
		if(i==0)
			return phi;
		else if(i==1)
			return theta;
		else if(i==2)
			return psi;
		else
			throw CEulerException("index error");
	}
};

ORBITDYN_API std::ostream & operator <<(std::ostream & os, const CEuler& euler);

#endif // __EULERANGLES_H
