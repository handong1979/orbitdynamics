#ifndef __UTILITY_H
#define __UTILITY_H

#include "Coordination.h"
#include "CentralBody.h"
#include "Maneuver.h"
#include "PlanetEphemeris.h"
#include "Kepler.h"


/************************************************************************/
/* Utilities                                                            */
/************************************************************************/

class CSatelliteBase;
class CSatellite;

std::string GetOrbitDynDir();
// Orbit Element
void Short2(const Kepler & MElem,double ZS[6],double & AS2);//计算短周期项
ORBITDYN_API Kepler Mean(const Kepler & Inst);//瞬根数==>平根数
ORBITDYN_API Kepler Instant(const Kepler & MElem);//平根数==>瞬根数
ORBITDYN_API Kepler Cartesian_Kepler(const Vector & Position,const Vector & Velocity,double miu = GE);
ORBITDYN_API void Kepler_Cartesian(const Kepler& obl,Vector& Pos,Vector& Vel,double miu = GE);

// Perturbation Accelerations
ORBITDYN_API const Vector PZonalHarmonic(const int LL,const Vector& SatPos,const CentralBody* cb = NULL);  //带谐项摄动Zonal Harmonic Terms
ORBITDYN_API const Vector PTesseralHarmonic(const int LL,const Vector& SatPos,const CentralBody* cb = NULL); //田谐项摄动
ORBITDYN_API const Vector PThirdBodyGrv(const Vector& SatVector,const Vector& ThirdBodyVector,double ThirdBodyGrvConst); // Third body gravitaional
ORBITDYN_API const Vector PLightPressure(const double CrSM,const Vector& r,const Vector& rs,double Radius = Re);
ORBITDYN_API const Vector PAirDrag(const CDateTime& t,const double CdSM,const Vector& ECFr,const Vector& r,const Vector& rd); // Air Drag
ORBITDYN_API const Vector PPostNewton(const Vector& SatVector,const Vector& SatVelocity);	//Post-Newton term
ORBITDYN_API double Shadow(const Vector& r,const Vector& rs,const double& Rcb = Re);

// Math
ORBITDYN_API double WhiteNoise(double sigma3,double mean);
ORBITDYN_API double EvenNoise(double min, double max);
ORBITDYN_API double RKF78(const CSatelliteBase& s,double h,double* x,double* xh,const int dim);
ORBITDYN_API double Gill4(const CSatelliteBase& s,double hh,double* x,double* xh,const int dim);
ORBITDYN_API void Legendre_sphPl(const int LL,const double x,double PL[]);//勒让得多项式
ORBITDYN_API void Legendre_sphPlm(const int LL,const double x,double PLM[][71]);//缔合勒让得多项式
ORBITDYN_API void SmxCmx(const int LL,const double S1X,const double C1X,double* SX,double* CX);
ORBITDYN_API inline double GG345(double b,double c) { return sqrt(b*b+c*c); }
ORBITDYN_API inline int sign(double x) { if(x==0) return 0; else return x>0 ? 1 : -1 ; }
ORBITDYN_API inline double Limit(double x) {	while(x>=PI) x=x-PI2;	while(x<-PI) x=x+PI2;	return x; }
ORBITDYN_API inline double mod(double x,double y){ return x-floor(x/y)*y; }
ORBITDYN_API Matrix Vector2Matrix(const Vector& x,const Vector& y,const Vector& z);
ORBITDYN_API void Matrix2Vector(const Matrix& m,Vector& x,Vector& y,Vector& z);

// Star
ORBITDYN_API double Sun_Eccentricity(const CDateTime& t);
ORBITDYN_API double Ecliptic_Equator_Angle(const CDateTime& t);   // 黄赤交角ε
ORBITDYN_API double Sun_Argument_Perigee(const CDateTime& t);     // 太阳近地点平黄经Γ
ORBITDYN_API double Sun_Mean_Anomaly(const CDateTime& t);         // 太阳平近点角M
ORBITDYN_API double Sun_Ecliptic_Longitude(const CDateTime& t);   // 太阳平黄经L
ORBITDYN_API double Moon_Argument_Perigee(const CDateTime& t);    // 白道近地点平黄经Γ
ORBITDYN_API double Moon_RAAN(const CDateTime& t);                // 白道升交点平黄经Ω
ORBITDYN_API double Moon_Ecliptic_Longitude(const CDateTime& t);  // 月球平黄经L
ORBITDYN_API double Moon_Mean_Anomaly(const CDateTime& t);        // 月球平近点角M
ORBITDYN_API double Moon_Sun_Ecliptic_Angle(const CDateTime& t);  // 月球与太阳的平距角D

