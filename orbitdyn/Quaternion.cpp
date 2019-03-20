#include "Quaternion.h"

CQuaternion::CQuaternion(const vec3& e,double phi)
{
	double me = norm(e,2);
	vec3 ee;
	if(me!=0)
		ee = e/me;
	else
		ee = e;
	double sphi = sin(phi/2.0);
	qx = ee(0) * sphi;
	qy = ee(1) * sphi;
	qz = ee(2) * sphi;
	qs = cos(phi/2.0);
}

CQuaternion::CQuaternion(double * v)
{
	qx = v[0];
	qy = v[1];
	qz = v[2];
	qs = v[3];
}

CQuaternion::CQuaternion(const mat& c)
{
	// 用vector初始化
	if((c.n_rows==1 && c.n_cols==4) || (c.n_cols==1 && c.n_rows==4))
	{
		qx = c[0];
		qy = c[1];
		qz = c[2];
		qs = c[3];
		return;
	}
	else if(c.n_cols!=3 || c.n_rows!=3) 
		throw CQuaternionException("CQuaternion(mat& c) input must be a 3*3 matrix");

	// 用矩阵初始化
	double trc1 = c(0,0) + c(1,1) + c(2,2) + 1.0;
	if( trc1 >= 0.004 )
	{
		qs = sqrt(trc1)/2.0;
		qx = (c(1,2) - c(2,1))/4.0/qs;
		qy = (c(2,0) - c(0,2))/4.0/qs;
		qz = (c(0,1) - c(1,0))/4.0/qs;
	}
	else if( 1.0 - c(0,0) + c(1,1) - c(2,2) >= 0.004 )
	{
		qy = sqrt(1.0 - c(0,0) + c(1,1) - c(2,2)) / 2.0 * (c(2,0)-c(0,2)>=0?1:-1);
		qx = (c(1,0) + c(0,1))/4.0/qy;
		qz = (c(2,1) + c(1,2))/4.0/qy;
		qs = (c(2,0) - c(0,2))/4.0/qy;
	}
	else if( 1.0 + c(0,0) - c(1,1) - c(2,2) >= 0.004 )
	{
		qx = sqrt(1.0 + c(0,0) - c(1,1) - c(2,2)) / 2.0 * (c(1,2)-c(2,1)>=0?1:-1);
		qy = (c(1,0) + c(0,1))/4.0/qx;
		qz = (c(0,2) + c(2,0))/4.0/qx;
		qs = (c(1,2) - c(2,1))/4.0/qx;
	}
	else
	{
		qz = sqrt(1.0 - c(0,0) - c(1,1) + c(2,2)) / 2.0 * (c(0,1)-c(1,0)>=0?1:-1);
		qx = (c(0,2) + c(2,0))/4.0/qz;
		qy = (c(1,2) + c(2,1))/4.0/qz;
		qz = (c(0,1) - c(1,0))/4.0/qz;
	}
	Normalize();
}

//! 操作符[]取值
double & CQuaternion::operator[](int i)
{
	if(i==0) return qx;
	else if(i==1) return qy;
	else if(i==2) return qz;
	else if(i==3) return qs;
	else
		throw CQuaternionException("index error!");
}
//! 操作符[]赋值
double CQuaternion::operator[](int i) const
{
	if(i==0) return qx;
	else if(i==1) return qy;
	else if(i==2) return qz;
	else if(i==3) return qs;
	else
		throw CQuaternionException("index error!");
}

double CQuaternion::Norm() const
{
	return sqrt(qx*qx+qy*qy+qz*qz+qs*qs);
}

void CQuaternion::Normalize()
{
	double nrm = Norm();
	if ( qs > 0 )
	{
		qx = qx/nrm;
		qy = qy/nrm;
		qz = qz/nrm;
		qs = qs/nrm;
	}
	else // qs<0
	{
		qx = -qx/nrm;
		qy = -qy/nrm;
		qz = -qz/nrm;
		qs = -qs/nrm;
	}
}

mat33 CQuaternion::C() const
{
	mat33 c;
	c(0,0) = 1.0 - 2.0*(qy*qy+qz*qz);
	c(0,1) = 2.0*(qx*qy+qz*qs);
	c(0,2) = 2.0*(qx*qz-qy*qs);
	c(1,0) = 2.0*(qx*qy-qz*qs);
	c(1,1) = 1.0 - 2.0*(qx*qx+qz*qz);
	c(1,2) = 2.0*(qy*qz+qx*qs);
	c(2,0) = 2.0*(qx*qz+qy*qs);
	c(2,1) = 2.0*(qy*qz-qx*qs);
	c(2,2) = 1.0 - 2.0*(qx*qx+qy*qy);
	return c;
}

vec3 CQuaternion::v() const
{
	vec3 v;
	v(0) = qx;
	v(1) = qy;
	v(2) = qz;
	return v;
}

