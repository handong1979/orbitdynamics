#ifndef __SATELLITE_H
#define __SATELLITE_H

// C++ common head file
#if defined(_MSC_VER) && _MSC_VER < 1300
	#include <iostream.h>
	#include <fstream.h>
	#include <math.h>
	#include <float.h>
	#include <list>
	#include <queue>
#else
	#include <fstream>
	#include <iostream>
	using namespace std;
	#include <cmath>
	#include <limits>
	#include <list>
	#include <queue>
	#include <stdexcept>
#endif

#include "Constant.h"
#include "Config.h"
#include "CEngine.h"
#include "CentralBody.h"
#include "PlanetEphemeris.h"
#include "Coordination.h"
#include "StopCondition.h"
#include "Maneuver.h"
#include "Quaternion.h"
#include "Kepler.h"

// �㶯�����ö���

// ��������
#define ODP_EARTH_ZONAL             0x01  // �����г��
#define ODP_EARTH_TESSERAL          0x02  // ������г��
#define ODP_AIR_DRAG                0x04  // ��������
#define ODP_LUNAR_CENT              0x08  // ������������(�Է�������������)
#define ODP_SOLAR_CENT              0x10  // ̫����������(�Է����Ĺ��)
#define ODP_SOLAR_PRESSURE          0x20  // ̫���ѹ
#define ODP_EARTH_ALBEDO            0x40  // �����չ�ѹ
#define ODP_SOLID_TIDES             0x80  // ������峱
#define ODP_OCEAN_TIDES             0x0100 // ���򺣳�
#define ODP_POSTNEWTON              0x0200  // ��ţ��ЧӦ
#define ODP_ADDITIONAL              0x0400  // ����ϵ�����㶯
#define ODP_THERMAL_RADIATION_PRESSURE 0x0800 // �ȷ���ѹ
// ��������
#define ODP_LUNAR_ZONAL        0x1000 // �����г��
#define ODP_LUNAR_TESSERAL     0x2000 // ������г��
#define ODP_LUNAR_NON_SPHERE   0x3000 // �������������(��г��+��г��)
#define ODP_EARTH_CENT         0x4000 // ������������(�Էǵ�����������)
#define ODP_LUNAR_TIDES        0x8000 // ������峱
#define ODP_LUNAR_LIBRATION    0x010000 // ����������ƽ��
// ���̽����
#define ODP_VENUS_CENT         0x100000 // ������������
#define ODP_MARS_CENT          0x200000 // ������������
#define ODP_JUPITER_CENT       0x400000 // ľ����������
#define ODP_SATURN_CENT        0x800000 // ������������
// ���ü���
#define ODP_EARTH_ALL  0x0FFF // ���������������֮��������㶯��
#define ODP_GEO        0x0FFB // ͬ���������ͨ�����ǵ��㶯��
#define ODP_LEO        0x0007 // �͹������ͨ�����ǵ��㶯��
#define ODP_LUNAR_SAT  0x1F030 // ��������

/*!��̬ģʽ
\enum AttitudeMode
EarthPoint: �����Եض�����̬\n
SunPoint: ���ն�����̬\n
InertiallyFixed: ���Զ�����̬\n
Custom: �Լ�����
*/
enum AttitudeMode{ EarthPoint = 0, SunPoint , InertiallyFixed , Custom };

