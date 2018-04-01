#ifndef __COORDINATION_H
#define __COORDINATION_H

#include "Config.h"
#include "armadillo_BLAS_LAPACK.h"
using namespace arma;
#include <cstdio>

//! 笛卡尔直角坐标
class ORBITDYN_API CCartesian
{
public:
	double x,y,z,dx,dy,dz;

	CCartesian(void):x(0),y(0),z(0),dx(0),dy(0),dz(0) {	}
	CCartesian(double d1,double d2,double d3,double d4,double d5,double d6)
		: x(d1), y(d2), z(d3), dx(d4), dy(d5), dz(d6) { }
		CCartesian(double * arg)
		{
			memcpy(&x,arg,sizeof(double)*6);
		}
		CCartesian(vec3 r,vec3 v){
			x=r(0); y=r(1); z=r(2); dx=v(0); dy=v(1); dz=v(2);
			}
		~CCartesian(void){ }
};

//! 球坐标
class ORBITDYN_API CSpherical
{
public:
	double Longitude,Latitude,Altitude;
	CSpherical():Longitude(0),Latitude(0),Altitude(0){ }
	CSpherical(double lon,double lat,double alt): Longitude(lon),Latitude(lat),Altitude(alt) { }
	~CSpherical(){ }
};

enum Coordination { ECI, VVLH, VNC, ECF};

#endif // __COORDINATION_H
