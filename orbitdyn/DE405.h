#pragma once
#include <stdio.h>
#include <BaseException.h>
#include <CDateTime.h>
#include <ublas.h>

enum PLANET { MERCURY=0, VENUS, EARTH, MARS, JUPITER, SATURN, URANUS, NEPTUNE, PLUTO,
	MOON, SUN, SOLAR_SYSTEM_BARYCENTER, EARTH_MOON_BARYCENTER, NUTATIONS, LIBRATIONS};

/*!
DE405星历中各大行星的位置是在太阳系质心ICRF参考坐标系中的位置矢量，单位为km
唯一的例外是：月球位置是相对地心的。
*/
class ORBITDYN_API DE405
{
	DE405(void);
	~DE405(void);

#if _MSC_VER < 1300
	#define ARRAY_SIZE 1018
	#define CONST_NUM 156
	#define CONST_NAME_LENGTH 7
	#define CONST_LABEL_LENGTH 60
#else
	const static unsigned int ARRAY_SIZE = 1018;
	const static unsigned int CONST_NUM = 156;
	const static unsigned int CONST_NAME_LENGTH = 7;
	const static unsigned int CONST_LABEL_LENGTH = 60;
#endif // _MSC_VER

	/*-------------------------------------------------------------------------*/
	/* Define ephemeris state type                                             */
	/*-------------------------------------------------------------------------*/
	struct stateData{
		double Position[3];
		double Velocity[3];
	};

	typedef struct stateData stateType;

	/*-------------------------------------------------------------------------*/
	/* Define the content of binary header records                             */
	/*-------------------------------------------------------------------------*/
	struct recOneData {
		char label[3][CONST_LABEL_LENGTH];
		char constName[CONST_NUM][CONST_NAME_LENGTH];
		double timeData[3];
		long int numConst;
		double AU;
		double EMRAT;
		long int coeffPtr[12][3];
		long int DENUM;
		long int libratPtr[3];
	};

	struct recTwoData {
		double constValue[CONST_NUM];
	}; 

	typedef struct recOneData recOneType;
	typedef struct recTwoData recTwoType;

	recOneType R1;
	recTwoType R2;
	FILE *Ephemeris_File;
	double Coeff_Array[ARRAY_SIZE] , T_beg , T_end , T_span;

	void Read_Coefficients( double Time );
	int Initialize_Ephemeris(const char *fileName );
	void Interpolate_Libration( double Time, int Target, double Libration[6] );
	void Interpolate_Nutation( double Time , int Target , double Nutation[4] );
	void Interpolate_Position( double Time , int Target , double Position[3] );
	void Interpolate_State( double Time , int Target , stateType *Planet );

	// 从枚举型变量PLANET转换成DE405中定义的整数编号
	int PLANET2int(PLANET p);

	// 月球在地月质心系中的位置速度
	void Moon_ESBARY(double JD,stateType* moon);

	// 地球在地月质心系中的位置速度
	void Earth_ESBARY(double JD,stateType* earth);

	// 月球在太阳系质心系中的位置速度
	void Moon_SSBARY(double JD,stateType* moon);

	// 地球在太阳系质心系中的位置速度
	void Earth_SSBARY(double JD,stateType* earth);

	static DE405* theInstance;
public:
	static DE405* Instance();

	Vector Moon(const CDateTime& t);
	Vector Sun(const CDateTime& t);
	void PlanetEphemeris(const CDateTime& t,PLANET cent,PLANET target,double cart[6]);
	void PlanetEphemeris(const CDateTime& t,PLANET cent,PLANET target,Vector& r,Vector& v);
	void Nutations(const CDateTime& t,double& dksi,double& deps); // 黄经章动与交角章动
	void Librations(const CDateTime& t,double& Omega,double& i,double& u); // 月固系相对月心惯性系的欧拉转角
};

class ORBITDYN_API DE405Exception : public BaseException
{
public:
	DE405Exception(const std::string& details = "") 
		: BaseException("DE405 exception:",details)
	{
	}
	virtual ~DE405Exception()
	{
	}
	DE405Exception(const DE405Exception& cdte)
		:BaseException(cdte)
	{
	}
};
