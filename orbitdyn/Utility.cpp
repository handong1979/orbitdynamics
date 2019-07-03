/*!
摄动加速度、数学函数
坐标转换函数(包括惯性系、地固系、轨道系)
\author  韩冬
*********************************************************************/

#include "Utility.h"
#include "Satellite.h"
#include "stdio.h"

/*!
读取环境变量，以便找到data目录所在的位置
*/
std::string GetOrbitDynDir()
{
    FILE* configfile;

#if __APPLE__
    fopen_s(&configfile,"~/.orbitdynamics","r");
    if(configfile!=NULL)
    {  // 检查配置文件内容
        //configfile >> Name;
        char Name[100];

        fscanf_s(configfile,"%s",Name);
        std::string DataDir(Name);
        std::string fileName = DataDir + "/data/planetEph.405";
        const char *fName = fileName.c_str();
        FILE *fp;
        fopen_s(&fp,fName,"r"); // 测试文件是否存在
        if(DataDir.empty() || fp == NULL)
        {
            if(configfile!=NULL) fclose(configfile);
            if(fp!=NULL) fclose(fp);
            return std::string("."); // 配置文件所指目录不存在，返回当前目录
        }
        else
        {
            if(configfile!=NULL) fclose(configfile);
            if(fp!=NULL) fclose(fp);
            return DataDir; // 配置文件中的目录
        }
    }
    
    return std::string("/Users/handong");
#else
	int env;
	// 读取环境变量OrbitDyn，即data目录所在位置
#if _MSC_VER < 1400
	char* Name = getenv("OrbitDyn");
	if(Name==NULL)
		env = 1;
	else
		env = 0;
#else
	char Name[100];
	size_t len;
	env = getenv_s(&len,Name,100,"OrbitDyn");
#endif // _MSC_VER < 1400
	if(!env) // 环境变量存在
	{
		std::string DataDir(Name);
		std::string fileName = DataDir + "\\data\\planetEph.405";
		const char *fName = fileName.c_str();
		FILE *fp;
		fopen_s(&fp,fName,"rb"); // 当有环境变量时测试文件是否存在
		if(DataDir.empty() || fp == NULL)
			env = 1; // 环境变量有错误，或405文件不存在
		else
		{
			return DataDir; // 环境变量中的目录
		}
	}
	if(env) // 环境变量不存在，或环境变量错误
	{
		fopen_s(&configfile,"C:\\OrbitDyn.config","r");
		if(false/*configfile!=NULL*/)
		{  // 检查配置文件内容
			//configfile >> Name;
			fscanf_s(configfile,"%s",Name);
			std::string DataDir(Name);
            std::string fileName = DataDir + "\\data\\planetEph.405";
			const char *fName = fileName.c_str();
			FILE *fp;
			fopen_s(&fp,fName,"rb"); // 测试文件是否存在
			if(DataDir.empty() || fp == NULL)
			{
				if(configfile!=NULL) fclose(configfile);
				if(fp!=NULL) fclose(fp);
				return std::string("."); // 配置文件所指目录不存在，返回当前目录
			}
			else
			{
				if(configfile!=NULL) fclose(configfile);
				if(fp!=NULL) fclose(fp);
				return DataDir; // 配置文件中的目录
			}
		}
		else
			return std::string("."); // 配置文件不存在，返回当前目录
	}
	return std::string("."); // 返回当前目录
#endif //__APPLE__
}

/*!产生白噪声
\param sigma3 噪声的3σ
\param mean 噪声均值
算法：\n
   mean + sigma3 / 3.0 * ( Σr - 6.0 )  \n
其中r伪0-1之间的伪随机数，Σr表示12个伪随机数的和
*/
double WhiteNoise(double sigma3,double mean)
{
	double var=0.;
	int n;
	for(n=0;n<12;n++)
		var+=(double)rand()/RAND_MAX;
	var=(var-6.)*sigma3/3.+mean;
	return var;
}

/*! 均匀分布的伪随机数
\param min 下限
\param max 上限
在min~max之间均匀分布的伪随机数，利用rand()产生的随机数，化到min~max之间。
*/
double EvenNoise(double min, double max)
{
	double val;
	val=2.0*rand()/32767.0-1.0;		// -1 到 1 之间均匀分布
	val=val/2.0*(max-min);
	val=val+(min+max)/2.0;				// min 到 max 之间均匀分布
	return(val);
}

/*! 格林尼治真恒星时
从瞬时真春分点到格林尼治经线的角度
\param t  CDatetime时间
\return  格林尼治真恒星时(0～2π)
*/
double GetSG(const CDateTime& t)
{
	double dpsi,deps;
	nutation_angle(t,dpsi,deps); // dpsi:黄经章动
	double eps = Ecliptic_Equator_Angle(t);
	double Omega = Moon_RAAN(t);
    return fmod(GetSGM(t) + dpsi*cos(eps) + (0.00264*sin(Omega) + 0.000063*sin(2*Omega))/3600*RAD, PI2);
}

/*! 格林尼治平恒星时
从瞬时平春分点到格林尼治经线的角度
\param t  CDatetime时间
\return  格林尼治平恒星时(0～2π)
*/
double GetSGM(const CDateTime& t)
{
	// JAT-EarthRef : IERS 1997   && Orbits-Xu p.14
	double ut1 = t.GetUT1();
	double ut10 = floor(ut1);
	double T0 = (ut10 - MJD2000)/36525.0;
	double UT1 = (ut1-ut10)*86400;
	double alpha = 1.002737909350795 + (5.9006e-11 - 5.9e-12*T0)*T0;
	double GMST = 6*3600 + 41*60 + 50.54841 + 
    			  (8640184.812866 + (0.093104 - 6.2e-6*T0)*T0)*T0 + 
				  alpha*UT1;
	GMST = fmod(GMST/3600.0*15.0,360);
	return GMST*RAD;

	// 下面是之前的版本,与上面的计算差别为4e-6度(2010-1-1)，对应GEO轨道位置～3.4m，LEO轨道～0.57m

	//double dt = t.GetUT1CentNum()*36525.0; // + Earth.GetdUT1(t)/86400.0; // 在 t.GetUT1CentNum()中考虑dUT1
	// 为了下面计算公式中减少数值截断误差，这里先把较大的转角归算到360度以内，再与常数项和二次方项相加
	//double rota = fmod(360.985647365*dt,360);
	//return fmod((280.4606184 + rota + 0.2908e-12*dt*dt)*RAD,PI2);
}

/*! J2000.0格林尼治恒星时
从J2000.0平春分点到格林尼治经线的角度
\param t  CDatetime时间
\return  J2000.0格林尼治恒星时(0～2π)
*/
double GetSGJ2000(const CDateTime& t)
{
	double zeta,theta,Z;
	precession_angle(t,zeta,theta,Z);
    return fmod(GetSGM(t) - zeta - Z,PI2);
}

/*! 太阳平轨道偏心率
\param t CDatetime时间
\return 太阳平轨道偏心率
*/
double Sun_Eccentricity(const CDateTime& t)
{
	double T = t.GetTDTCentNum();
	return 0.01670862 - 4.204e-5*T - 1.24e-6*T*T;
}

/*! 黄道与J2000.0平赤道的黄赤交角ε
\param t CDatetime时间
\return 黄赤交角(弧度)
*/
double Ecliptic_Equator_Angle(const CDateTime& t)
{
	double T = t.GetTDTCentNum();
	return (23.4392911 - 0.013004167*T - 1.64e-7*T*T ) * RAD;	
}

/*! J2000.0太阳平近地点幅角
\param t CDatetime时间
\return 太阳近地点平黄经(弧度)
*/
double Sun_Argument_Perigee(const CDateTime& t)
{
	double T = t.GetTDTCentNum();
	return fmod((282.9373472 + 0.32256206*T + 1.51e-4*T*T )*RAD,PI2);
	//(282.9373472 + 6190.32/3600.0*T + 1.655/3600.0*T*T)为相对当天平春分点
	// 的结果，减去黄经总岁差后得到上面使用的式子
}

/*! J2000.0太阳平近点角M
\param t CDatetime时间
\return 太阳平近点角(弧度)
*/
double Sun_Mean_Anomaly(const CDateTime& t)
{
	double T = t.GetTDTCentNum();
	//return fmod((357.5291 + 35997.65235681*T - 0.0001561*T*T)*RAD,PI2);
	return fmod((357.5291 + 129596581.04/3600.0*T - 0.562/3600.0*T*T)*RAD,PI2);
}

/*! 对于当天平春分点的太阳平黄经L
\param t CDatetime时间
\return 太阳平黄经(弧度)
*/
double Sun_Ecliptic_Longitude(const CDateTime& t)
{
	double T = t.GetTDTCentNum();
	return ( 280.46644722 + 36000.7698222222*T + 1.093/3600.0*T*T ) * RAD;
}

/*! 对于当天平春分点的白道近地点平黄经Γ
\param t CDatetime时间
\return 白道近地点平黄经(弧度)
*/
double Moon_Argument_Perigee(const CDateTime& t)
{
	double T = t.GetTDTCentNum();
	return fmod(( 83.3532417 + 4069.013711*T - 37.165/3600.0*T*T ) * RAD,PI2);
}

/*! 对于当天平春分点的白道升交点平黄经Ω
\param t CDatetime时间
\return 白道升交点平黄经(弧度)
*/
double Moon_RAAN(const CDateTime& t)
{
	double T = t.GetTDTCentNum();
	return ( 125.0445555 - 1934.136185*T + 7.476/3600.0*T*T ) * RAD;
}

/*! 对于当天平春分点的月球平黄经L
\param t CDatetime时间
\return 月球平黄经(弧度)
*/
double Moon_Ecliptic_Longitude(const CDateTime& t)
{
	double T = t.GetTDTCentNum();
	return ( 218.31665555 + 481267.8813425*T - 4.787/3600.0*T*T ) * RAD;
}

/*! 对于当天平春分点的月球平近点角M
\param t CDatetime时间
\return 月球平近点角(弧度)
*/
double Moon_Mean_Anomaly(const CDateTime& t)
{
	return fmod(Moon_Ecliptic_Longitude(t) - Moon_Argument_Perigee(t),PI2);
}

/*! 对于当天平春分点的月球与太阳的平距角D
\param t CDatetime时间
\return 月球与太阳的平距角(弧度)
*/
double Moon_Sun_Ecliptic_Angle(const CDateTime& t)
{
	return Moon_Mean_Anomaly(t) - Sun_Mean_Anomaly(t);
}