/*! ���ǵ��������
\author ����
CSatelliteBase�����������ǵ��������

�������ǵĻ���������
- ʱ��(��Է����ʼ��ԪStartEpoch)��ʱ��(����)
- ������������ϵ��λ���ٶ�,���Ĺ���ϵ����ΪJ2000.0,���Ĺ���ϵ����Ϊ����ƽ��Ϊ��������,xΪJ2000.0���������ϵ�ͶӰ
- �������ƣ������������Ǻʹ洢�����������ļ���
- ���ǵĹ�������������������
- ���Ƕ���ѧ���ԣ����ʱȡ����������䡢��������

������ʵ�ֵĺ�����ҪΪȡֵ���������RKF78�����㷨�����⺯��Ϊ���Ƕ���ѧ���̺͹�������ļ���

�㶯Դ��ѡ��ʹ��SetForce(int l,unsigned char PTB)����ʵ�֣�����lΪ������������PTBΪ�㶯Դѡ��������������º궨�壺
\code
#define ODP_ZONAL      0x80   // ��г��
#define ODP_TESSERAL   0xC0   // ��г��+��г��
#define ODP_LUNAR      0x20   // ��������
#define ODP_SOLAR      0x10   // ̫������
#define ODP_LIGHT      0x08   // ̫���ѹ
#define ODP_AIR        0x04   // ��������
#define ODP_POSTNEWTON 0x02   // ��ţ��ЧӦ
#define ODP_LUNAR_NON_SPHERE  0x01  // �������������

#define ODP_ALL        0xFE   // ���������������֮��������㶯��
#define ODP_GEO        0xF4   // ͬ���������ͨ�����ǵ��㶯������г�������������ѹ����ţ��ЧӦ
#define ODP_LEO        0x08   // �͹������ͨ�����ǵ��㶯������������
\endcode
PTBΪһ��8bit��־�֣���8λ�ֱ��ʾ��\n
||1||2||3||4||5||6||7||8
|Zonal��г��|Zonal+Tesseral��г���г��|Lunar��������|Solar̫������|Light̫���ѹ|Air��������|PostN��ţ��ЧӦ|Lunar_nonsphere�������������
*/
class ORBITDYN_API CSatelliteBase
{
protected:
	//! ��ʼ��Ԫ
	CDateTime Epoch; //TODO: ����Ӧ����TDTʱ�䣬��Ҫ��UTC!!
	//! �����ʼ��Ԫ��ʱ��
	double ElapsedTime;
	//!���������ϵλ��
	vec3 Position;
	//!���������ϵ�ٶ�
	vec3 Velocity;
	//!������
	PLANET center;
public:
	//!�������ƣ������������ݣ�Ĭ��Ϊa��b��c......
	std::string Name;
	//!��ʼ�������
	Kepler Status0;
	//!������ʼ����(��λ:kg)(Ĭ��1000kg)
	double Mass0;
	//! ����ȼ������(��λ:kg)(��ֵ0kg)
	double BurnedFuel;
	//!��������ϵ��
	double Cd;
	//!ӭ�����
	double AirDragArea;
	//!̫���ѹ������
	double Cr;
	//!��ѹ��Ч���
	double LightPressArea;

	//!������״̬��־(true:����,false:�ػ�)
	bool ThrustIsOn;
	//! �����ڱ���ϵ�ķ���(һ��Ϊ��λʸ��)
	vec3 ThrustDirection;
	//! ������̬��Ԫ��
	CQuaternion qbi;
	//! ������̬���ٶ�
	vec3 wbi;

	// ״̬:
	//!˲ʱ�����İ볤��(unit��km)
	double a;
	//!˲ʱ������ƫ����
	double e;
	//!˲ʱ�����Ĺ�����(unit:rad)
	double i;
	//!˲ʱ����������ྭ(unit:rad)
	double Omega;
	//!˲ʱ�������ص����(unit:rad)
	double w;
	//!˲ʱ����ƽ�����(unit:rad)
	double M;

	//!˲ʱ������ƽ����������(unit:rad)
	double f;
	//!˲ʱ������ƽ����ƫ�����(unit:rad)
	double E;
	//!˲ʱ������Ƿ���u=f+w     (unit:rad)
	double u;
	//!ƽ�����ٶ�n=sqrt(GE/a/a/a)   (unit:rad/s)
	double n;
	//! ʸ��
	double r;
	//! �ٶ�
	double v;
	//! ������ٶ�
	double w0;
	//! ����ϵ�����ϵת������
	mat33 Coi;
	//! ���ٶ�
	vec3 acc;

/////// ����������� //////////////////////////

	//! ���ǳ�ʼ��Ԫ
	inline CDateTime StartEpoch() const { return Epoch; }
	
	//! ���ǵ�ǰ��Ԫ
	inline CDateTime CurrentEpoch() const { return Epoch+ElapsedTime; }

	//! ���ǵ����ʱ�䣬����Epoch��ʼ���������
	inline double t() const { return ElapsedTime; }

	//! ���ǵ�λ��(��λ:km)
	inline vec3 Pos() const { return Position; }

	//! ���ǵ��ٶ�(��λ:km/s)
	inline vec3 Vel() const { return Velocity; }
	
	//! ��������λ��(��λ:km)���ٶ�(��λ:km/s)
	inline void SetPosVel(double t,const vec3& p,const vec3& v) 
	{
		ElapsedTime = t;
		Position = p;
		Velocity = v;
		RefreshStatus();
	}	

	//! ���ǵĹ������������һ��Kepler
	inline Kepler GetOrbitElements()const {	return Kepler(a,e,i,Omega,w,M);	}
	
	//! ���ǵĹ������������һ��Kepler
	inline void GetOrbitElements(double oe[6])const { memcpy(&oe,&a,sizeof(double)*6); }
	
