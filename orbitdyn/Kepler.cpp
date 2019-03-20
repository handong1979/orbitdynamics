#include "Kepler.h"

//! 解开普勒方程 M = E - e*sin(E)
// 即已知e和M，找到E满足开普勒方程
// 采用牛顿迭代法解方程，定义函数:
// f(E) = E - e*sin(E) - M
// 则 f'(E) = 1 - e*cos(E)
// 通过下式进行迭代计算E:
//                  f(E|k)
// E|k+1 = E|k - ----------
//                 f'(E|k)
// 对于双曲线轨道，开普勒方程为 M = e*sin(E) - E，解法类似
ORBITDYN_API double KeplerFunc(double ee,double MM)
{
	double E0 = MM;
	double E1;
	if(ee<=1.0) E1 = E0-(E0-ee*sin(E0)-MM)/(1-ee*cos(E0));
	else       E1 = E0-(ee*sinh(E0)-E0-MM)/(ee*cosh(E0)-1.0);
	while(fabs(E1-E0)>1e-8)
	{
		E0 = E1;
		if(ee<=1.0) E1 = E0-(E0-ee*sin(E0)-MM)/(1-ee*cos(E0));
		else       E1 = E0-(ee*sinh(E0)-E0-MM)/(ee*cosh(E0)-1.0);
	}
	return E1;
}

//! 解广义开普勒方程，返回值为E+w
// 这里和刘林书上有点不同，此处定义为：
// ksi = e*cos(w)
// eta = e*sin(w)
// lamda = w + M
// 广义开普勒方程写为
// u~ - lamda = ksi*sin(u~) - eta*cos(u~) 
// 其中u~ = w + E
// 采用牛顿迭代法，定义方程
// f(u~) = u~ - lamda - (ksi*sin(u~) - eta*cos(u~))
// 则 f'(u~) = 1 - (ksi*cos(u~) + eta*sin(u~))
ORBITDYN_API double KeplerFunc2(double ksi,double eta,double lamda)
{
	int JJ = 0;
	double Ew = lamda + ksi*sin(lamda) - eta*cos(lamda);
	double sEw = sin(Ew);
	double cEw = cos(Ew);
	double W0 = Ew - (lamda + ksi*sEw - eta*cEw);
	while(fabs(W0)>1e-8)
	{
		Ew = Ew - W0 / (1.0 - (ksi*cEw + eta*sEw));
		JJ++;
		if(JJ>20){
			KeplerException ke;
			ke.SetDetails("KeplerFunc2(): Too many iterations,ksi=%e,eta=%e,lamda=%e",ksi,eta,lamda);
			throw ke;
		}
		sEw = sin(Ew);
		cEw = cos(Ew);
		W0 = Ew - (lamda + ksi*sEw - eta*cEw);
	}
	return Ew;
}

//默认构造
Kepler::Kepler():a(6978.14),e(0.0),i(0.0),o(0.0),w(0.0),M(0.0)
{
}

//类型转换
Kepler::Kepler(const double oe[6]) 
{
	a = oe[0];	e = oe[1];	i = oe[2];
	o = oe[3];	w = oe[4];	M = oe[5];
} 

Kepler::Kepler(double SemimajorAxis,double Eccentricity,double Inclination,
	   double RAAN,double ArgPerigee,double MeanAnomaly)
{   
	a = SemimajorAxis;		e = Eccentricity;		i = Inclination;
	o = RAAN;				w = ArgPerigee;			M = MeanAnomaly;
}

Kepler::~Kepler()
{

}


double Kepler::Axis() const 
{
	return a; 
}

double Kepler::Ecce() const 
{ 
	return e;
}

double Kepler::Incl() const
{ 
	return i*DEG;
}

double Kepler::RAAN() const
{ 
	return o*DEG; 
}

double Kepler::AofP() const
{ 
	return w*DEG; 
}

double Kepler::MeanA() const
{ 
	return M*DEG; 
}

double Kepler::r() const
{
	if(a>0)
		return a*( 1.0 - e*cos(KeplerFunc(e,M)) );
	else
		return a*( 1.0 - e*cosh(KeplerFunc(e,M)) );
}

double Kepler::v() const
{
	if(a>0)
		return sqrt(2.0/(a*( 1.0 - e*cos(KeplerFunc(e,M)))) - 1.0/a );
	else
		return sqrt(2.0/(a*( 1.0 - e*cosh(KeplerFunc(e,M)))) - 1.0/a );
}

double Kepler::p() const
{ 
	return a*(1-e*e);
}

//! return f
double Kepler::f() const
{
	double E = KeplerFunc(e,M);
	if(e<=1.0)
		return fmod(atan2(a*sqrt(1-e*e)*sin(E),a*(cos(E)-e))+PI2,PI2);
	else
		return fmod(atan2(-a*sqrt(e*e-1)*sinh(E),a*(cosh(E)-e))+PI2,PI2);
}

//! return u
double Kepler::u() const
{
	return fmod(f()+w+PI2,PI2);
}