/*!RKF78积分器
\param s 卫星
\param h 步长
\return 截断误差
*/
double RKF78(CSatelliteBase& s,double h,vec& x,vec& xh)
{
	static const double ak[13] = {
		            0,		2.0 / 27.0,		1.0 / 9.0,		1.0 / 6.0,		5.0 / 12.0,		1.0 / 2.0,
			5.0 / 6.0,		1.0 / 6.0,		2.0 / 3.0,		1.0 / 3.0,		1.0,		0.0,		1.0  };

	static const double b1 = 2.0 / 27.0;
	static const double b2[2] = { 1.0/36.0, 1.0/12.0 };
	static const double b3[3] = { 1.0/24.0, 0.0, 0.125 };
	static const double b4[4] = { 5.0/12.0, 0.0, -1.5625, 1.5625 };
	static const double b5[5] = { 0.05,	0.0, 0.0, 0.25, 0.2 };
	static const double b6[6] = { -25.0/108.0, 0.0, 0.0, 125.0/108.0, -65.0/27.0, 125.0/54.0 };
	static const double b7[7] = {  31.0/300.0, 0.0, 0.0, 0.0, 61.0/225.0, -2.0/9.0, 13.0/900.0 };
	static const double b8[8] = { 2.0, 0.0, 0.0, -53.0/6.0, 704.0/45.0, -107.0/9.0, 67.0/90.0, 3.0 };
	static const double b9[9] = { -91.0/108.0, 0.0, 0.0, 23.0/108.0, -976.0/135.0, 311.0/54.0, -19.0/60.0, 17.0/6.0, -1.0/12.0 };
	static const double b10[10] = { 2383.0/4100.0, 0.0, 0.0, -341.0/164.0, 4496.0/1025.0, -301.0/82.0, 2133.0/4100.0, 45.0/82.0, 45.0/164.0, 18.0/41.0 };
	static const double b11[11] = { 3.0/205.0, 0.0, 0.0, 0.0, 0.0, -6.0/41.0, -3.0/205.0, -3.0/41.0, 3.0/41.0, 6.0/41.0, 0.0 };
	static const double b12[12] = { -1777.0/4100.0, 0.0, 0.0, -341.0/164.0, 4496.0/1025.0, -289.0/82.0, 2193.0/4100.0, 51.0/82.0, 33.0/164.0, 12.0/41.0, 0.0, 1.0 };
	static const double W[13] = { 0.0, 0.0, 0.0, 0.0, 0.0, 34.0/105.0, 9.0/35.0, 9.0/35.0, 9.0/280.0, 9.0/280.0, 0.0, 41.0/840.0, 41.0/840.0 };

	double t1;
	static vec x1;
	static vec y[13];
	for(int ii=0;ii<13;ii++) y[ii].copy_size(x);

	double t = s.t();
	x1 = x;
	t1 = t;
	s.DynFunction(t1,x1,y[0]);

	x1 = x + h*b1*y[0];
	t1 = t + h*ak[1];
	s.DynFunction(t1,x1,y[1]);

	x1 = x + h*(b2[0]*y[0] + b2[1]*y[1]);
	t1 = t + h*ak[2];
	s.DynFunction(t1,x1,y[2]);

	x1 = x + h*(b3[0]*y[0] + b3[2]*y[2]);
	t1 = t + h*ak[3];
	s.DynFunction(t1,x1,y[3]);

	x1 = x + h*(b4[0]*y[0] + b4[2]*y[2] + b4[3]*y[3]);
	t1 = t + h*ak[4];
	s.DynFunction(t1,x1,y[4]);

	x1 = x + h*(b5[0]*y[0] + b5[3]*y[3] + b5[4]*y[4]);
	t1 = t + h*ak[5];
	s.DynFunction(t1,x1,y[5]);

	x1 = x + h*(b6[0]*y[0] + b6[3]*y[3] + b6[4]*y[4] + b6[5]*y[5]);
	t1 = t + h*ak[6];
	s.DynFunction(t1,x1,y[6]);

	x1 = x + h*(b7[0]*y[0] + b7[4]*y[4] + b7[5]*y[5] + b7[6]*y[6]);
	t1 = t + h*ak[7];
	s.DynFunction(t1,x1,y[7]);

	x1 = x + h*(b8[0]*y[0] + b8[3]*y[3] + b8[4]*y[4] + b8[5]*y[5] + b8[6]*y[6] + b8[7]*y[7]);
	t1 = t + h*ak[8];
	s.DynFunction(t1,x1,y[8]);

	x1 = x + h*(b9[0]*y[0] + b9[3]*y[3] + b9[4]*y[4] + b9[5]*y[5] + b9[6]*y[6] + b9[7]*y[7] + b9[8]*y[8]);
	t1 = t + h*ak[9];
	s.DynFunction(t1,x1,y[9]);

	x1 = x + h*(b10[0]*y[0] + b10[3]*y[3] + b10[4]*y[4] + b10[5]*y[5] + b10[6]*y[6] + b10[7]*y[7] + b10[8]*y[8] + b10[9]*y[9]);
	t1 = t + h;  // ak[10] = 1.0
	s.DynFunction(t1,x1,y[10]);

	x1 = x + h*(b11[0]*y[0] + b11[5]*y[5] + b11[6]*y[6] + b11[7]*y[7] + b11[8]*y[8] + b11[9]*y[9]);
	t1 = t;     // ak[11] = 0.0
	s.DynFunction(t1,x1,y[11]);

	x1 = x + h*(b12[0]*y[0] + b12[3]*y[3] + b12[4]*y[4] + b12[5]*y[5] + b12[6]*y[6] + b12[7]*y[7] + b12[8]*y[8] + b12[9]*y[9] + b12[11]*y[11]);
	t1 = t + h;   // ak[12] = 1.0
	s.DynFunction(t1,x1,y[12]);

	xh = x + h*( W[5]*y[5] + W[6]*y[6] + W[7]*y[7] + W[8]*y[8] + W[9]*y[9] + W[11]*y[11] + W[12]*y[12] );
	double EE = sum(abs((y[0] + y[10] - y[11] - y[12])*h*41.0/840.0));
	
	return EE;
}

/*!Gill4积分器
\param s 卫星
\param h 步长
\return 截断误差
*/
double Gill4(CSatelliteBase& s,double h,vec& x,vec& xh)
{
	static const double s2 = sqrt(2.0);
	static vec x1;
	static vec y[4];
	for(int ii=0;ii<4;ii++) y[ii].copy_size(x);
	
	double t = s.t(); // 对于CRapidSatllite,动力学方程中不含t
	x1 = x;
	s.DynFunction(t,x1,y[0]);

	x1 = x + h/2*y[0];
	s.DynFunction(t,x1,y[1]);

	x1 = x + h*((s2-1)/2*y[0] + (1-s2/2)*y[1]);
	s.DynFunction(t,x1,y[2]);

	x1 = x + h*(-s2/2*y[1] + (1+s2/2)*y[2]);
	s.DynFunction(t,x1,y[3]);

	xh = x + h/6*(y[0] + (2-s2)*y[1] + (2+s2)*y[2] + y[3]);

	return 0;
}

/*!带谐项摄动Zonal Harmonic Terms
\param LL 带谐项阶数
\param SatPos 卫星在地固系中的坐标
\param cb 中心天体，默认Earth，即地球带谐项摄动，设为Moon时即月球带谐项摄动
\return 带谐项摄动加速度(ECF)(km/s^2)
带谐项势函数为：
\[
V_l  = \frac{\mu }{r}\sum\limits_{l = 2}^\infty  {\overline C _l } \left( {\frac{{{\mathop{\rm Re}\nolimits} }}{r}} \right)^l \overline P _l \left( {\sin \phi } \right)
\]
带谐项摄动加速度为：
\[
\frac{{\partial V_l }}{{\partial {\bf{r}}}} = \frac{{\partial V_l }}{{\partial r}} \cdot \frac{{\partial r}}{{\partial {\bf{r}}}} + \frac{{\partial V_l }}{{\partial \sin \phi }} \cdot \frac{{\partial \sin \phi }}{{\partial {\bf{r}}}} + \frac{{\partial V_l }}{{\partial \lambda }} \cdot \frac{{\partial \lambda }}{{\partial {\bf{r}}}}
\]
其中：
\[
\frac{{\partial V_l }}{{\partial \lambda }} =  - \frac{\mu }{{r^2 }}\sum\limits_{l = 2}^\infty  {\overline C _l \left( {l + 1} \right)} \left( {\frac{{{\mathop{\rm Re}\nolimits} }}{r}} \right)^l \overline P _l \left( {\sin \phi } \right)
\]
\[
\frac{{\partial V_l }}{{\partial \sin \phi }} = \frac{\mu }{r}\sum\limits_{l = 2}^\infty  {\overline C _l } \left( {\frac{{{\mathop{\rm Re}\nolimits} }}{r}} \right)^l \overline {P'} _l \left( {\sin \phi } \right)
\]
\[
\frac{{\partial V_l }}{{\partial \lambda }} = 0
\]
\[
\frac{{\partial r}}{{\partial {\bf{r}}}} = \frac{{\bf{r}}}{r} = \frac{1}{r}\left( \begin{array}{l}
 x \\ 
 y \\ 
 z \\ 
 \end{array} \right)
\]
\[
\frac{{\partial \sin \phi }}{{\partial {\bf{r}}}} =  - \frac{z}{{r^3 }}{\bf{r}} + \frac{1}{r}{\bf{k}} =  - \frac{z}{{r^3 }}\left( \begin{array}{l}
 x \\ 
 y \\ 
 z \\ 
 \end{array} \right) + \frac{1}{r}\left( \begin{array}{l}
 0 \\ 
 0 \\ 
 1 \\ 
 \end{array} \right)
\]
\[
\frac{{\partial \lambda }}{{\partial {\bf{r}}}} = \frac{1}{{x^2  + y^2 }}\left( \begin{array}{l}
  - y \\ 
 x \\ 
 0 \\ 
 \end{array} \right) = \frac{1}{{x^2  + y^2 }}\left( \begin{array}{l}
  - \sin \lambda  \\ 
 \cos \lambda  \\ 
 0 \\ 
 \end{array} \right)
\]
*/
const vec3 PZonalHarmonic(const int LL,const vec3& SatPos,const CentralBody* cb )/* cb = Earth */
{
	static vec3 k("0 0 1");
	static double PL[71];
	if(cb==NULL)
		cb = Earth::Instance();
	double r = norm(SatPos,2);
	double zr = SatPos(2)/r;
	if(LL<2) return vec3("0 0 0");//即不考虑地球非球形引力摄动
	Legendre_sphPl(LL,zr,PL);
	vec3 prpr = SatPos/r;
	vec3 psfpr = -zr/r/r*SatPos + 1.0/r*k;

	double WP,RL,dPL,pVpr=0,pVpsf=0;
	for(int L=2;L<=LL;L++)
	{
		WP = sqrt((2.0*L+1.0)/(2.0*L-1.0));
		if(zr*zr==1.0)
			dPL = 0;
		else
            dPL = L/(1.0-zr*zr)*(WP*PL[L-1]-zr*PL[L]);
		RL = pow(cb->RE()/r,L);
		pVpr += cb->CLM[L][0]*RL*(L+1.0)*PL[L];
		pVpsf += cb->CLM[L][0]*RL*dPL;
	}
	pVpr *= -cb->GE()/r/r;
	pVpsf *= cb->GE()/r;
	return pVpr*prpr + pVpsf*psfpr;
}

/*! Tesseral Harmonic Terms
\param LL 阶数
\param SatPos 地固系中的坐标
\param cb 中心天体，默认Earth，即地球田谐项摄动,设为Moon时即月球田谐项摄动
\return Tesseral Harmonic Acceleration(ECF)(km/s^2)
*/
const vec3 PTesseralHarmonic(const int LL,const vec3& SatPos,const CentralBody* cb )/* cb = Earth */
{
	static vec3 k("0 0 1");
	static double PLM[71][71]={0},SX[71]={0},CX[71]={0};
	if(cb==NULL)
		cb = Earth::Instance();
	double RL,dPlm,WCS,WSC,pVpr=0,pVpsf=0,pVpl=0;
	double r = norm(SatPos,2);
	double zr = SatPos(2)/r;
	Legendre_sphPlm(LL,zr,PLM);
	double gxy = sqrt(SatPos(0)*SatPos(0)+SatPos(1)*SatPos(1));
	if(fabs(gxy)<1.0e-6)
	{
		vec3 F;
		F.zeros();
		for(int L=2;L<=LL;L++)
		{
			RL = pow(cb->RE()/r,L);
			F(0) -= cb->CLM[L][1]*PLM[L][1]/RL;
			F(1) -= cb->SLM[L][1]*PLM[L][1]/RL;
		}
		return -cb->GE()/r/r*F;
	}
	vec3 prpr = SatPos/r;
	vec3 psfpr = -zr/r/r*SatPos + 1.0/r*k;
	vec3 plpr;
	plpr(0) = -SatPos(1);
	plpr(1) = SatPos(0);
	plpr(2) = 0.0;
	plpr = plpr/(gxy*gxy);
	SmxCmx(LL, SatPos(1)/gxy, SatPos(0)/gxy, SX, CX);
	for(int L=2;L<=LL;L++)
	{
		RL = pow(cb->RE()/r,L);
		for(int M=1;M<=L;M++)
		{
			dPlm = 1.0/(1.0-zr*zr)*(sqrt((2.0*L+1)/(2.0*L-1)*(L+M)*(L-M))*PLM[L-1][M]-L*zr*PLM[L][M]);
            WCS = cb->CLM[L][M]*CX[M] + cb->SLM[L][M]*SX[M];
			WSC = cb->SLM[L][M]*CX[M] - cb->CLM[L][M]*SX[M];
			pVpr -= RL*(L+1.0)*PLM[L][M]*WCS;
			pVpsf += RL*dPlm*WCS;
			pVpl += RL*M*PLM[L][M]*WSC;
		}
	}
	pVpr *= cb->GE()/r/r;
	pVpsf *= cb->GE()/r;
	pVpl *= cb->GE()/r;
	return pVpr*prpr + pVpsf*psfpr + pVpl*plpr;
}