	//! ��������
	inline double Mass(){ return Mass0 - BurnedFuel; }
	
	//! ���Ƿ���������
	inline double Force(){ return Engine->F; }

	CSatelliteBase();
	CSatelliteBase(std::string name);
	virtual ~CSatelliteBase();
	void Initialize();
	void Initialize(const CDateTime& t,const vec3& r,const vec3& v);
	void Initialize( const char* filename );
	void SetAutoSave(bool save = true);
	void SetForce(int l,unsigned int PTB);
	void SetEngine(double I,double f);

// ���ƺ���
	void Propagate(double Step,const double Duration);
	void Propagate2Perigee();
	void Propagate2Apogee();
	void Propagate2AscendingNode();
	void Propagate2DescendingNode();
	void Propagate2Epoch(const CDateTime epoch);
	// �������
	void PropagateBackward(double h,const double dt);

	//! ˲ʱ�������
	void ImpluseManeuver(vec3 Dv,Coordination d = VVLH );

	//! �ر��Զ������ļ�
	void CloseFile(){FileInstElem.close();FirstTimeOutput = true;}
private:
	//! CSatelliteBase�����ļ������������Զ������ļ�������
	static unsigned int Counter;

protected:
	//! ������
	mat33 PR;
	//! �¶�����
	mat33 NU;
	//! ���ƾ���
	mat33 PW;
	//! ����¶����ƾ����Ӧ��ʱ��
	double PNTime;
	//! ˢ������¶��Ⱦ���
	void RefreshMatrix(const CDateTime t); 
	
	
	//! ��������г�����г��Ľ���
	int LL;
	
	//! �㶯Դѡ���־
	unsigned int Perturbation;
	
	//! ������
	CEngine* Engine;

/////// �ļ�����  ///////////////

	//! ����ļ�
	fstream FileInstElem;
	
	//! �Ƿ��һ��д�ļ���־
	bool FirstTimeOutput;
	
	//! �Ƿ��Զ������־
	bool Save;
	
	virtual void SaveElem() = 0;

	virtual int OneStep(double StepSize,double MaxCutError = 1,CStopCondition* SC = NULL);
	virtual void RefreshStatus() = 0;
	virtual void DynFunction(const double t,const vec& x,vec& y) = 0;

	ORBITDYN_API friend double RKF78(CSatelliteBase& s,double hh,vec& x,vec& xh);
	ORBITDYN_API friend double Gill4(CSatelliteBase& s,double hh,vec& x,vec& xh);
};

ORBITDYN_API ostream & operator<<(ostream & os, const CSatelliteBase& sat);

/*!
\class CSatellite
����CSatelliteBase�Ļ���ʵ��
һ�������������ǵĹ�����㣬Ҳ���Լ������ת�ƹ���ͳӶ�����
\example examples/basic.cpp
*/
class ORBITDYN_API CSatellite:public CSatelliteBase
{
public:
	//! ���켰��������
	CSatellite(){}
	~CSatellite(){}

	//! ��ʼ�������ú���
	void Initialize(const CDateTime& t,const Kepler elem,const char flag = 'i');
	
	//! ������ǵĳ�ʼֵ���ļ���
	void ReportInitial();

	//! ����ع�ϵECFλ��
	const vec3 ECFPos();

	//! �����ڵع̵�λ���ٶ�
	void GetECF(vec3& ECFr,vec3& ECFv);
	
	//! �������ǵĵ���γ�Ⱥ����µ�ĵ���γ��
	CSpherical SubSatPoint(CSpherical & Geodetic,CSpherical & SSP);
	
	//! ����ƽ���������
	Kepler MedianElement() const;
	
	//! ��������
	CSpherical GetLLA();
	
	//double FiniteManeuver(Direction , StopCondition);

private:
	void DynFunction(const double t,const vec& x,vec& y);
	void RefreshStatus();

protected:
	void SaveElem();
};

/*!
�ӿ�����ٶȵĹ������
ʹ��Gill4�����㷨�����ֹ��̵Ķ���ѧ�����в���������¶�������λ�á��㶯���Ȳ�����
����ÿ��һ��ʱ�����

Ϊ�˱�֤��ʱ�������ʱ���ۼƵľ��ȣ�ʹ�������ĺ�����ElapsedTimems��Ϊ��ʱ������ԭ�е�ElapsedTime��Ϊ��������
*/
class ORBITDYN_API CRapidSatellite : public CSatellite
{
public:
	CRapidSatellite():ElapsedTimems(0)
		,m_lastpert(0)
		,PeriodCoord(1800000)
		,PeriodPert(50)
	{}
	~CRapidSatellite(){}