//! return E
double Kepler::E() const
{
	return KeplerFunc(e,M);
}

double Kepler::n(double miu /*= GE*/) const
{
	return a>0 ? sqrt(miu/a/a/a) : sqrt(-miu/a/a/a); 
}

double Kepler::ApogeeRadius() const
{
	return a*(1+e);
}

double Kepler::PerigeeRadius() const
{
	return a*(1-e);
}

double Kepler::ApogeeAltitude(double r /*= Re*/) const
{
	return a*(1+e)-r;
}

double Kepler::PerigeeAltitude(double r /*= Re*/) const
{
	return a*(1-e)-r; 
}

//! 设置轨道根数,输入角度必须为弧度，轨道位置为平近点角
void Kepler::SetElements(double SemimajorAxis,double Eccentricity,double Inclination,
								double RAAN,double ArgPerigee,double MeanAnomaly)
{
	a = SemimajorAxis;	e = Eccentricity;	i = Inclination;
	o = RAAN;			w = ArgPerigee;		M = MeanAnomaly;
}
//! 设置轨道根数,输入角度必须为弧度，轨道位置为真近点角
void Kepler::SetElementsF(double SemimajorAxis,double Eccentricity,double Inclination,
								 double RAAN,double ArgPerigee,double TrueAnomaly)
{
	a = SemimajorAxis;	e = Eccentricity;	i = Inclination;
	o = RAAN;			w = ArgPerigee;
	double r = a*(1-e*e)/(1+e*cos(TrueAnomaly));
	// 对于椭圆和双曲线，下式在形式上基本相同，只是在根号下差一个符号，只要对1-e^2取绝对值就可以了
	double E;
	if(e<1.0)
	{
		E = fmod(atan2(r*sin(TrueAnomaly)/a/sqrt(1-e*e),r/a*cos(TrueAnomaly)+e)+PI2,PI2);
		M = E - e*sin(E);
	}
	else if(e>1.0)
	{
		double x = (-r*sin(TrueAnomaly)/a/sqrt(e*e-1)) / (r/a*cos(TrueAnomaly)+e);
		E = 0.5*log((1+x)/(1-x));
		M = e*sinh(E) - E;
	}
	else
	{
		//??
	}
}
//! 设置轨道根数,输入角度必须为弧度，轨道位置为轨道幅角
void Kepler::SetElementsU(double SemimajorAxis,double Eccentricity,double Inclination,
								 double RAAN,double ArgPerigee,double Argument)
{
	a = SemimajorAxis;	e = Eccentricity;	i = Inclination;
	o = RAAN;			w = ArgPerigee;
	double r = a*(1-e*e)/(1+e*cos(Argument-w));

	double E;

	if(e<1.0)
	{
		E = fmod(atan2(r*sin(Argument-w)/a/sqrt(1-e*e),r/a*cos(Argument-w)+e)+PI2,PI2);
		M = E - e*sin(E);
	}
	else if(e>1.0)
	{
		double x = (-r*sin(Argument-w)/a/sqrt(e*e-1)) / (r/a*cos(Argument-w)+e);
		E = 0.5*log((1+x)/(1-x));
		M = e*sinh(E) - E;
	}
	else
	{
		//??
	}
}
//! 设置轨道根数,输入角度必须为弧度
void Kepler::SetElements(const double elem[6])
{
	a = elem[0];	e = elem[1];	i = elem[2];
	o = elem[3];	w = elem[4];	M = elem[5];
}

//! 轨道六根数存入数组
double * Kepler::ToArray(double array[6])
{
	array[0]=a;	array[1]=e;	array[2]=i;
	array[3]=o;	array[4]=w;	array[5]=M;
	return array;
}


// 标准输出流,角度单位为度
ORBITDYN_API ostream & operator << (ostream & out,const Kepler & kp)
{
//#define SPACE  "     "
//	out.setf(ios::fixed);
//	int w = out.width();
//	out.precision(5);
//	out.width(11);
//	out<<kp.a<<SPACE;
//	out.width(11);
//	out.precision(8);
//	out<<kp.e<<SPACE;
//	out.precision(5);
//	out.width(9);
//	out<<kp.i*DEG<<SPACE;
//	out.width(9);
//	out<<kp.o*DEG<<SPACE;
//	out.width(9);
//	out<<kp.w*DEG<<SPACE;
//	out.width(9);
//	out<<kp.M*DEG;
//	out.width(w);
//	out.unsetf(ios::fixed);
//	return out;
//#undef SPACE
	out << kp.a << "\t" << kp.e << "\t" << kp.i*DEG << "\t" << kp.o*DEG << "\t" << kp.w*DEG << "\t" << kp.M*DEG;
	return out;
}

// 标准输入流,角度单位为弧度
ORBITDYN_API istream & operator >> (istream & in,Kepler & kp)
{
	in>>kp.a;	in>>kp.e;	in>>kp.i;
	in>>kp.o;	in>>kp.w;	in>>kp.M;
	return in;
}