/*! Pl(x):Normalized legendre Polynomial
\param LL 阶数
\param x 自变量
\param PL 归一化勒让德多项式的值
*/
void Legendre_sphPl(const int LL,const double x,double PL[])
{
	double W1,W2,W3,W4;
	PL[0] = 1;
	PL[1] = sqrt(3.0) * x;
	PL[2] = sqrt(5.0) * (1.5*x*x-0.5);
	if(LL<3) return;
	for(int L=3;L<=LL;L++)
	{
		W1 = 1.0/L;
		W2 = 2.0*L;
		W3 = sqrt((W2-1.0)/(W2-3.0)); // sqrt(2l-1)/(2L-3)
		W4 = sqrt((W2+1.0)/(W2-1.0)); // sqrt(2l+1)/(2L-1)
		PL[L] = W4*( (2.0-W1)*x*PL[L-1]-W3*(1.0-W1)*PL[L-2] );
	}
}
/*! Plm(x):Normalized legendre Polynomial
\param LL 阶数
\param x 自变量
\param PLM 归一化勒让德多项式的值
*/
void Legendre_sphPlm(const int LL,const double x,double PLM[][71])
{
	double G3 = sqrt(3.0);
	double GX2 = sqrt(1-x*x);
	double W1,W2,WL1,WL2,WL3,WL0,WM1,WM2,WM3,WM12,WLM1,WLM2;
	PLM[1][1] = G3*GX2;
	for(int L=2;L<=LL;L++)
	{
		W1 = 2.0*L;         // 2L
		W2 = sqrt(W1);      // sqrt(2L)
		WL1 = sqrt(W1+1.0); // sqrt(2L+1)
		WL2 = sqrt(W1-1.0); // sqrt(2L-1)
		WL3 = sqrt(W1-3.0); // sqrt(2L-3)
		WL0 = WL1/W2;       // sqrt(2L+1)/2L
		PLM[L][L] = WL0 * GX2 * PLM[L-1][L-1];
		PLM[L][L-1] = WL1 * x * PLM[L-1][L-1];
		for(int M=1;M<=L-2;M++)
		{
			WM1 = L + M;       // L+M
			WM2 = L - M;       // L-M
			WM3 = sqrt((WM1-1.0)*(WM2-1.0)); // sqrt(L+M-1)*(L-M-1)
			WM12 = sqrt(WM1*WM2);    // sqrt(L+M)*(L-M)
			WLM1 = WL1*WL2/WM12;     // sqrt(2L+1)*(2L-1)/(L+M)/(L-M)
			WLM2 = WM3*WL1/(WM12*WL3); // sqrt(2L+1)*(L+M-1)*(L-M-1)/(2L-3)/(L+M)/(L-M)
			PLM[L][M] = WLM1 * x * PLM[L-1][M] - WLM2 * PLM[L-2][M];
		}
	}
}
/*! 用递推方法计算sin(m*x)和cos(m*x)
\param LL 阶数
\param S1X  sin(x)
\param C1X  cos(x)
\param SX  返回sin(m*x)数组
\param CX  返回cos(m*x)数组
*/
void SmxCmx(const int LL,const double S1X,const double C1X,double* SX,double* CX)
{
	SX[0] = 0;    // sin(0)
	CX[0] = 1;    // cos(0)
	SX[1] = S1X;  // sin(x)
	CX[1] = C1X;  // cos(x)
	SX[2] = 2.0*SX[1]*CX[1]; // 2sin(x)cos(x)
	CX[2] = 2.0*CX[1]*CX[1] - 1.0; // 2cos(x)^2 -1
	if(LL<3) return;
	double CX2 = 2.0 * CX[1]; // 2cos(x)
	for(int MM=3;MM<=LL;MM++)
	{
		SX[MM] = CX2*SX[MM-1] - SX[MM-2]; // 2cos(x)sin[(m-1)x] - sin[(m-2)x]
		CX[MM] = CX2*CX[MM-1] - CX[MM-2]; // 2cos(x)cos[(m-1)x] - cos[(m-2)x]
	}
}
/*! third body gravitation
\note r,r3b,G的单位要统一，坐标系要统一
\param r satellite vector
\param r3b third body vector
\return third body Gravity Acceleration(ECI)(km/s^2)
*/
const vec3 PThirdBodyGrv(const vec3& r, const vec3& r3b,double G)
{
	vec3 dr = r - r3b;
	double r3b3 = pow(norm(r3b,2),3);
	double dr3 = pow(norm(dr,2),3);
	return - G * (dr/dr3 + r3b/r3b3);
}
/*!阴影计算--双圆锥地影模型dual cone
计算卫星是否在半影区(penumbra)或本影区(umbra)
函数返回可视因子(visible fraction),这里可视因子定义为：本影区为0,半影区在0~1之间,阳照区为1
调用函数时注意r和rs为中心体到卫星、太阳的矢量
\param r 卫星位置矢量
\param rs 太阳位置矢量
\param Rcb 中心天体半径(默认为地球半径)
\return 可视因子(visible fraction)
*/
double Shadow(const vec3& r,const vec3& rs,const double& Rcb )/* Rcb = Re */
{
	vec3 dr = r - rs;
	double rm = norm(r,2), drm = norm(dr,2);
	double thetaES = acos( dot(r,dr)/rm/drm );  // 天体-卫星-太阳夹角
	double as = asin(Rs/drm);  // 卫星上看太阳的视半径
	double ae = asin(Rcb/rm);  // 卫星上看天体的视半径
	if( thetaES <= ae-as)
	{
		return 0.0; // 本影区，日全食
	}
	else if( thetaES < ae+as )
	{
		if(as-thetaES>=ae)
			return 1.0-ae*ae/as/as;  // 天体比太阳视半径小，日环食
		else
		{
			double a = acos( (ae*ae+thetaES*thetaES-as*as)/(2.0*ae*thetaES) );
			double b = acos( (as*as+thetaES*thetaES-ae*ae)/(2.0*as*thetaES) );
			return 1.0 - ( as*as*(b-sin(b)*cos(b)) + ae*ae*(a-sin(a)*cos(a)) )/as/as/PI;  // 太阳可视面积与太阳圆面积的比值，日偏食
		}			
	}
	else
	{
		return 1.0; // 阳照区
	}
}

/*!阴影计算--圆柱地影模型
计算卫星是否在半影区(penumbra)或本影区(umbra)
函数返回可视因子(visible fraction),这里可视因子定义为：本影区为0,半影区在0~1之间,阳照区为1
调用函数时注意r和rs为中心体到卫星、太阳的矢量
\param r 卫星位置矢量
\param rs 太阳位置矢量
\param Radius 中心天体半径(默认为地球半径)
\return 可视因子(visible fraction)
*/
double Shadow_Cylindrical(const vec3& r,const vec3& rs,double Radius)
{
	double r1  = norm(r,2);
	double rs1 = norm(rs,2);
 	double CPS = dot(r,rs)/(r1*rs1);
	double SPS = sqrt(1-CPS*CPS);
	if(CPS<0 && SPS<Radius/r1) return 0.0;  //判断地影
	return 1.0;
}
/*! 光压摄动 Light Pressure
\param CrSM  Cr*Area/Mass  Cr:反射系数  Area:光压面积    Mass:卫星质量
\param r 卫星位置矢量
\param rs 太阳位置矢量
\return 光压摄动加速度(km/s^2)
*/
const vec3 PLightPressure(const double CrSM,const vec3& r,const vec3& rs,double Radius )/* Radius = Re */
{
	vec3 dr = r - rs;
	double dr1 = norm(dr,2);
	vec3 acc = (Shadow(r,rs,Radius) * 4.5604e-6 * CrSM * pow(AU/dr1,2)/dr1) * dr;  // 4.5604e-6是一个AU处的太阳光压常数(N/m^2)

	// STK HPOP使用的公式，与上面的公式等效，3.823e26为太阳辐射功率[W]，4.5604e-6 = 3.823e26/(4*PI*c*AU^2)
	//vec3 acc = CrSM * 3.823e26 / (4*PI*LightVel*pow(dr,3)); 

	return acc/1000.0; // 返回的光压摄动加速度为km/s^2
}
/*! 大气阻力摄动 Atmosphere Drag
\param CdSM Cd*Area/Mass阻力系数乘面质比(m^2/kg)
\param ruo 大气密度(kg/m^3)
\param r 卫星位置矢量
\param rd 卫星速度矢量
\return 大气阻力加速度(ECI)(km/s^2)
*/
const vec3 PAirDrag(const CDateTime& t,const double CdSM,const vec3& ECFr,const vec3& r,const vec3& rd)
{
	vec3 air;
	air(0) = -We*r(1);
	air(1) =  We*r(0);
	air(2) =  0; // 地球自转牵连速度 = (0,0,We)叉乘r
	vec3 RelVel = rd - air;
	double ruo = Earth::Instance()->AtmosRuoH(t,ECFr);
	return ( -0.5* CdSM*1.0e-6 * ruo*1.0e9 * norm(RelVel,2) )*RelVel;
}
/*! 后牛效应摄动 Post-Newtonian Effect
\param r 卫星位置矢量
\param rd 卫星速度矢量
\return 后牛顿效应摄动加速度(ECI)(km/s^2)
*/
const vec3 PPostNewton(const vec3& r,const vec3& rd)
{
	double c2 = LightVel*LightVel;
	double r1 = norm(r,2);
	double r3 = r1*r1*r1;
	double c2r3 = c2*r3;
	double v2 = norm(rd,2);
    double r4v2 = 4.0 * GE / r1 - v2 * v2;
	double rv4 = 4.0 * dot(r,rd);
	return (r*r4v2 + rd*rv4) * GE / c2r3;
}

