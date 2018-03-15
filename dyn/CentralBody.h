#ifndef __CENTRALBODY_H
#define __CENTRALBODY_H

#if defined(_MSC_VER) && _MSC_VER < 1300
	#include <iostream.h>
	#include <fstream.h>
	#include <math.h>
#else
	#include <fstream>
	#include <iostream>
	#include <cmath>
	using namespace std;
#endif

#include "Config.h"
#include "Msise00.h"

#pragma warning(disable:4251)

ORBITDYN_API double SA76(double h);

class ORBITDYN_API CentralBody
{
public:

	//! ������ϵ��
	double CLM[71][71],SLM[71][71];

	//! ������ģ��
	enum GRAVITY {WGS84, WGS84_EGM96, EGM96, JGM2, JGM3, GLGM2, LP165P} GrvModel;

	//! ����ģ��
	enum EPHEMICS {DE421EPH, DE405EPH , ANALYTICEPH } EphModel;

	CentralBody();
	virtual ~CentralBody();
	
	//! ����������
	virtual void SetGravityField(GRAVITY) = 0;

	//! ��õ���J2000����ϵ�µ�λ��
	virtual vec3 GetECI(const CDateTime&) = 0;

	//! ��õ���J2000����ϵ�µ�λ���ٶ�
	//virtual void GetECI(const CDateTime&,vec& r,vec& v) = 0;

	inline double GE()const{ return GrvParam; }
	inline double RE()const{ return Equator_Radius; }
protected:
	//! ��������
	double GrvParam;
	//! ƽ������뾶����
	double Equator_Radius;
	//! �����ļ�Ŀ¼
	std::string DataDir;
    //! ��ȡ�������ļ�
	void ReadGrvFile(const char* file);
};

//! ̫��
class ORBITDYN_API Sun : public CentralBody
{
public:
	//! ��ȡ����
	static Sun* Instance();

	void SetGravityField(GRAVITY);

	//! ��õ���J2000����ϵ�µ�λ��
	vec3 GetECI(const CDateTime&);
private:
	Sun();
	~Sun(){};
	static Sun* theInstance;
};


//! ����
class ORBITDYN_API Earth : public CentralBody
{
public:
	//! ���õ���������ϵ��
	double A2,A3,A4,J2,J3,J4;

	//! ����ģ��
	enum ATMOS { STANDARD1976, MSISE2000, MANUAL } AtmosModel;

	//! ��ȡ����
	static Earth* Instance();

	//! ����߶�h(km)���Ĵ����ܶ�
	double AtmosRuoH(const CDateTime& t,const vec3& ECFr);
	//! ����������
	void SetGravityField(GRAVITY);

	//! ��eop��ֵ���㼫�Ʋ���
	void GetPolarMotion(const CDateTime& t,double& Xp,double& Yp);

	//! ��eop��ֵ����dUT1 = UT1 - UTC
	double GetdUT1(const CDateTime& t);

	//! ��õ���J2000����ϵ�µ�λ��
	vec3 GetECI(const CDateTime&);
private:
	Earth();
	~Earth();
	
	//! ����
	static Earth* theInstance;
	//! ���ơ�dUT1���ݱ�
	double ** eop;
	//! ���ݱ�����
    int Num;
	//! ���ݱ��ʼMJD
	double mjd1;

	//! �����ܶ��ļ��ṹ
	struct FILE_ATMOS_DEN {
		int nrow; // ����
		double step; // ??
		double* t; // ʱ��(MJD��ʾ)
		double* rou; // �����ܶ�
	} file_atmos_den;

public:
	//! ���ô���ģ��
	void SetAtmosphereModel(ATMOS);
};

//! ����
class ORBITDYN_API Moon : public CentralBody
{
public:
	//! ��ȡ����
	static Moon* Instance();

	void SetGravityField(GRAVITY);

	//! ��õ���J2000����ϵ�µ�λ��
	vec3 GetECI(const CDateTime&);
private:
	Moon();
	~Moon(){};
	static Moon* theInstance;
};

class ORBITDYN_API CentralBodyException : public BaseException
{
public:
	CentralBodyException(const std::string& details = "") 
		: BaseException("CCentralBody exception:",details)
	{
	}
	virtual ~CentralBodyException() _NOEXCEPT
	{
	}
	CentralBodyException(const CentralBodyException& cdte)
		:BaseException(cdte)
	{
	}
};

#endif // __CENTRALBODY_H
