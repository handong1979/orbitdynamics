#include "ublas.h"

Vector Cross(const Vector &v1, const Vector &v2)
{
	Vector vo;
	vo[0] = v1[1]*v2[2] - v1[2]*v2[1];
	vo[1] = v1[2]*v2[0] - v1[0]*v2[2];
	vo[2] = v1[0]*v2[1] - v1[1]*v2[0];
	return vo;
}


void SetVectorValue(Vector& v, double d1,double d2,double d3)
{
	v[0] = d1;
	v[1] = d2;
	v[2] = d3;
}

ORBITDYN_API void SetVectorValue( Vector& v, double *da )
{
	v[0] = da[0];
	v[1] = da[1];
	v[2] = da[2];
}

/*!绕X轴转角angle的旋转矩阵
[ 1          0         0     ]
[ 0    cos(angle)  sin(angle)]
[ 0   -sin(angle)  cos(angle)]
\param angle 转过的角(弧度)
\return X旋转矩阵
*/
CosineMatrix RotationX(double angle)
{
	CosineMatrix m;
	m.clear();
	m(0,0) = 1.0;
	m(1,1) = cos(angle);
	m(2,2) = m(1,1);
	m(1,2) = sin(angle);
	m(2,1) = -m(1,2);
	return m;
}
/*!绕Y轴转角angle的旋转矩阵
[cos(angle)  0    -sin(angle)]
[     0      1         0     ]
[sin(angle)  0     cos(angle)]
\param angle 转过的角(弧度)
\return Y旋转矩阵
*/
CosineMatrix RotationY(double angle)
{
	CosineMatrix m;
	m.clear();
	m(0,0) = cos(angle);
	m(1,1) = 1.0;
	m(2,2) = m(0,0);
	m(0,2) = -sin(angle);
	m(2,0) = -m(0,2);
	return m;
}
/*!绕Z轴转角angle的旋转矩阵
[ cos(angle)  sin(angle)  0]
[-sin(angle)  cos(angle)  0]
[     0            0      1]
\param angle 转过的角(弧度)
\return Z旋转矩阵
*/
CosineMatrix RotationZ(double angle)
{
	CosineMatrix m;
	m.clear();
	m(0,0) = cos(angle);
	m(1,1) = m(0,0);
	m(2,2) = 1.0;
	m(0,1) = sin(angle);
	m(1,0) = -m(0,1);
	return m;
}
// 矩阵转置
CosineMatrix operator~(const CosineMatrix& m)
{
	return trans(m);
}


// 矩阵乘法
CosineMatrix operator*(const CosineMatrix& m,const CosineMatrix& n)
{
	return prod(m,n);
}

// 矩阵乘矢量
Vector operator*(const CosineMatrix& m,const Vector& n)
{
	return prod(m,n);
}
// 创建矢量
ORBITDYN_API Vector MakeVector( double x,double y,double z )
{
	Vector v;
	v[0] = x;
	v[1] = y;
	v[2] = z;
	return v;
}

// 创建矢量
ORBITDYN_API Vector MakeVector( double* d )
{
	Vector v;
	v[0] = d[0];
	v[1] = d[1];
	v[2] = d[2];
	return v;
}