/*! 由位置速度计算轨道根数
\param miu 中心天体引力常数
\param Position 位置
\param Velocity 速度
\return 轨道根数(km, ,rad,rad,rad,rad)
*/
Kepler Cartesian_Kepler(const vec3 & Position,const vec3 & Velocity, double miu )/* miu = GE */
{
	 double a,e,i,o,w,M,u,EE;
	 double r = norm(Position,2);
	 double v = norm(Velocity,2);
	 if(r==0 || v==0)
		 throw KeplerException("Cartesian_Kepler(): position or velocity is zero.");
	 double v2 = v*v;
	 //double rrd = Dot(Position,Velocity);
/*a*/a = 1.0/(2.0/r-v2/miu);
	 vec3 Uz("0.0 0.0 1.0"); // ECI-Z轴
	 vec3 h = cross(Position,Velocity);//角动量方向
	 vec3 N = cross(Uz,h);//节线方向
	 double p = dot(h,h)/miu;
/*e*/if( p/a >= 1.0 ) e = 0;
	 else e = sqrt(1.0-p/a);

	 vec3 E = cross(Velocity,h)/miu - Position/r; //((v2-1.0/r)*Position-rrd*Velocity);
	// vec3 Eh = (v2/miu-1/r)*Position - rrd/miu*Velocity; 与上面表达式相同
	 double Nnorm = norm(N,2);
/*w*/if(norm(E,2)==0 || Nnorm==0)
	 {
	 	w = 0.0;
	 }
	 else
	 {
		 double cosw = norm_dot(N,E);
		 if(cosw>=1.0) w = 0.0;
		 else if(cosw<=-1.0) w = PI;
		 else w = E(2)>0 ? acos(cosw) : PI2-acos(cosw) ;
	 }
	 
	 double cosi = norm_dot(h,Uz);
/*i*/if(cosi>=1.0) i = 0.0;
	 else if(cosi<=-1.0) i = PI;
	 else i = acos(cosi);

	 if(Nnorm==0){
		o = 0.0;  // 0倾角卫星没有轨道节线
		u = atan2(Position(1),Position(0));
	 }
	 else{
		double Nx = N(0)/norm(N,2);
/*o*/	if(Nx>=1.0) o = 0.0;
		else if(Nx<=-1.0) o = PI;
		else o = N(1)>0 ? acos(Nx) : PI2-acos(Nx) ;
		double cosu = norm_dot(Position,N);
		if(cosu>=1.0) u = 0.0;
		else if(cosu<=-1.0) u = PI;
		else u = Position(2)>0 ? acos(cosu) : PI2-acos(cosu);
	 }
	if(e<=1.0)
	{
		EE = fmod(atan2(r*sin(u-w)/a/sqrt(1-e*e),r/a*cos(u-w)+e)+PI2,PI2);
/*M*/    M = mod(EE - e*sin(EE),PI2);
	}
	else
	{
		double tmp = (r*sin(u-w)/a/sqrt(e*e-1)) / (r/a*cos(u-w)+e);
		EE = 0.5*log((1+tmp)/(1-tmp));
/*M*/    M = fmod(EE - e*sinh(EE),PI2);
		M = mod(M,PI2);
	}
	return Kepler(a,e,i,o,w,M);
}
/*! 由轨道根数计算位置速度
\param obl 轨道根数
\param Pos 位置
\param Vel 速度
\param miu 中心天体引力常数
*/
void Kepler_Cartesian(const Kepler& obl,vec3& Pos,vec3& Vel, double miu )/* miu = GE */
{
	double sino = sin(obl.o);
	double coso = cos(obl.o);
	double sinw = sin(obl.w);
	double cosw = cos(obl.w);
	double sini = sin(obl.i);
	double cosi = cos(obl.i);
	vec3 P;
	P(0) = coso*cosw-sino*sinw*cosi;
	P(1) = sino*cosw+coso*sinw*cosi;
	P(2) = sinw*sini;
	vec3 Q;
	Q(0) = -coso*sinw-sino*cosw*cosi;
	Q(1) = -sino*sinw+coso*cosw*cosi;
	Q(2) =  cosw*sini;
	double r = obl.r();
	double f = obl.f();
	Pos = r*cos(f)*P + r*sin(f)*Q;
	Vel = -sqrt(miu/obl.p())*( sin(f)*P - (cos(f)+obl.e)*Q );
}
/*! 计算瞬时根数
\param MeanElem 平均根数{a e i O w M}
\return 瞬时根数
*/
Kepler Instant(const Kepler & MeanElem)
{
	double ShortTerm[6],as2;//ShortTerm[6]:短周期项,as2:半长轴的二阶短周期项
	Short2(MeanElem,ShortTerm,as2);
	Kepler InstElem;
	InstElem.a = MeanElem.a + ShortTerm[0] + as2;
	InstElem.i = MeanElem.i + ShortTerm[1];
	InstElem.o = MeanElem.o + ShortTerm[2];
	double ksi = MeanElem.e*cos(MeanElem.w) + ShortTerm[3];
	double eta = MeanElem.e*sin(MeanElem.w) + ShortTerm[4];
	double lamda = MeanElem.w + MeanElem.M + ShortTerm[5];
	InstElem.e = sqrt(ksi*ksi + eta*eta);
	InstElem.w = fmod(atan2(eta,ksi) + PI2,PI2);
	InstElem.M = fmod(lamda - InstElem.w + 2*PI2,PI2);
	return InstElem;
}
/*! 计算短周期项
\param MElem 平均根数
\param ZS 短周期项(a_s,i_s,Omega_s,ksi_s,eta_s,lamda_s)
\param AS2 a的二阶长期项
*/
void Short2(const Kepler & MElem,double ZS[6],double & AS2)
{
	double A2 = Earth::Instance()->A2; // 与引力场计算相符，否则使用Constant::A2
	double M_a = MElem.a / Re;
	double M_i = MElem.i;
	//double M_omiga = MElem.o;
	double M_ksi = MElem.e*cos(MElem.w);
	double M_eta = MElem.e*sin(MElem.w);
	double M_lamda = fmod(MElem.w + MElem.M + PI2,PI2);

	double E2 = M_ksi*M_ksi + M_eta*M_eta;  // e^2
	double WE2 = 1.0 - E2;                  // 1 - e^2
	double GE2 = sqrt(WE2);                 // sqrt(1-e^2)
	double F1e = 1.0 / (1.0 + GE2);         // F1e = 1/( 1+sqrt(1-e^2) )
// 这部分计算u，见《航天器轨道理论》P174下半页
// 这里定义 ksi = e*cos(w) , eta = e*sin(w) 与刘林书上差个负号，相应的公式均加个负号
	double UE = KeplerFunc2(M_ksi,M_eta,M_lamda); // E + w
	double SU = sin(UE);                         // sin(E+w)
	double CU = cos(UE);                         // cos(E+w)
	double Ef = M_ksi*SU - M_eta*CU;             // UE-lamda = E - M
	double RA = 1.0/(1.0 - M_ksi*CU - M_eta*SU); // a/r
	double RA3 = RA * RA * RA;                   // (a/r)^3
	// 下面U的计算有两种，两种结果都正确,但二种方法在f=90和270时计算存在截断误差
	// 1.刘林《航天器轨道理论》书上的公式：
	double SfE = RA*Ef*( 1.0 + F1e*(-M_eta*SU - M_ksi*CU) );  // sin(u-UE)
	double W = SfE / sqrt( 1.0 - SfE*SfE );      // tan(u-UE)
	double fE = atan(W);                         // u-UE
	double UJMw = fE + Ef;                       // u-lmada = (u-UE) + (UE-lamda)
	double U = M_lamda + UJMw;                   // u
	double S1u = sin(U);           // sin(u)
	double C1u = cos(U);           // cos(u)
	// 2.《航天器轨道确定》书上的公式：
	//double S1u = RA*(M_ksi*(M_lamda-UE)*F1e + SU - M_eta);
	//double C1u = RA*(-M_eta*(M_lamda-UE)*F1e + CU - M_ksi);
	//double U = atan2(S1u,C1u);
	//double UJMw = U - M_lamda; // u-lamda
/////////////////////////////////////////////////////
	double S2u = 2.0 * S1u * C1u;  // sin(2u)
	double C2u = 2.0*C1u*C1u-1.0;  // cos(2u)
	double SI = sin(M_i);          // sin(i)
	double S2 = SI * SI;           // sin(i)^2
	ZS[0] = A2/M_a * ( (2.0/3.0-S2) * (RA3-pow(WE2,-1.5)) + S2*RA3*C2u );   // a的短周期项as
//--ZS[0] END----------------------------------------------------
	double P = M_a * WE2;   // p = a(1-e^2)
	double P2 = P * P;      // p^2
	double A2P2 = A2/P2;    // A2/p^2
	double F2e = (1.0+2.0*GE2)*F1e*F1e;     // F2(e)
	double F3e = 0.25*(1.0+3.0*GE2)*F1e*F1e*F1e; // F3(e)
	double S3u = S1u*C2u + C1u*S2u;  // sin(3u)
	double C3u = C1u*C2u - S1u*S2u;  // cos(3u)
	double S4u = S1u*C3u + C1u*S3u;  // sin(4u)
	double C4u = C1u*C3u - S1u*S3u;  // cos(4u)
	double S5u = S1u*C4u + C1u*S4u;  // sin(5u)
	double C5u = C1u*C4u - S1u*S4u;  // cos(5u)
	double CI = cos(M_i);           // cos(i)
	double W23i = 1.0 - 1.5*S2;     // 1-3/2*sin(i)^2
	double W25i = 2.0 - 2.5*S2;     // 2-5/2*sin(i)^2
	double Wkc1 = M_ksi * M_ksi;    // ksi^2
	double Wkc2 = M_eta * M_eta;    // eta^2
	double Wkc3 = Wkc1 - Wkc2;      // ksi^2 - eta^2
	double Wkc4 = M_ksi * M_eta;    // ksi*eta

	ZS[1] = 0.5*A2P2*SI*CI*(M_ksi*C1u-M_eta*S1u+C2u+(M_ksi*C3u+M_eta*S3u)/3.0+Wkc3*F2e/3.0);

	ZS[2] = -A2/P2*CI*(UJMw+0.5*(M_ksi*S1u-3.0*M_eta*C1u)-0.5*S2u-(M_ksi*S3u-M_eta*C3u)/6.0-Wkc4*F2e/3.0);

	ZS[3] = A2P2*(-W25i*UJMw*M_eta+((1.0+0.25*Wkc1+2.25*Wkc2)*C1u-Wkc4*S1u
			+0.5*M_ksi*C2u+M_eta*S2u+(Wkc1-3.0*Wkc2)*C3u/12.0+Wkc4*S3u/3.0+(1.0-E2*F2e/12.0)*M_ksi
			-(Wkc1-3.0*Wkc2)*M_ksi*F2e/12.0)+S2*((-1.25+0.375*Wkc1-3.125*Wkc2)*C1u+0.25*Wkc4*S1u
	        +0.5*M_ksi*C2u-2.0*M_eta*S2u+(7.0/12.0+11.0/48.0*Wkc1+25.0/48.0*Wkc2)*C3u
	        -7.0/24.0*Wkc4*S3u+0.375*(M_ksi*C4u+M_eta*S4u)+Wkc3*C5u/16.0+Wkc4*S5u/8.0-(1.25-E2*F2e/6.0)*M_ksi
			+(0.25*F2e-F3e/6.0)*(Wkc1-3.0*Wkc2)*M_ksi));

	ZS[4] = A2P2*(W25i*UJMw*M_ksi+((1.0+1.25*Wkc1+0.25*Wkc2)*S1u-2.0*Wkc4*C1u
			-0.5*M_eta*C2u-E2*S3u/12.0+(1.0-E2*F2e/12.0)*M_eta
			+(Wkc2-3.0*Wkc1)*M_eta*F2e/12.0)+S2*((-1.75-1.125*E2)*S1u+3.25*Wkc4*C1u
			+0.5*M_ksi*S2u+2.0*M_eta*C2u+(7.0/12.0+13.0/48.0*Wkc1+23.0/48.0*Wkc2)*S3u
			+5.0/24.0*Wkc4*C3u+0.375*(M_ksi*S4u-M_eta*C4u)
			+Wkc3*S5u/16.0-Wkc4*C5u/8.0-(1.25-E2*F2e/6.0)*M_eta+(0.25*F2e-F3e/6.0)*(3.0*Wkc1-Wkc2)*M_eta));

	ZS[5] = -CI*ZS[2]+A2P2*(W23i*UJMw+(M_ksi*S1u-M_eta*C1u)+F1e*((1.0-0.25*E2)*(M_ksi*S1u-M_eta*C1u)+0.5*Wkc3*S2u
			-Wkc4*C2u+(Wkc1-3.0*Wkc2)*M_ksi*S3u/12.0+(Wkc2-3.0*Wkc1)*M_eta*C3u/12.0))
			+A2P2*S2*(-(2.0*M_ksi*S1u-M_eta*C1u)+0.75*S2u-(M_ksi*S3u-M_eta*C3u)/6.0+0.5*Wkc4*F2e
			+F1e*((-0.5+1.25*GE2+0.125*Wkc3)*M_ksi*S1u+(2.5+1.25*GE2-0.125*(7.0*Wkc1+5.0*Wkc2))*M_ksi*C1u
			-0.75*Wkc3*S2u+1.5*Wkc4*C2u+(1.0+5.0*GE2/12.0-7.0*Wkc1/48.0+17.0*Wkc2/48.0)*M_ksi*S3u
			-(1.0+5.0*GE2/12.0-19.0*Wkc1/48.0+5.0*Wkc2/48.0)*M_eta*C3u
			+0.375*Wkc3*S4u-0.75*Wkc4*C4u+(Wkc1-3.0*Wkc2)*M_ksi*S5u/16.0
			-(3.0*Wkc1-Wkc2)*M_eta*C5u/16.0-(0.25+(1.0+0.5*E2)*F2e/3.0)*Wkc4));
//--ZS[1]--ZS[5] END----------------------------------------------------
	double A2a = 2.0*A2/M_a;   // 2*A2/a
	double RA4 = RA * RA3;     // (a/r)^4
	double RA5 = RA * RA4;     // (a/r)^5
	double E4 = E2 * E2;       // (1-e^2)^2
	double S4 = S2 * S2;       // sin(i)^4
	double F4e = F1e/GE2;      // F4(e)
	double F5e = 0.5*(5.0+3.0*GE2-2.0*E2)*F1e; // F5(e)
	double Wkc5 = Wkc1 * Wkc1;  // ksi^4
	double Wkc6 = Wkc2 * Wkc2;  // eta^4
	double Wkc7 = Wkc1 * Wkc2;  // ksi^2 * eta^2
	double AS21 = -2.0*(ZS[0]/M_a+A2P2*GE2*W23i)*ZS[0]-A2a*(SI*CI*RA3*(1.0-C2u))*ZS[1]
				-A2a*(RA4/GE2*(M_ksi*S1u-M_eta*C1u)*(1.0-1.5*S2*(1.0-C2u))+GE2*RA5*S2*S2u)*ZS[5]
				+A2a*(RA4*(1.0-1.5*S2*(1.0-C2u))*(C1u+F4e*(Wkc2*C1u-Wkc4*S1u))
				+RA3*S2*S2u*pow(WE2,-1.5)*(-F5e*M_eta-2.0*S1u-0.5*(M_ksi*S2u-M_eta*C2u)+0.5*F1e*M_ksi
				*(4.0*(M_ksi*S1u-M_eta*C1u)+Wkc3*S2u-2.0*Wkc4*C2u)))*ZS[3]
				+A2a*(RA4*(1.0-1.5*S2*(1.0-C2u))*(S1u+F4e*(Wkc1*S1u-Wkc4*C1u))
				+RA3*S2*S2u*pow(WE2,-1.5)*(F5e*M_ksi+2.0*C1u+0.5*(M_ksi*C2u+M_eta*S2u)-0.5*F1e*M_eta
				*(4.0*(M_ksi*S1u-M_eta*C1u)+Wkc3*S2u-2.0*Wkc4*C2u)))*ZS[4];
	double AS22 = A2P2*A2P2*M_a*GE2*(W23i*W23i*((16.0+19.0*E2)/9.0+35.0*E4/(18.0*WE2)+2.0*GE2/9.0)
				+S2*(1.0+2.0*E2/3.0)+S4*(25.0*E2/24.0-5.0/6.0+35.0*E4/(16.0*WE2)
				+(Wkc5-6.0*Wkc7+Wkc6)/(32.0*WE2))+S2*(5.0/6.0-1.75*S2-2.0*W25i*F2e/3.0
				+E2*(7.0/3.0-3.5*S2)/WE2)*Wkc3);
	AS2 = AS21 - AS22;
//--AS2-END-------------------------------------------------------------
	ZS[0] *= Re;
	AS2 *= Re;
}

