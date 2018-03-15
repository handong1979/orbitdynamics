#pragma once

#include <Vector.h>
#include <MatrixTCL.h>
#include <stdexcept>

class PosVel
{
public:
	double x,y,z,vx,vy,vz;
	PosVel()
		:x(0),y(0),z(0),vx(0),vy(0),vz(0)
	{
	}
	PosVel(double d1,double d2,double d3,double d4,double d5,double d6)
		:x(d1),y(d2),z(d3),vx(d4),vy(d5),vz(d6)
	{
	}
	PosVel(const Vector &p,const Vector &v)
		:x(p.x),y(p.y),z(p.z),vx(v.x),vy(v.y),vz(v.z)
	{
	}
	PosVel(double dp[6])
		:x(dp[0]),y(dp[1]),z(dp[2]),vx(dp[3]),vy(dp[4]),vz(dp[5])
	{
	}
	PosVel(const PosVel & pv)
		:x(pv.x),y(pv.y),z(pv.z),vx(pv.vx),vy(pv.vy),vz(pv.vz)
	{
	}
	~PosVel(){}
	PosVel& operator=(const PosVel& a)
	{
		x = a.x;		y = a.y;		z = a.z;
		vx = a.vx;		vy = a.vy;		vz = a.vz;
		return *this;
	}

	void SetByM(const Matrix& m)
	{
		if(m.RowNo() != 6 || m.ColNo() != 1)
			throw std::range_error("PosVel operator=(Matrix): matrix is not 6*1");
		x = m(0,0);		y = m(1,0);		z = m(2,0);
		vx = m(3,0);		vy = m(4,0);		vz = m(5,0);
	}

	double& operator[](int i)
	{
		switch(i)
		{
		case 0: return x;
		case 1: return y;
		case 2: return z;
		case 3: return vx;
		case 4: return vy;
		case 5: return vz;
		default:
			throw std::range_error("PosVel operator[]: wrong index");
		}
	}

	double operator[](int i) const
	{
		switch(i)
		{
		case 0: return x;
		case 1: return y;
		case 2: return z;
		case 3: return vx;
		case 4: return vy;
		case 5: return vz;
		default:
			throw std::range_error("PosVel operator[]: wrong index");
		}
	}

	Vector Pos() const {
		return Vector(x,y,z);
	}

	Vector Vel() const {
		return Vector(vx,vy,vz);
	}

	void SetPos(const Vector r){
		x = r.x;
		y = r.y;
		z = r.z;
	}

	void SetVel(const Vector v){
		vx = v.x;
		vy = v.y;
		vz = v.z;
	}
};

//PosVel operator+(const PosVel& a,const PosVel& b)
//{
//	PosVel o;
//	o.x = a.x + b.x;
//	o.y = a.y + b.y;
//	o.z = a.z + b.z;
//	o.vx = a.vx + b.vx;
//	o.vy = a.vy + b.vy;
//	o.vz = a.vz + b.vz;
//	return o;
//}