mat CQuaternion::Eq() const
{
	mat e(4,3);
	e(0,0) =  qs;  e(0,1) = -qz;  e(0,2) =  qy;
	e(1,0) =  qz;  e(1,1) =  qs;  e(1,2) = -qx;
	e(2,0) = -qy;  e(2,1) =  qx;  e(2,2) =  qs;
	e(3,0) = -qx;  e(3,1) = -qy;  e(3,2) = -qz;
	return e;
}

void CQuaternion::QInt(const vec3& wbi,double dt)
{
	vec4 E = Eq()*wbi*dt;
	qx += 0.5*E(0);
	qy += 0.5*E(1);
	qz += 0.5*E(2);
	qs += 0.5*E(3);
	Normalize();
}

vec4 CQuaternion::qv() const
{
	vec4 q;
	q[0] = qx;
	q[1] = qy;
	q[2] = qz;
	q[3] = qs;
	return q;
}

//////////////////////////////////////////////////////////////////////////
// 非成员函数
const CQuaternion operator+(const CQuaternion& q1,const CQuaternion& q2)
{
	CQuaternion q;
	q.qx = q1.qx + q2.qx;
	q.qy = q1.qy + q2.qy;
	q.qz = q1.qz + q2.qz;
	q.qs = q1.qs + q2.qs;
	return q;
}

const CQuaternion operator-(const CQuaternion& q1,const CQuaternion& q2)
{
	CQuaternion q;
	q.qx = q1.qx - q2.qx;
	q.qy = q1.qy - q2.qy;
	q.qz = q1.qz - q2.qz;
	q.qs = q1.qs - q2.qs;
	return q;
}

const CQuaternion operator*(const CQuaternion& q1,double& val)
{
	CQuaternion q;
	q.qx = q1.qx*val;
	q.qy = q1.qy*val;
	q.qz = q1.qz*val;
	q.qs = q1.qs*val;
	return q;
}

const CQuaternion operator-(const CQuaternion &q)
{
	return CQuaternion(-q.qx,-q.qy,-q.qz,-q.qs);
}

const CQuaternion operator*(const CQuaternion& q1,const CQuaternion& q2)
{
	CQuaternion q(q2.qs*q1.qx + q2.qz*q1.qy - q2.qy*q1.qz + q2.qx*q1.qs,
		-q2.qz*q1.qx + q2.qs*q1.qy + q2.qx*q1.qz + q2.qy*q1.qs,
		q2.qy*q1.qx - q2.qx*q1.qy + q2.qs*q1.qz + q2.qz*q1.qs,
		-q2.qx*q1.qx - q2.qy*q1.qy - q2.qz*q1.qz + q2.qs*q1.qs);
	q.Normalize();
	return q;
}

const CQuaternion operator/(const CQuaternion& q2,const CQuaternion& q1)
{
	CQuaternion q(-q2.qs*q1.qx - q2.qz*q1.qy + q2.qy*q1.qz + q2.qx*q1.qs,
		q2.qz*q1.qx - q2.qs*q1.qy - q2.qx*q1.qz + q2.qy*q1.qs,
		-q2.qy*q1.qx + q2.qx*q1.qy - q2.qs*q1.qz + q2.qz*q1.qs,
		q2.qx*q1.qx + q2.qy*q1.qy + q2.qz*q1.qz + q2.qs*q1.qs);
	q.Normalize();
	return q;
}

const CQuaternion Qm(const CQuaternion& q1,const CQuaternion& q2)
{
	return q1*q2;
}

const CQuaternion Qim(const CQuaternion& q1,const CQuaternion& q2)
{
	return q2/q1;
}

const vec3 operator*(const CQuaternion& q,const vec3& v)
{
	vec3 vv;
	vv(0) = (1.0 - 2.0*(q.qy*q.qy+q.qz*q.qz))*v(0) + 2.0*(q.qx*q.qy+q.qz*q.qs)*v(1) + 2.0*(q.qx*q.qz-q.qy*q.qs)*v(2);
	vv(1) = 2.0*(q.qx*q.qy-q.qz*q.qs)*v(0) + (1.0 - 2.0*(q.qx*q.qx+q.qz*q.qz))*v(1) + 2.0*(q.qy*q.qz+q.qx*q.qs)*v(2);
	vv(2) = 2.0*(q.qx*q.qz+q.qy*q.qs)*v(0) + 2.0*(q.qy*q.qz-q.qx*q.qs)*v(1) + (1.0 - 2.0*(q.qx*q.qx+q.qy*q.qy))*v(2);
	return vv;
}

ostream & operator<<(ostream & os, const CQuaternion& q)
{
	os<<q.qx<<"\t"<<q.qy<<"\t"<<q.qz<<"\t"<<q.qs;
	return os;
}

istream & operator>>(istream & is, CQuaternion& q)
{
	is>>q.qx>>q.qy>>q.qz>>q.qs;
	return is;
}