/*! 求平均根数
\param inst 瞬时根数
\return 平均根数
*/
Kepler Mean(const Kepler & inst)
{
	double ST[6],AS2;
	Kepler ME = inst;
	int count=0;
	double aeps,ksieps,etaeps,ieps,oeps,lmdeps;
	double a2,ksi2,eta2,i2,o2,lamda2;
	double ksi = inst.e*cos(inst.w);
	double eta = inst.e*sin(inst.w);
	double lamda = fmod(inst.w + inst.M,PI2);
	double mksi,meta,mlamda;
	while(true)
	{
		Short2(ME,ST,AS2);
		a2 = ME.a + ST[0] + AS2;
		i2 = ME.i + ST[1];
		o2 = ME.o + ST[2];
		ksi2 = ME.e*cos(ME.w) + ST[3];
		eta2 = ME.e*sin(ME.w) + ST[4];
		lamda2 = ME.w + ME.M + ST[5];		
		aeps = a2 - inst.a;
		ieps = i2 - inst.i;
		oeps = fmod(o2 - inst.o,PI2);
		ksieps = ksi2 - ksi;
		etaeps = eta2 - eta;
		lmdeps = fmod(lamda2 - lamda,PI2);
		if(lmdeps>3.15) lmdeps -= PI2;
		if( fabs(aeps)<1e-6 && fabs(ksieps)<1e-10 && fabs(etaeps)<1e-10
		    && fabs(ieps)<1e-8 && fabs(oeps)<1e-8 && fabs(lmdeps)<1e-8 )
			break;
		if( count++>50 )
		{
			throw KeplerException("Mean(): iteration too many times!");
			break;
		}
		ME.a = inst.a - ST[0] - AS2;
		ME.i = inst.i - ST[1];
		ME.o = inst.o - ST[2];
		mksi = inst.e*cos(inst.w) - ST[3];
		meta = inst.e*sin(inst.w) - ST[4];
		mlamda = inst.w + inst.M - ST[5];
		ME.e = GG345(mksi,meta);
		ME.w = fmod(atan2(meta,mksi)+PI2,PI2);
		ME.M = fmod(mlamda-ME.w+2*PI2,PI2);
	}
	return ME;
}
/*! 初始化平面控制喷气
\param init 初始轨道根数
\param destin 目标轨道根数
\param m1 第一次理想瞬时脉冲时间和方向
\param m2 第二次理想瞬时脉冲时间和方向
\param distri 分布系数，例如为0.8则这次双脉冲变轨改变的轨道半长轴为总半长轴差的80%
*/
void InitPlaneJet(const Kepler & init,const Kepler & destin,Maneuver& m1,Maneuver& m2,double distri)
{
	double daa,e,w,dex,dey,v,daa2,de2;
	double dv1,dv2,u1,u2;
	daa = ( destin.a - init.a ) / init.a * distri; // DeltaA/A
	e = init.e;
	w = init.w;
	dex = destin.e*cos(destin.w) - e*cos(w);
	dey = destin.e*sin(destin.w) - e*sin(w);
	v = sqrt(GE/destin.a)*1000;
	daa2 = daa*daa;
	de2 = dex*dex + dey*dey;

	if( daa2 > de2 )
	{
		u1 = 60.0 * RAD;
		dv1 = v/4.0*(daa2-de2)/(daa-dex*cos(u1)-dey*sin(u1));
		dv2 = v/2.0*daa - dv1;
		double cosu2 = v/2.0/dv2*(dex-2.0*dv1/v*cos(u1));
		double sinu2 = v/2.0/dv2*(dey-2.0*dv1/v*sin(u1));
		u2 = fmod(atan2(sinu2,cosu2)+PI2,PI2);
	}
	else if( daa2 < de2 )
	{
		dv1 = v/4.0*(daa+sqrt(de2));
		double cosu1 = dex/sqrt(de2);
		double sinu1 = dey/sqrt(de2);
		u1 = fmod(atan2(sinu1,cosu1)+PI2,PI2);

		dv2 = v/4.0*(daa-sqrt(de2));
		double cosu2 = -dex/sqrt(de2);
		double sinu2 = -dey/sqrt(de2);
		u2 = fmod(atan2(sinu2,cosu2)+PI2,PI2);
	}
	else
	{
		dv1 = v/2.0*daa;
		dv2 = 0;
		double cosu1 = dex/sqrt(de2)*daa/fabs(daa);
		double sinu1 = dey/sqrt(de2)*daa/fabs(daa);
		u1 = fmod(atan2(sinu1,cosu1)+PI2,PI2);
		u2 = 0;
	}

	double f,ra,ce,se,e1,dm1,dm2;
	f = u1 - w;
	ra = (1.0-e*e)/(1.0+e*cos(f));
	ce = ra*cos(f) + e;
	se = ra*sin(f)/sqrt(1-e*e);
	e1 = atan2(se,ce);
	dm1 = fmod(e1-e*sin(e1)+PI2,PI2);

	f = u2-w;
	ra = (1.0-e*e)/(1.0+e*cos(f));
	ce = ra*cos(f)+e;
	se = ra*sin(f)/sqrt(1-e*e);
	e1 = atan2(se,ce);
	dm2 = fmod(e1-e*sin(e1)+PI2,PI2);

	double tmp;
	if( dm2 < dm1 )
	{
		tmp = dv1;
		dv1 = dv2;
		dv2 = tmp;
		tmp = dm1;
		dm1 = dm2;
		dm2 = tmp;
	}
	//The time spot at which the first pulse occurs
	m1.DV(0) = dv1;
	m1.DV(1) = 0;
	m1.DV(2) = 0;
	m1.M = dm1;
	////The time spot at which the second pulse occurs
	if(dv2==0)
	{
		return;
	}
	else
	{
		m2.DV(0) = dv2;
		m2.DV(1) = 0;
		m2.DV(2) = 0;
		m2.M = dm2;
	}
}  //end of function InitPlaneJet()

/*! 轨道平面控制
\param init 初始轨道根数
\param destin 目标轨道根数
\param m1 第一次轨道倾角机动瞬时脉冲时间和方向
\param m2 第二次轨道RAAN机动瞬时脉冲时间和方向
\param distri 分布系数，例如为0.8则所有计算的ΔV都乘上80%
*/
void InitNormalJet(const Kepler & init,const Kepler & destin,Maneuver& m1,Maneuver& m2,double distri)
{
	double wn = sqrt(GE/init.a/init.a/init.a);
	double E1 = KeplerFunc(init.e , init.M);

	double idv = -(destin.i - init.i) * (wn*init.a*sqrt(1-init.e*init.e)) / (1-init.e*cos(E1)) * 1000;
	m1.DV(0) = 0;
	m1.DV(1) = idv*distri;//Flag=3*int(idv/fabs(idv));
	m1.DV(2) = 0;
	m1.M = fmod( -init.w + PI2, PI2 );

	idv = -(destin.o - init.o) * (wn*init.a*sqrt(1-init.e*init.e))	* sin(init.i) / (1-init.e*cos(E1)) * 1000;
	m2.DV(0) = 0;
	m2.DV(1) = idv*distri;//Flag=3*int(idv/fabs(idv));
	m2.DV(2) = 0;
	m2.M = fmod( PI/2 - init.w + PI2, PI2 );//f=PI/2-w ==> M
}