	//! ��ʼ�������ú���
	void Initialize(const CDateTime& t,const Kepler elem,const char flag);

	// h��λΪms
	void Propagate(int h);

private:
	// StepSize��λΪms
	int OneStep(int StepSize,double MaxCutError = 1,CStopCondition* StopCon = NULL );
	void DynFunction(const double t,const vec& x,vec& y);
	void RefreshStatus();
	void RefreshPert(const CDateTime t);// ˢ���㶯��
#if _MSC_VER < 1300
	unsigned long ElapsedTimems;
#else
	//! ��Գ�ʼ��Ԫ��ʱ�������
	unsigned long long ElapsedTimems;
#endif

public:

	// Ϊ�˼ӿ�����ٶȣ��������±���������RKF���ö���ѧ����ʱ�����±����ɲ����ظ�����
	// ���������������
	long PeriodCoord;   // �㶯���ĸ������ڣ�Ĭ��ֵ1,800,000ms

private:
	//! J2000-->ECIת������
	mat33 HG;
	//! ����λ��
	vec3 rm;
	//! ̫��λ��
	vec3 rs;

public:
	// ��¼��ǰ���㶯���ٶȣ���RKFÿ�ε��ö���ѧ����ʱ��������Ϊ�Ҷ��㶯���ٶȲ�
	long PeriodPert;   // �㶯���ĸ������ڣ�Ĭ��ֵ50ms

private:
#if _MSC_VER < 1300
	unsigned long m_lastpert;
#else
	unsigned long long  m_lastpert; // �ϴμ����㶯��ʱ�䣬�㶯ÿPeriodPert(ms)����һ��
#endif
	vec3 F_zonal;     // ��г��
	vec3 F_tesseral;  // ��г��
	vec3 F_moon;      // ��������
	vec3 F_sun;       // ̫������
	vec3 F_air;       // ��������
	vec3 F_light;     // ��ѹ
	vec3 F_ppn;       // ��ţ��
	vec3 F_Thrust;    // ����
};

/*!
\class CLEO
����͹������
ʹ��Сƫ���ʹ��������Ϊ״̬�������л���
һ�㲻ʹ�ã�����Ϊ�˱����������Ļ��֣�Ϊ�Ժ���о��ȱȽϱ������
*/
class ORBITDYN_API CLEO:public CSatellite
{
public:
	CLEO(){}
	~CLEO(){}
	void Propagate(double h,const double dt);
private:
	int OneStep(double StepSize,double MaxCutError = 1.0,CStopCondition* SC = NULL);
	void DynFunction(const double t,const vec& x,vec& y);
	void RefreshStatus();
};
/*!
\class CGEO
����ͬ���������
���ӵ���ͬ��������õĹ����������û�о���ʵ��
*/
class ORBITDYN_API CGEO:public CSatellite
{
public:
	double D;      //! ���Ư����
	double ex,ey;  //! ƫ����ʸ��
	double ix,iy;  //! ���ʸ��
	double L;      //! L=Omega+w+M
};
/*!
\class CMoonSat
�������ǵĹ������
�������·��е����ǵĹ��
����
*/
class ORBITDYN_API CMoonSat: public CSatelliteBase
{
private:
	// �Զ���ѧ���̺͹�������ļ������¶���
	void DynFunction(const double t,const vec& x,vec& y);
	void RefreshStatus();
	void SaveElem();
public:
	CMoonSat(){
		// �������ODP_LUNAR��ʾ�Ƿ��ǵ�������
		Perturbation = ODP_LUNAR_SAT;
		LL = 70;
		LightPressArea = 15.076;
		Mass0 = 1460.0;
		Cr = 1.44;
	}
	//! ʹ�����Ĺ���ϵ���������ʼ��
	void Initialize(const CDateTime& t,const Kepler elem);
	//! ʹ�����Ĺ���ϵλ���ٶȳ�ʼ��
	void Initialize(const CDateTime& t,const vec3 p,const vec3 v);
};

class ORBITDYN_API SatelliteException : public BaseException
{
public:
	SatelliteException(const std::string& details = "") 
		: BaseException("CSatellite exception:",details)
	{
	}
	virtual ~SatelliteException() _NOEXCEPT
	{
	}
	SatelliteException(const SatelliteException& cdte)
		:BaseException(cdte)
	{
	}
};

#endif  // __SATELLITE_H
