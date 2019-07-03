#include "Euler.h"
#include "Utility.h"
#include "Constant.h"
using namespace Constant;


CEuler::CEuler():
	seq(312),phi(0.0),theta(0.0),psi(0.0)
{

}

CEuler::CEuler(double a,double b,double c,const unsigned int s/*=312*/)
{
	seq = s;
	phi = a;
	theta = b;
	psi = c;
}

CEuler::CEuler( double* angle, const unsigned int s/*=312*/)
{
	seq = s;
	phi = angle[0];
	theta = angle[1];
	psi = angle[2];
}

CEuler::CEuler(const vec3 angle, const unsigned int s/*=312*/)
{
	seq = s;
	phi = angle[0];
	theta = angle[1];
	psi = angle[2];
}

CEuler::~CEuler()
{

}

void CEuler::SetValue(double a,double b,double c)
{
	phi = a;
	theta = b;
	psi = c;
}

//! 按照转序的定义生成余弦阵
mat33 CEuler::ToMatrix()
{
	mat33 A;
	switch(seq)
	{
	case 312:
		return RotationY(theta)*RotationX(phi)*RotationZ(psi);
		break;
	case 321:
		return RotationX(phi)*RotationY(theta)*RotationZ(psi);
		break;
	case 123:
		return RotationZ(psi)*RotationY(theta)*RotationX(phi);
		break;
	case 213:
		return RotationZ(psi)*RotationX(phi)*RotationY(theta);
		break;
	case 231:
		return RotationX(phi)*RotationZ(psi)*RotationY(theta);
		break;
	case 132:
		return RotationY(theta)*RotationZ(psi)*RotationX(phi);
		break;
	//else if( seq== 121 )
	//{
	//	A = RotationX(psi)*RotationY(theta)*RotationX(phi);
	//}
	//else if( seq== 131 )
	//{
	//	A = RotationX(psi)*RotationZ(psi)*RotationX(phi);
	//}
	//else if( seq== 212 )
	//{

	//}
	//else if( seq== 232 )
	//{

	//}
	//else if( seq== 313 )
	//{

	//}
	//else if( seq== 323 )
	//{

	//}
	default:
		throw CEulerException("EulerAngles.ToMatrix : Do NOT define such a rotation sequence!");
	}
	return A;
}	

//! 生成小角度下的姿态矩阵
mat33 CEuler::SmallAngleMatrix()
{
	mat33 m;
	m.eye();
	m(0,1) = psi;
	m(0,2) = -theta;
	m(1,0) = -psi;
	m(1,2) = phi;
	m(2,0) = theta;
	m(2,1) = -phi;
	return m;
}

void CEuler::SetValueFromMatrix312( const mat& m )
{
	seq = 312;
	if( fabs(m(1,2)) > 1 )
		phi = asin(m(1,2)/fabs(m(1,2)));
	else
		phi = asin(m(1,2));

	theta = atan2(-m(0,2),m(2,2));
	psi = atan2(-m(1,0),m(1,1));
}

void CEuler::SetValueFromMatrix123( const mat& m )
{
	if( fabs(m(2,0))>=1.0 )
		theta = asin(m(2,0)/fabs(m(2,0)));
	else
		theta = asin(m(2,0));

	psi = atan2(-m(1,0),m(0,0));
	phi = atan2(-m(2,1),m(2,2));
}

void CEuler::SetValueFromMatrix231( const mat& m )
{
	if( fabs(m(0,1))>=1.0 )
		psi = asin(m(0,1)/fabs(m(0,1)));
	else
		psi = asin(m(0,1));

	phi = atan2(-m(2,1),m(1,1));
	theta = atan2(-m(0,2),m(0,0));
}

void CEuler::SetValueFromMatrix132( const mat& m )
{
	phi = atan2(m(1,2),m(1,1));
	theta = atan2(m(2,0),m(0,0));
	if(fabs(m(1,0))>1) psi = asin(m(1,0)/fabs(m(1,0)));
	else psi = asin(m(1,0));
}

void CEuler::SetValueFromMatrix213( const mat& m )
{
	psi = atan2(m(0,1),m(1,1));
	theta = atan2(m(2,0),m(2,2));
	if(fabs(m(2,1))>1) phi = asin(m(2,1)/fabs(m(2,1)));
	else phi = asin(m(2,1));
}

void CEuler::SetValueFromMatrix321( const mat& m )
{
	phi = atan2(m(1,2),m(2,2));
	psi = atan2(m(0,1),m(0,0));
	if(fabs(m(2,0))>1) theta = asin(m(2,0)/fabs(m(2,0)));
	else theta = asin(m(2,0));
}

vec3 CEuler::ToVec()
{
	vec3 att;
	att[0] = phi;
	att[1] = theta;
	att[2] = psi;
	return att;
}

std::ostream & operator<<(std::ostream & os, const CEuler& euler)
{
	os<<euler.phi<<"\t"<<euler.theta<<"\t"<<euler.psi;
	return os;
}