/*! relative position and velocity in satellite RefSat orbit coordinate
\param RefSat 目标轨道根数
\param chaser 追踪轨道根数
\param RelPos chaser's relative position in RefSat's orbit coordination
\param RelVel chaser's relative velocity to satellite in RefSat's orbit coordination
*/
void RIC(const Kepler& chaser,const Kepler& RefSat,vec3& RelPos,vec3& RelVel)
{
	vec3 rc,vc,rt,vt;
	Kepler_Cartesian(chaser,rc,vc);
	Kepler_Cartesian(RefSat,rt,vt);
	mat33 Coti = GetCoi(RefSat);
	RelPos = Coti*(rc - rt);
	//vec3 w(0,-RefSat.n(),0);
	// 这里的轨道角速度应该用位置速度来计算，与使用平均轨道
	// 相比，相对速度有差别，差别大小与目标轨道偏心率有关
	vec3 w = cross(rt,vt)/norm(rt,2)/norm(rt,2);
	w = Coti*w;
	RelVel = Coti*(vc - vt) - cross(w,RelPos);
}

/*! orbit elements of a satellite which has relative position and relative velocity in satellite RefSat orbit coordinate
\param RefSat reference satellite
\param relpos relative position
\param relvel relative velocity
\return elem chaser orbit elements
*/
Kepler RIC(const Kepler & RefSat,const vec3 & relpos,const vec3 & relvel)
{
	mat33 Coti = GetCoi(RefSat);
	vec3 rt,vt;
	Kepler_Cartesian(RefSat,rt,vt);
	//vec3 w(0,-RefSat.n(),0);
	// 这里的轨道角速度应该用位置速度来计算，与使用平均轨道
	// 相比，相对速度有差别，差别大小与目标轨道偏心率有关
	vec3 w = cross(rt,vt)/norm(rt,2)/norm(rt,2);
	w = Coti*w;
	vec3 cPos = rt + trans(Coti)*relpos; //目标在惯性坐标系下的位置矢量
	vec3 cVel = vt + trans(Coti)*(relvel + cross(w,relpos)); //目标在惯性坐标系下的速度矢量
	return Cartesian_Kepler(cPos,cVel); //由目标惯性系位置速度计算其轨道根数
}

/*! 由地固坐标系位置计算地理经纬度和高度
\param ECF 地固坐标系位置,单位km
\return 地理经纬度和高度(deg,km)
*/
CSpherical ECF_LLA(const vec3 & ECF)
{
	CSpherical lla; // 经纬度
	lla.Longitude = fmod(atan2(ECF(1),ECF(0)) + 4*PI, PI2) * DEG;
	//if( lla.Longitude > 180 ) lla.Longitude -= 360.0;

	double r = norm(ECF,2);
	double ee = 1.0 - (1.0 - Oblate)*(1.0 - Oblate);
	double b = Re*sqrt(1.0 - ee);
	double x2y2 = sqrt(ECF(0)*ECF(0) + ECF(1)*ECF(1));

	// 使用迭代法计算地理纬度和高度
	double N = Re;
	double h = r - sqrt(Re*b);
	double B = atan( ECF(2) / x2y2 / (1.0 - ee*N/(N+h) ) );
	double N1,h1,B1;
	for(int I=0;I<6;I++)
	{
		N1 = Re/sqrt( 1.0 - ee*sin(B)*sin(B) );
		h1 = x2y2 / cos(B) - N1;
		B1 = atan( ECF(2) / x2y2 / (1.0 - ee*N1/(N1+h1)) );
		if( fabs(B1-B)<1e-6 && fabs(h1-h)<1e5 ) break;
		N = N1;
		h = h1;
		B = B1;
	}
	lla.Latitude = B1 * DEG;
	lla.Altitude = h1;

	// 使用Ozone算法计算地理纬度和高度(结果不对)
	//double c = Re * sqrt(ee);
	//double n = (Re*x2y2 - c*c) / (2.0*b*ECF.z);
	//double s = (Re*x2y2 + c*c) / (2.0*b*ECF.z);
	//double V = (4.0*n*s + 1.0) / 3.0;
	//double W = (s + n) / (s - n);
	//double D = sqrt(W*W + V*V);
	//double I = pow(D+W,1.0/3.0) - pow(D-W,1.0/3.0);
	//double J = sqrt(4.0*n*n + 2.0*I);
	//double K = sqrt(I*I + 1.0);
	//double T = 2.0*n + J;
	//double G = T*T - 4.0*(I-K);
	//double U = (sqrt(G) + T) / 2.0;
	//double phi = atan( 2.0 * Re * U / b / (U*U-1.0) );
	//lla.Latitude = sign(ECF.z)*phi;
	//lla.Altitude = x2y2/cos(lla.Latitude) - Re/sqrt(1.0 - ee*sin(lla.Latitude)*sin(lla.Latitude));
	//lla.Latitude *= DEG;

	return lla;
}

/*! 太阳的惯性系轨道平根数
\param t CDateTime时间
\return 太阳在赤道惯性系的轨道平根数(km,RAD)
*/
const Kepler SunOrbit(const CDateTime& t)
{
	double T = t.GetTDTCentNum();     //t到J2000.0的儒略世纪数
	double T2 = T * T;
	return Kepler(
		1.00000102*AU,								             		// a
		0.01670862 -            0.00004204*T -     0.00000124*T2,		// e
		Ecliptic_Equator_Angle(t),	// i(epsilon)
		0,
		Sun_Argument_Perigee(t),	
		Sun_Mean_Anomaly(t));
}

/*! 太阳的赤道惯性系位置矢量
使用解析星历
\param t CDateime时间
\return 太阳在道惯性系的位置矢量(km)
*/
const vec3 SunAnalytic(const CDateTime& t)//太阳解析星历
{
	Kepler k = SunOrbit(t);
	vec3 r,v;
	Kepler_Cartesian(k,r,v);
	return r;
}

/*! 月球的地心惯性系位置矢量
使用解析星历
\param t  CDatetime时间
\return 月球在道惯性系的位置矢量
*/
const vec3 MoonAnalytic(const CDateTime& t)
{
	double T = t.GetTDTCentNum();
	double T2 = T*T;
	double T3 = T2*T;
	double T4 = T3*T;
	double l = 134.96340251 + (1717915923.2178*T + 31.8792*T2 + 0.051635*T3 - 0.00024470*T4)/3600;
	//double l = 134.9634 + (1717915923.2178*T)/3600;
	l = l*RAD;

	double lp = 357.52910918 + (129596581.0481*T - 0.5532*T2 - 0.000136*T3 - 0.00001149*T4)/3600;
	//double lp = 357.5291 + (129596581.0481*T)/3600;
	lp = lp*RAD;

	double F = 93.27209062 + (1739527262.8478*T - 12.7512*T2 - 0.00103*T3 + 0.00000417*T4)/3600;
	//double F = 93.272 + (1739527262.8478*T)/3600;
	F = F*RAD;
	
	double D = 297.85019547 + (1602961601.2090*T - 6.3706*T2 + 0.006593*T3 - 0.00003169*T4)/3600;
	//double D = 297.85 + (1602961601.2090*T)/3600;
	D = D*RAD;
	
	double Omega = 125.04455501 + (-6962890.2665*T + 7.47222*T2 + 0.007702*T3 - 0.00005939*T4)/3600;
	//double Omega = 125.044555 + (-6962890.2665*T)/3600;
	Omega = Omega*RAD;
	
	double dF = 22640*sin(l) + 769*sin(2*l) + 36*sin(3*l) - 125*sin(D) + 2370*sin(2*D) 
	    -668*sin(lp) - 412*sin(2*F) + 212*sin(2*D-2*l) + 4586*sin(2*D-l) + 192*sin(2*D+l) 
	    +165*sin(2*D-lp) + 206*sin(2*D-l-lp) - 110*sin(l+lp) + 148*sin(l-lp);
	//double dF = 22640*sin(l) + 2370*sin(2*D) + 4586*sin(2*D-l);
	dF = dF/3600*RAD;
	
	F = F + dF;
	F = fmod(F,PI*2);
	if (F>PI)
	    F = F-PI2;
	
	double em = 0.05490;
	double am = 384401;
	double im = 5.145396*RAD;
	
	double E = KeplerFunc(em,l);
	//double E = l + em*sin(l);// + 1/2*em^2*sin(2*l);
	double u;
	if (F>=-PI/2 && F<=PI/2)
	    u = atan(tan(F)/cos(im));
	else if (F<-PI/2)
	    u = atan(tan(F)/cos(im)) - PI;
	else // F>PI/2
	    u = atan(tan(F)/cos(im)) + PI;
	    
	double f = atan2(am*sqrt(1-em*em)*sin(E),am*cos(E)-am*em);
	//double w = u - f;
	double r = am*(1-em*em)/(1+em*cos(f));
	//double epson = (84381.448 - 46.815*T - 0.00059*T^2 + 0.001813*T^3)/3600*rad;
	double epson = 0.409105;
	vec3 p;
	p(0) = cos(u);
	p(1) = sin(u);
	p(2) = 0;
	return r*RotationX(-epson)*RotationZ(-Omega)*RotationX(-im)*p;
}

/*! 地心惯性系(J2000.0惯性系)到月心惯性系的转换
\param r 地心惯性系矢量
\param TDT 简约儒略日表示的地球动力学时(Unit:day)
\return 月心惯性系矢量
*/
const vec3 ECI_MCI(const CDateTime& t,vec3 & r)
{
	return r - DE405::Instance()->Moon(t);
}

/*! 由月心惯性系位置速度计算月心惯性系2中的轨道根数\n
月心惯性系指地心惯性系(J2000.0)平移到月心的坐标系\n
月心惯性系2指以月球赤道面为XY平面，X轴指向J2000.0春分点在月球赤道上的投影
\param TDT 简约儒略日表示的地球动力学时(Unit:day)
*/
Kepler MCI_MCI2Kepler(const CDateTime& t,vec3& r,vec3& v)
{
	double T = t.GetTDTCentNum(); //TDT到J2000.0的儒略世纪数
	double T2 = T*T;
	double epson = (23.439291 -    46.8150/3600.0*T - 0.00059/3600.0*T2 ) * RAD;  // 黄赤交角
	double OMEGA = ( 125.0445555 - 1934.136185*T +  7.476/3600.0*T2 ) * RAD;  // 白道升交点平黄经(对应当天平春分点)
	double I = 0.0267617152;  // 月球赤道与黄道的夹角
	double Op = atan2(sin(OMEGA)*cos(I),cos(OMEGA));
	mat33 c = RotationZ(-Op)*RotationX(-I)*RotationZ(OMEGA)*RotationX(epson);
	vec3 Pos = c*r;
	vec3 Vel = c*v;
	return Cartesian_Kepler(Pos,Vel,GM);
}
/*! 地心惯性系(J2000.0惯性系)到月心惯性系的转换
\param r 地心惯性系位置
\param v 地心惯性系速度
\param t CDateTime日期时间
\param rm 月心惯性系位置
\param vm 月心惯性系速度
*/
void ECI_MCI(const CDateTime& t,vec3 & r,vec3& v,vec3 & rm,vec3& vm)
{
	vec3 MPos,MVel;
	DE405::Instance()->PlanetEphemeris(t,EARTH,MOON,MPos,MVel);
    rm = r - MPos;
	vm = v - MVel;
}
/*! MCI到MCF的转换矩阵
is the matrix of coordinate transformation
for moon-fixed coordinate system
to moon-inteial coordinate system   \n

MCFr = MCI_MCF*MCIr;   MCIr = ~MCI_MCF*MCFr;
\param t CDateTime时间
*/
mat33 MCI_MCF(const CDateTime& t)
{
	double Omega,i,u;
	DE405::Instance()->Librations(t,Omega,i,u);
	return RotationZ(u)*RotationX(i)*RotationZ(Omega);
}
/*! ECI到ECF的转换矩阵
	(HG) = (EP)(ER)(NR)(PR)   \n
		is the matrix of coordinate transformation
		for earth-fixed coordinate system
		to celestial coordinate system of epoch   \n
	(-HG) is transpose of matrix (HG)

	ECFr = ECI_ECF*ECIr;   ECIr = ~ECI_ECF*ECFr;
\param t CDateTime时间
\param PolarDriftX 极移量
\param PolarDriftY 极移量
*/
mat33 ECI_ECF(const CDateTime& t)
{
	return PolarMotion(t) * EarthRotation(t) * Nutation(t) * Precession(t);
}

