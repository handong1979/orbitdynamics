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
void Short2(const Kepler & MElem,double ZS[6],double & AS2);//�����������
ORBITDYN_API Kepler Mean(const Kepler & Inst);//˲����==>ƽ����
ORBITDYN_API Kepler Instant(const Kepler & MElem);//ƽ����==>˲����
ORBITDYN_API Kepler Cartesian_Kepler(const vec3 & Position,const vec3 & Velocity,double miu = GE);
ORBITDYN_API void Kepler_Cartesian(const Kepler& obl,vec3& Pos,vec3& Vel,double miu = GE);

// Perturbation Accelerations
ORBITDYN_API const vec3 PZonalHarmonic(const int LL,const vec3& SatPos,const CentralBody* cb = NULL);  //��г���㶯Zonal Harmonic Terms
ORBITDYN_API const vec3 PTesseralHarmonic(const int LL,const vec3& SatPos,const CentralBody* cb = NULL); //��г���㶯
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
ORBITDYN_API void Legendre_sphPl(const int LL,const double x,double PL[]);//���õö���ʽ
ORBITDYN_API void Legendre_sphPlm(const int LL,const double x,double PLM[][71]);//�޺����õö���ʽ
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
ORBITDYN_API double Sun_Eccentricity(const CDateTime& t);         // ̫����ƫ����
ORBITDYN_API double Ecliptic_Equator_Angle(const CDateTime& t);   // �Ƴཻ�Ǧ�
ORBITDYN_API double Sun_Argument_Perigee(const CDateTime& t);     // ̫����ص�ƽ�ƾ���
ORBITDYN_API double Sun_Mean_Anomaly(const CDateTime& t);         // ̫��ƽ�����M
ORBITDYN_API double Sun_Ecliptic_Longitude(const CDateTime& t);   // ̫��ƽ�ƾ�L
ORBITDYN_API double Moon_Argument_Perigee(const CDateTime& t);    // �׵����ص�ƽ�ƾ���
ORBITDYN_API double Moon_RAAN(const CDateTime& t);                // �׵�������ƽ�ƾ���
ORBITDYN_API double Moon_Ecliptic_Longitude(const CDateTime& t);  // ����ƽ�ƾ�L
ORBITDYN_API double Moon_Mean_Anomaly(const CDateTime& t);        // ����ƽ�����M
ORBITDYN_API double Moon_Sun_Ecliptic_Angle(const CDateTime& t);  // ������̫���ƽ���D

ORBITDYN_API const Kepler SunOrbit(const CDateTime& t);//̫������������
ORBITDYN_API const vec3 SunAnalytic(const CDateTime& t);//̫���������
ORBITDYN_API const vec3 MoonAnalytic(const CDateTime& t);//�����������
ORBITDYN_API void precession_angle(const CDateTime& t,double& zeta,double& theta,double& Z);   //���
ORBITDYN_API void nutation_angle(const CDateTime& t,double& dksi,double& deps);     //�¶�

// Orbit Maneuver
// ƽ����aew���Ͽ���
ORBITDYN_API void InitPlaneJet(const Kepler & init,const Kepler & destin,Maneuver& m1,Maneuver& m2,double distri=1);
// ���ƽ�����
ORBITDYN_API void InitNormalJet(const Kepler & init,const Kepler & destin,Maneuver& m1,Maneuver& m2,double distri=1);

// Relative Motion
//��֪�����ǡ��������λ�ú�����ٶ�
ORBITDYN_API void RIC(const Kepler& chaser,const Kepler& RefSat,vec3& RelPos,vec3& RelVel);
//��֪һ�����Ǻ����λ������ٶȣ�����һ�������ǵĹ������
ORBITDYN_API Kepler RIC(const Kepler & RefSat,const vec3 & relpos,const vec3 & relvel);

// Coordinate Transform
ORBITDYN_API mat RotationX(double angle);
ORBITDYN_API mat RotationY(double angle);
ORBITDYN_API mat RotationZ(double angle);
// ���Ĺ���ϵ==>���Ĺ���ϵ
ORBITDYN_API vec3 ECI_MCI(const CDateTime t,vec3 & r);
// ���Ĺ���ϵ==>���Ĺ���ϵ
ORBITDYN_API void ECI_MCI(const CDateTime& t,vec3 & r,vec3& v,vec3 & rm,vec3& vm);
// ����J2000����ϵ==>���ĳ�����Թ������
ORBITDYN_API Kepler MCI_MCI2Kepler(const CDateTime& t,vec3& r,vec3& v);
//ECI==>ECF  Matirx
ORBITDYN_API mat33 ECI_ECF(const CDateTime& t);
//MCI==>MCF  Matirx
ORBITDYN_API mat33 MCI_MCF(const CDateTime& t);
//���
ORBITDYN_API mat33 Precession(const CDateTime& t);
//�¶�  
ORBITDYN_API mat33 Nutation(const CDateTime& t);  
//������ת
ORBITDYN_API mat33 EarthRotation(const CDateTime& t);
//����  
ORBITDYN_API mat33 PolarMotion(const CDateTime& t); 
ORBITDYN_API CSpherical ECF_LLA(const vec3 & ECF);
// Coi����
ORBITDYN_API mat33 GetCoi(const Kepler& k);

// Greenwich Stellar Time
// �������������ʱ
ORBITDYN_API double GetSG(const CDateTime & t);
// ��������ƽ����ʱ
ORBITDYN_API double GetSGM(const CDateTime& t);

// Atmosphere Model
//	����1976��׼����ģ��	����: �߶�h(km) 	���: �����ܶ�(kg/m^3)
ORBITDYN_API double SA76(double h);


#if _MSC_VER < 1400
#define strcpy_s    strcpy
#define fscanf_s    fscanf
#define sscanf_s    sscanf
#define _strupr_s   strupr
void fopen_s(FILE** fp,const char* filename,const char* rw);
#endif // _MSC_VER < 1400

#endif //__UTILITY_H
