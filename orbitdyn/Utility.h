#ifndef __UTILITY_H
#define __UTILITY_H

#include "Coordination.h"
#include "CentralBody.h"
#include "Maneuver.h"
#include "PlanetEphemeris.h"
#include "kepler.h"


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
ORBITDYN_API Kepler Cartesian_Kepler(const vec3 & Position,const vec3 & Velocity,double miu = GE);
ORBITDYN_API void Kepler_Cartesian(const Kepler& obl,vec3& Pos,vec3& Vel,double miu = GE);

// Perturbation Accelerations
ORBITDYN_API const vec3 PZonalHarmonic(const int LL,const vec3& SatPos,const CentralBody* cb = NULL);  //带谐项摄动Zonal Harmonic Terms
ORBITDYN_API const vec3 PTesseralHarmonic(const int LL,const vec3& SatPos,const CentralBody* cb = NULL); //田谐项摄动
ORBITDYN_API const vec3 PThirdBodyGrv(const vec3& SatVector,const vec3& ThirdBodyVector,double ThirdBodyGrvConst); // Third body gravitaional
ORBITDYN_API const vec3 PLightPressure(const double CrSM,const vec3& r,const vec3& rs,double Radius = Re);
ORBITDYN_API const vec3 PAirDrag(const CDateTime& t,const double CdSM,const vec3& ECFr,const vec3& r,const vec3& rd); // Air Drag
ORBITDYN_API const vec3 PPostNewton(const vec3& SatVector,const vec3& SatVelocity);	//Post-Newton term
ORBITDYN_API double Shadow(const vec3& r,const vec3& rs,const double& Rcb = Re);

// Math
ORBITDYN_API double WhiteNoise(double sigma3,double mean);
ORBITDYN_API double EvenNoise(double min, double max);
ORBITDYN_API double RKF78(CSatelliteBase& s,double h,double* x,double* xh,const int dim);
ORBITDYN_API double Gill4(CSatelliteBase& s,double hh,double* x,double* xh,const int dim);
ORBITDYN_API void Legendre_sphPl(const int LL,const double x,double PL[]);//勒让得多项式
ORBITDYN_API void Legendre_sphPlm(const int LL,const double x,double PLM[][71]);//缔合勒让得多项式
ORBITDYN_API void SmxCmx(const int LL,const double S1X,const double C1X,double* SX,double* CX);
ORBITDYN_API inline double GG345(double b,double c) { return sqrt(b*b+c*c); }
ORBITDYN_API inline int sign(double x) { if(x==0) return 0; else return x>0 ? 1 : -1 ; }
ORBITDYN_API inline double maxabs3( double a, double b, double c );
ORBITDYN_API inline double max3( double a, double b, double c );
ORBITDYN_API inline double Limit(double x) {	while(x>=PI) x=x-PI2;	while(x<-PI) x=x+PI2;	return x; }
#define modpi Limit
ORBITDYN_API inline double mod(double x,double y){ return x-floor(x/y)*y; }
ORBITDYN_API mat33 Vector2Matrix(const vec3& x,const vec3& y,const vec3& z);
ORBITDYN_API mat33 out_product(const vec3 &v1,const vec3& v2);
ORBITDYN_API mat33 Avv( vec3 X1I, vec3 X2I, vec3 X1B, vec3 X2B );

// Star
ORBITDYN_API double Sun_Eccentricity(const CDateTime& t);         // 太阳轨道偏心率
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
ORBITDYN_API const vec3 SunAnalytic(const CDateTime& t);//太阳解析星历
ORBITDYN_API const vec3 MoonAnalytic(const CDateTime& t);//月球解析星历
ORBITDYN_API void precession_angle(const CDateTime& t,double& zeta,double& theta,double& Z);   //岁差
ORBITDYN_API void nutation_angle(const CDateTime& t,double& dksi,double& deps);     //章动

// Orbit Maneuver
// 平面内aew联合控制
ORBITDYN_API void InitPlaneJet(const Kepler & init,const Kepler & destin,Maneuver& m1,Maneuver& m2,double distri=1);
// 轨道平面控制
ORBITDYN_API void InitNormalJet(const Kepler & init,const Kepler & destin,Maneuver& m1,Maneuver& m2,double distri=1);

// Relative Motion
//已知两卫星、求其相对位置和相对速度
ORBITDYN_API void RIC(const Kepler& chaser,const Kepler& RefSat,vec3& RelPos,vec3& RelVel);
//已知一个卫星和相对位置相对速度，求另一个颗卫星的轨道根数
ORBITDYN_API Kepler RIC(const Kepler & RefSat,const vec3 & relpos,const vec3 & relvel);

// Coordinate Transform
ORBITDYN_API mat RotationX(double angle);
ORBITDYN_API mat RotationY(double angle);
ORBITDYN_API mat RotationZ(double angle);
// 地心惯性系==>月心惯性系
ORBITDYN_API vec3 ECI_MCI(const CDateTime t,vec3 & r);
// 地心惯性系==>月心惯性系
ORBITDYN_API void ECI_MCI(const CDateTime& t,vec3 & r,vec3& v,vec3 & rm,vec3& vm);
// 月心J2000惯性系==>月心赤道惯性轨道根数
ORBITDYN_API Kepler MCI_MCI2Kepler(const CDateTime& t,vec3& r,vec3& v);
//ECI==>ECF  Matirx
ORBITDYN_API mat33 ECI_ECF(const CDateTime& t);
//MCI==>MCF  Matirx
ORBITDYN_API mat33 MCI_MCF(const CDateTime& t);
//岁差
ORBITDYN_API mat33 Precession(const CDateTime& t);
//章动  
ORBITDYN_API mat33 Nutation(const CDateTime& t);  
//地球自转
ORBITDYN_API mat33 EarthRotation(const CDateTime& t);
//极移  
ORBITDYN_API mat33 PolarMotion(const CDateTime& t); 
ORBITDYN_API CSpherical ECF_LLA(const vec3 & ECF);
// Coi矩阵
ORBITDYN_API mat33 GetCoi(const Kepler& k);

// Greenwich Stellar Time
// 格林威治真恒星时
ORBITDYN_API double GetSG(const CDateTime & t);
// 格林威治平恒星时
ORBITDYN_API double GetSGM(const CDateTime& t);

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