ORBITDYN_API const Kepler SunOrbit(const CDateTime& t);//太阳解析轨道根数
ORBITDYN_API const Vector SunAnalytic(const CDateTime& t);//太阳解析星历
ORBITDYN_API const Vector MoonAnalytic(const CDateTime& t);//月球解析星历
ORBITDYN_API void precession_angle(const CDateTime& t,double& zeta,double& theta,double& Z);   //岁差
ORBITDYN_API void nutation_angle(const CDateTime& t,double& dksi,double& deps);     //章动
ORBITDYN_API Matrix Precession(const CDateTime& t);
ORBITDYN_API Matrix Nutation(const CDateTime& t);

// Orbit Maneuver
ORBITDYN_API void InitPlaneJet(const Kepler & init,const Kepler & destin,Maneuver& m1,Maneuver& m2,double distri=1);
ORBITDYN_API void InitNormalJet(const Kepler & init,const Kepler & destin,Maneuver& m1,Maneuver& m2,double distri=1);

// Relative Motion
ORBITDYN_API void RIC(const Kepler& chaser,const Kepler& RefSat,Vector& RelPos,Vector& RelVel);//已知两卫星、求其相对位置和相对速度
ORBITDYN_API Kepler RIC(const Kepler & RefSat,const Vector & relpos,const Vector & relvel);//已知一个卫星和相对位置相对速度，求另一个颗卫星的轨道根数

// Coordinate Transform
ORBITDYN_API const Vector ECI_VVLH(const Vector ECI,const CSatelliteBase & RefSat);//惯性系==>轨道系
ORBITDYN_API const Vector VVLH_ECI(const Vector VVLH,const CSatelliteBase & RefSat);//轨道系==>惯性系
ORBITDYN_API const Vector ECI_MCI(const CDateTime t,Vector & r); // 地心惯性系==>月心惯性系
ORBITDYN_API void ECI_MCI(const CDateTime& t,Vector & r,Vector& v,Vector & rm,Vector& vm);
ORBITDYN_API const Kepler MCI_MCI2Kepler(const CDateTime& t,Vector& r,Vector& v); // 月心J2000惯性系==>月心赤道惯性轨道根数
ORBITDYN_API Matrix ECI_ECF(const CDateTime& t);//ECI==>ECF  Matirx
ORBITDYN_API Matrix MCI_MCF(const CDateTime& t);//MCI==>MCF  Matirx
ORBITDYN_API Matrix Precession(const CDateTime& t);  //岁差
ORBITDYN_API Matrix Nutation(const CDateTime& t);  //章动
ORBITDYN_API Matrix EarthRotation(const CDateTime& t);  //地球自转
ORBITDYN_API Matrix PolarMotion(const CDateTime& t); //极移
ORBITDYN_API const CSpherical ECF_LLA(const Vector & ECF);
ORBITDYN_API Matrix GetCoi(const Kepler& k);

// Greenwich Stellar Time
ORBITDYN_API double GetSG(const CDateTime & t);
ORBITDYN_API double GetSGM(const CDateTime& t);
ORBITDYN_API double GetSGJ2000(const CDateTime& t);

// Atmosphere Model
//	美国1976标准大气模型	输入: 高度h(km) 	输出: 大气密度(kg/m^3)
ORBITDYN_API double SA76(double h);


#if _MSC_VER < 1400
#define strcpy_s    strcpy
#define fscanf_s    fscanf
#define sscanf_s    sscanf
#define _strupr_s   strupr
void fopen_s(FILE** fp,const char* filename,const char* rw);
#endif // _MSC_VER < 1400

#endif //__UTILITY_H