/*!岁差矩阵 precession matrix
  设有一个矢量在J2000.0惯性坐标系下的坐标为r，其在瞬时平赤道坐标系中的坐标为 Precession*r
  反之，一个矢量在瞬时平赤道坐标系下的坐标为r，其在J2000.0惯性坐标系中的坐标为 ~Precession*r
  三个岁差角u1,u2,u3的转序为Precession = Rz(-Z)*Ry(theta)*Rz(-zeta)
\param t CDateTime时间
\return 岁差转换矩阵PR
\[
\left( {PR} \right) = R_z \left( { - z_A } \right)R_y \left( {\theta _A } \right)R_z \left( { - \zeta _A } \right)
\]
*/
mat33 Precession(const CDateTime& t)
{
	double zeta,theta,Z;
	precession_angle(t,zeta,theta,Z);
	return RotationZ(-Z)*RotationY(theta)*RotationZ(-zeta);
}
/*! 岁差系数
- (zeta+Z): 赤经岁差
- theta: 赤纬岁差
\param  t  时间
\param  zeta precession angle ( unit: radian )
\param  theta precession angle ( unit: radian )
\param  Z precession angle ( unit: radian )
\[
\begin{array}{l}
 \zeta _A  = 2306''.2181t + 0''.30188t^2  + 0''.017998t^3  \\ 
 z_A  = 2306''.2181t + 1''.09468t^2  + 0''.018203t^3  \\ 
 \theta _A  = 2004''.3109t - 0''.42665t^2  - 0''.041833t^3  \\ 
 \end{array}
\]

*/
void precession_angle(const CDateTime& t,double& zeta,double& theta,double& Z)
{
	double dt = t.GetTDTCentNum();
	// 206264.806247096 = 3600*DEG
	zeta  = dt*(2306.2181 + dt*(0.30188 + 0.017998*dt) )/206264.806247096;
	theta = dt*(2004.3109 - dt*(0.42665 - 0.041833*dt) )/206264.806247096;
	Z     = dt*(2306.2181 + dt*(1.09468 + 0.018203*dt) )/206264.806247096;
}
/*! 章动矩阵 nutation matrix
  设有一个矢量在瞬时平赤道坐标系下的坐标为r，其在瞬时真赤道坐标系中的坐标为 Nutation*r
  反之，一个矢量在瞬时真赤道坐标系下的坐标为r，其在瞬时平赤道坐标系中的坐标为 ~Nutation*r
  三个章动角u1,u2,u3的转序为Nutation = Rz(-(eps+deps))*Ry(-dksi)*Rx(eps)
\param t CDatetime时间
\return 章动矩阵NU
*/
mat33 Nutation(const CDateTime& t)
{
	double dksi,deps;
	double eps = Ecliptic_Equator_Angle(t);
	nutation_angle(t,dksi,deps);
	return RotationX(-eps-deps)*RotationZ(-dksi)*RotationX(eps);
}
/*!章动系数
- dksi: 黄经章动
- deps: 交角章动
\param t CDatetime时间
\param dksi 黄经章动 ( unit: radian )
\param deps 交角章动 ( unit: radian )
*/
void nutation_angle(const CDateTime& t,double& dksi,double& deps)
{
	/*
	//这里采用刘林《航天器轨道理论》中的方法计算章动角，经过与
	//JPL DE405比较，章动角的差别小于0.3"
	//static const double A0[5] = {-17.1996,-1.3187, -0.2274, 0.2026,  0.1426  };
	//static const double A1[5] = {-0.01742,-0.00016,-0.00002,0.00002,-0.00034 };
	//static const double B0[5] = {   9.2025,   0.5736,   0.0977, -0.0859,   0.0054 };
	//static const double B1[5] = {  0.00089, -0.00031, -0.00005, 0.00005, -0.00001 };
	// 将上面常数的单位由角秒转为弧度:
	static const double A0[5] = {-8.33860138961158e-005, -6.39323801279145e-006, -1.10246631084308e-006, 9.8223251792792e-007, 6.91344309262198e-007 };
	static const double A1[5] = {-8.44545432492812e-008, -7.75701889775258e-010, -9.69627362219072e-011, 9.69627362219072e-011, -1.64836651577242e-009 };
	static const double B0[5] = {4.4614979004105e-005,  2.7808912748443e-006,  4.73662966444017e-007,  -4.16454952073091e-007,  2.61799387799149e-008 };
	static const double B1[5] = {4.31484176187487e-009, -1.50292241143956e-009,  -2.42406840554768e-010,  2.42406840554768e-010, -4.84813681109536e-011 };
	double alfa[5],beta[5];
	double dt = t.GetTDTCentNum();
	alfa[0] = (1325*360.0 + 198.86739806)*dt;
	alfa[1] = (  99*360.0 + 359.05034000)*dt;
	alfa[2] = (1342*360.0 + 82.01753806 )*dt;
	alfa[3] = (1236*360.0 + 307.11148000)*dt;
	alfa[4] =-(   5*360.0 + 134.13626083)*dt;
	int i;
	for(i=0;i<5;i++)    alfa[i] = fmod(alfa[i]+3600,360);
	alfa[0] = (alfa[0] + 134.96298139 + 0.00869722*dt*dt)*RAD;
	alfa[1] = (alfa[1] + 357.52772333 - 0.00016028*dt*dt)*RAD;
	alfa[2] = (alfa[2] + 93.271910280 - 0.00368250*dt*dt)*RAD;
	alfa[3] = (alfa[3] + 297.85036306 - 0.00191417*dt*dt)*RAD;
	alfa[4] = (alfa[4] + 125.04452222 + 0.00207083*dt*dt)*RAD;
	beta[0] = alfa[4];
	beta[1] = 2.0*(alfa[2] - alfa[3] + alfa[4]);
	beta[2] = 2.0*(alfa[2] + alfa[4]);
	beta[3] = 2.0*alfa[4];
	beta[4] = alfa[1];
	dksi = 0.0;
	deps = 0.0;
	for(i=0;i<5;i++)
	{
		dksi += (A0[i]+A1[i]*dt)*sin(beta[i]);
		deps += (B0[i]+B1[i]*dt)*cos(beta[i]);
	}*/
	

	/* IAU 1980 Theory of Nutation */

	const double rev = 360.0*3600.0;  // arcsec/revolution
        
	const int  N_coeff = 106;
	const long C[106][9] = {
        {  0, 0, 0, 0, 1,-1719960,-1742,  920250,   89 },   //   1
        {  0, 0, 0, 0, 2,   20620,    2,   -8950,    5 },   //   2
        { -2, 0, 2, 0, 1,     460,    0,    -240,    0 },   //   3
        {  2, 0,-2, 0, 0,     110,    0,       0,    0 },   //   4
        { -2, 0, 2, 0, 2,     -30,    0,      10,    0 },   //   5
        {  1,-1, 0,-1, 0,     -30,    0,       0,    0 },   //   6
        {  0,-2, 2,-2, 1,     -20,    0,      10,    0 },   //   7
        {  2, 0,-2, 0, 1,      10,    0,       0,    0 },   //   8
        {  0, 0, 2,-2, 2, -131870,  -16,   57360,  -31 },   //   9
        {  0, 1, 0, 0, 0,   14260,  -34,     540,   -1 },   //  10
        {  0, 1, 2,-2, 2,   -5170,   12,    2240,   -6 },   //  11
        {  0,-1, 2,-2, 2,    2170,   -5,    -950,    3 },   //  12
        {  0, 0, 2,-2, 1,    1290,    1,    -700,    0 },   //  13
        {  2, 0, 0,-2, 0,     480,    0,      10,    0 },   //  14
        {  0, 0, 2,-2, 0,    -220,    0,       0,    0 },   //  15
        {  0, 2, 0, 0, 0,     170,   -1,       0,    0 },   //  16
        {  0, 1, 0, 0, 1,    -150,    0,      90,    0 },   //  17
        {  0, 2, 2,-2, 2,    -160,    1,      70,    0 },   //  18
        {  0,-1, 0, 0, 1,    -120,    0,      60,    0 },   //  19
        { -2, 0, 0, 2, 1,     -60,    0,      30,    0 },   //  20
        {  0,-1, 2,-2, 1,     -50,    0,      30,    0 },   //  21
        {  2, 0, 0,-2, 1,      40,    0,     -20,    0 },   //  22
        {  0, 1, 2,-2, 1,      40,    0,     -20,    0 },   //  23
        {  1, 0, 0,-1, 0,     -40,    0,       0,    0 },   //  24
        {  2, 1, 0,-2, 0,      10,    0,       0,    0 },   //  25
        {  0, 0,-2, 2, 1,      10,    0,       0,    0 },   //  26
        {  0, 1,-2, 2, 0,     -10,    0,       0,    0 },   //  27
        {  0, 1, 0, 0, 2,      10,    0,       0,    0 },   //  28
        { -1, 0, 0, 1, 1,      10,    0,       0,    0 },   //  29
        {  0, 1, 2,-2, 0,     -10,    0,       0,    0 },   //  30
        {  0, 0, 2, 0, 2,  -22740,   -2,    9770,   -5 },   //  31
        {  1, 0, 0, 0, 0,    7120,    1,     -70,    0 },   //  32
        {  0, 0, 2, 0, 1,   -3860,   -4,    2000,    0 },   //  33
        {  1, 0, 2, 0, 2,   -3010,    0,    1290,   -1 },   //  34
        {  1, 0, 0,-2, 0,   -1580,    0,     -10,    0 },   //  35
        { -1, 0, 2, 0, 2,    1230,    0,    -530,    0 },   //  36
        {  0, 0, 0, 2, 0,     630,    0,     -20,    0 },   //  37
        {  1, 0, 0, 0, 1,     630,    1,    -330,    0 },   //  38
        { -1, 0, 0, 0, 1,    -580,   -1,     320,    0 },   //  39
        { -1, 0, 2, 2, 2,    -590,    0,     260,    0 },   //  40
        {  1, 0, 2, 0, 1,    -510,    0,     270,    0 },   //  41
        {  0, 0, 2, 2, 2,    -380,    0,     160,    0 },   //  42
        {  2, 0, 0, 0, 0,     290,    0,     -10,    0 },   //  43
        {  1, 0, 2,-2, 2,     290,    0,    -120,    0 },   //  44
        {  2, 0, 2, 0, 2,    -310,    0,     130,    0 },   //  45
        {  0, 0, 2, 0, 0,     260,    0,     -10,    0 },   //  46
        { -1, 0, 2, 0, 1,     210,    0,    -100,    0 },   //  47
        { -1, 0, 0, 2, 1,     160,    0,     -80,    0 },   //  48
        {  1, 0, 0,-2, 1,    -130,    0,      70,    0 },   //  49
        { -1, 0, 2, 2, 1,    -100,    0,      50,    0 },   //  50
        {  1, 1, 0,-2, 0,     -70,    0,       0,    0 },   //  51
        {  0, 1, 2, 0, 2,      70,    0,     -30,    0 },   //  52
        {  0,-1, 2, 0, 2,     -70,    0,      30,    0 },   //  53
        {  1, 0, 2, 2, 2,     -80,    0,      30,    0 },   //  54
        {  1, 0, 0, 2, 0,      60,    0,       0,    0 },   //  55
        {  2, 0, 2,-2, 2,      60,    0,     -30,    0 },   //  56
        {  0, 0, 0, 2, 1,     -60,    0,      30,    0 },   //  57
        {  0, 0, 2, 2, 1,     -70,    0,      30,    0 },   //  58
        {  1, 0, 2,-2, 1,      60,    0,     -30,    0 },   //  59
        {  0, 0, 0,-2, 1,     -50,    0,      30,    0 },   //  60
        {  1,-1, 0, 0, 0,      50,    0,       0,    0 },   //  61
        {  2, 0, 2, 0, 1,     -50,    0,      30,    0 },   //  62
        {  0, 1, 0,-2, 0,     -40,    0,       0,    0 },   //  63
        {  1, 0,-2, 0, 0,      40,    0,       0,    0 },   //  64
        {  0, 0, 0, 1, 0,     -40,    0,       0,    0 },   //  65
        {  1, 1, 0, 0, 0,     -30,    0,       0,    0 },   //  66
        {  1, 0, 2, 0, 0,      30,    0,       0,    0 },   //  67
        {  1,-1, 2, 0, 2,     -30,    0,      10,    0 },   //  68
        { -1,-1, 2, 2, 2,     -30,    0,      10,    0 },   //  69
        { -2, 0, 0, 0, 1,     -20,    0,      10,    0 },   //  70
        {  3, 0, 2, 0, 2,     -30,    0,      10,    0 },   //  71
        {  0,-1, 2, 2, 2,     -30,    0,      10,    0 },   //  72
        {  1, 1, 2, 0, 2,      20,    0,     -10,    0 },   //  73
        { -1, 0, 2,-2, 1,     -20,    0,      10,    0 },   //  74
        {  2, 0, 0, 0, 1,      20,    0,     -10,    0 },   //  75
        {  1, 0, 0, 0, 2,     -20,    0,      10,    0 },   //  76
        {  3, 0, 0, 0, 0,      20,    0,       0,    0 },   //  77
        {  0, 0, 2, 1, 2,      20,    0,     -10,    0 },   //  78
        { -1, 0, 0, 0, 2,      10,    0,     -10,    0 },   //  79
        {  1, 0, 0,-4, 0,     -10,    0,       0,    0 },   //  80
        { -2, 0, 2, 2, 2,      10,    0,     -10,    0 },   //  81
        { -1, 0, 2, 4, 2,     -20,    0,      10,    0 },   //  82
        {  2, 0, 0,-4, 0,     -10,    0,       0,    0 },   //  83
        {  1, 1, 2,-2, 2,      10,    0,     -10,    0 },   //  84
        {  1, 0, 2, 2, 1,     -10,    0,      10,    0 },   //  85
        { -2, 0, 2, 4, 2,     -10,    0,      10,    0 },   //  86
        { -1, 0, 4, 0, 2,      10,    0,       0,    0 },   //  87
        {  1,-1, 0,-2, 0,      10,    0,       0,    0 },   //  88
        {  2, 0, 2,-2, 1,      10,    0,     -10,    0 },   //  89
        {  2, 0, 2, 2, 2,     -10,    0,       0,    0 },   //  90
        {  1, 0, 0, 2, 1,     -10,    0,       0,    0 },   //  91
        {  0, 0, 4,-2, 2,      10,    0,       0,    0 },   //  92
        {  3, 0, 2,-2, 2,      10,    0,       0,    0 },   //  93
        {  1, 0, 2,-2, 0,     -10,    0,       0,    0 },   //  94
        {  0, 1, 2, 0, 1,      10,    0,       0,    0 },   //  95
        { -1,-1, 0, 2, 1,      10,    0,       0,    0 },   //  96
        {  0, 0,-2, 0, 1,     -10,    0,       0,    0 },   //  97
        {  0, 0, 2,-1, 2,     -10,    0,       0,    0 },   //  98
        {  0, 1, 0, 2, 0,     -10,    0,       0,    0 },   //  99
        {  1, 0,-2,-2, 0,     -10,    0,       0,    0 },   // 100
        {  0,-1, 2, 0, 1,     -10,    0,       0,    0 },   // 101
        {  1, 1, 0,-2, 1,     -10,    0,       0,    0 },   // 102
        {  1, 0,-2, 2, 0,     -10,    0,       0,    0 },   // 103
        {  2, 0, 0, 2, 0,      10,    0,       0,    0 },   // 104
        {  0, 0, 2, 4, 2,     -10,    0,       0,    0 },   // 105
        {  0, 1, 0, 1, 0,      10,    0,       0,    0 }    // 106
    };
        
    // Variables
    double  l, lp, F, D, Om; //Om changed to class variable
    double  arg;
    double T = t.GetTDTCentNum();
	double T2 = T*T;
	double T3 = T2*T;
    // Mean arguments of luni-solar motion
    //   l   mean anomaly of the Moon
    //   l'  mean anomaly of the Sun
    //   F   mean argument of latitude
    //   D   mean longitude elongation of the Moon from the Sun
    //   Om  mean longitude of the ascending node
    // IERS tech report 21 1996 conventions (below)
	l  = fmod( 134.96340251*3600 + 1717915923.2178*T  + 31.8792*T2 + 0.051635*T3, rev );
	lp = fmod( 357.52910918*3600 + 129596581.0481*T  -  0.5532*T2 - 0.000136*T3, rev );
	F  = fmod( 93.27209062*3600 + 1739527262.8478*T  - 12.7512*T2 - 0.001037*T3, rev );
	D  = fmod( 297.85019547*3600 + 1602961601.2090*T  -  6.3706*T2 + 0.006593*T3, rev );
    Om = fmod( 450160.398036 - 6962890.2665*T + 7.4722*T2 + 0.007702*T3 - 0.00005939*T3*T, rev );
// JAT,经过与上面IERS对比，对章动角计算的差别在2*10^-5量级
//    l  = fmod(  485866.733 + (1325.0*rev +  715922.633)*T + 31.310*T2 + 0.064*T3, rev );
//    lp = fmod( 1287099.804 + (  99.0*rev + 1292581.224)*T -  0.577*T2 - 0.012*T3, rev );
//    F  = fmod(  335778.877 + (1342.0*rev +  295263.137)*T - 13.257*T2 + 0.011*T3, rev );
//    D  = fmod( 1072261.307 + (1236.0*rev + 1105601.328)*T -  6.891*T2 + 0.019*T3, rev );
//    Om = MathUtils.Modulo(  450160.280 - (   5.0*rev +  482890.539)*T +  7.455*T2 + 0.008*T3, rev );
       
    // Nutation in longitude and obliquity [rad]
    const double Arcs = RAD/3600.0;
        
	deps = 0.0;
	dksi = 0.0;
	for (int i=0; i<N_coeff; i++) {
		arg  =  ( C[i][0]*l + C[i][1]*lp + C[i][2]*F + C[i][3]*D + C[i][4]*Om ) * Arcs;
		dksi += ( C[i][5]+C[i][6]*T ) * sin(arg);
		deps += ( C[i][7]+C[i][8]*T ) * cos(arg);
	}
// TODO
//* Delta Delta corrections
//        double ddpsi = -0.0534;
//        double ddeps = -0.00472;
//        this.dpsi = this.dpsi + ddpsi;
//        this.deps = this.deps + ddeps;
        
	dksi = 1.0E-5 * dksi*Arcs;
	deps = 1.0E-5 * deps*Arcs;
}
/*! 地球自转矩阵earth rotatioin matrix
  设有一个矢量在瞬时真赤道坐标系下的坐标为r，其在准地固坐标系中的坐标为 EarthRotation*r
  反之，一个矢量在准地固坐标系下的坐标为r，其在瞬时真赤道坐标系中的坐标为 ~EarthRotation*r
  格林威治恒星时SG     转序为EarthRotation = Rz(SG)
\param t CDatetime时间
\return 地球自转矩阵
*/
mat33 EarthRotation(const CDateTime& t)
{
	return RotationZ( GetSG(t) );
}
/*!  地球极移矩阵
[1,   0,   Xp]
[0,   1,  -Yp]
[Xp,  Yp,   1]
设有一个矢量在准地固坐标系下的坐标为r，其在地固坐标系中的坐标为 PolarMotion*r
反之，一个矢量在地固坐标系下的坐标为r，其在准地固坐标系中的坐标为 ~PolarMotion*r
*/
mat33 PolarMotion(const CDateTime& t)
{
	double Xp,Yp;
	Earth::Instance()->GetPolarMotion(t,Xp,Yp);
	mat33 m;
	m.eye();
	m(0,2) = Xp;
	m(2,0) = -Xp;
	m(1,2) = -Yp;
	m(2,1) = Yp;
	return m;
}
/*!卫星轨道坐标系相对惯性系的方向余弦阵
      [ 0  1   0]
Coi = [ 0  0  -1]*RotationZ(u)*RotationX(i)*RotationZ(Omega)
      [-1  0   0]
\param sat 卫星
\return 卫星轨道坐标系相对惯性系的方向余弦阵
*/
mat33 GetCoi(const Kepler& k)
{
	mat33 T;
	T.zeros();
	T(0,1) = 1.0;    T(1,2) = -1.0;    T(2,0) = -1.0;
	mat33 C = RotationZ(k.u())*RotationX(k.i)*RotationZ(k.o);
	return T*C;
}

/*!绕X轴转角angle的旋转矩阵
[ 1          0         0     ]
[ 0    cos(angle)  sin(angle)]
[ 0   -sin(angle)  cos(angle)]
\param angle 转过的角(弧度)
\return X旋转矩阵
*/
mat RotationX(double angle)
{
	mat33 m;m.zeros();
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
mat RotationY(double angle)
{
	mat33 m;m.zeros();
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
mat RotationZ(double angle)
{
	mat33 m;m.zeros();
	m(0,0) = cos(angle);
	m(1,1) = m(0,0);
	m(2,2) = 1.0;
	m(0,1) = sin(angle);
	m(1,0) = -m(0,1);
	return m;
}

/*!由三个行矢量x,y,z组成一个矩阵
    [x.x  x.y  x.z]
M = [y.x  y.y  y.z]
    [z.x  z.y  z.z]
\param x X矢量
\param y Y矢量
\param z Z矢量
\return 组成的矩阵
*/
mat33 Vector2Matrix(const vec3& x,const vec3& y,const vec3& z)
{
	mat33 m;
	m.row(0) = x.t();
	m.row(1) = y.t();
	m.row(2) = z.t();
	return m;
}

/*!三个数取最大值*/
double max3( double a, double b, double c ) 
{
	double m = a>b?a:b;
	return m>c?m:c;
}

/*!三个数取绝对值最大值*/
double maxabs3( double a, double b, double c ) 
{
	a = fabs(a);
	b = fabs(b);
	c = fabs(c);
	double m = a>b?a:b;
	return m>c?m:c;
}

//!矢量外积
mat33 out_product(const vec3 &v1,const vec3& v2)
{
	return v1*v2.t();
}

//!双矢量定姿
mat33 Avv( vec3 X1I, vec3 X2I, vec3 X1B, vec3 X2B ) 
{
	vec3 V2I = cross(X1I,X2I);
	V2I = V2I/norm(V2I,2);
	vec3 V3I = cross(X1I,V2I);
	vec3 V2B = cross(X1B,X2B);
	V2B = V2B/norm(V2B,2);
	vec3 V3B = cross(X1B,V2B);
	return out_product(X1B,X1I) + out_product(V2B,V2I) + out_product(V3B,V3I);
}

#if _MSC_VER < 1400
void fopen_s(FILE** fp,const char* filename,const char* rw)
{
	*fp = fopen(filename,rw);
}
#endif // _MSC_VER < 1400
