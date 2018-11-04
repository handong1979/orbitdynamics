#include "librationsat.h"

const double miu = (GE+GM)/(GS+GM+GE);  // 3.04042345408276e-006
const double gamaL1 = 0.01001097735839; // root of equation: x^5 - (3-miu)*x^4 + (3-2*miu)*x^3 - miu*x^2 + 2*miu*x - miu=0
const double BL1 = (1-miu)/pow(1-gamaL1,3) + miu/pow(gamaL1,3); // 4.061074;
const double CL1 = (1-miu)/pow(1-gamaL1,4) + miu/pow(gamaL1,4);// 303.75203181
const double DL1 = (1-miu)/pow(1-gamaL1,5) + miu/pow(gamaL1,5);// 30238.9552;

const double TU = sqrt(AU*AU*AU/(GS+GE+GM));
const double VU = AU/TU;


void ECI_RLP(const CDateTime& t,Vector& r_GCI,Vector& v_GCI,Vector& r_RLP,Vector& v_RLP)
{
	Matrix Q,dQ;  // ECI==>RLP转换矩阵

	Vector r_SE,v_SE;
	DE405::Instance()->PlanetEphemeris(t,SUN,EARTH_MOON_BARYCENTER,r_SE,v_SE);
	double R = r_SE.Module();  // 日-地位置矢量
	double V = v_SE.Module();  // 地球相对太阳的速度矢量

	Vector x = r_SE/R;
	Vector y = Cross(Cross(r_SE,v_SE),r_SE);
	Vector z = Cross(r_SE,v_SE);
	Vector dx = v_SE/R - Dot(r_SE,v_SE)/R/R/R*r_SE;
	Vector dy = Cross(z,v_SE)/R/z.Module() - Dot(r_SE,v_SE)/R/R/R/z.Module()*y;
	y = y/y.Module();
	z = z/z.Module();

	Q(0,0) = x.x;	dQ(0,0) = dx.x;
	Q(0,1) = x.y;   dQ(0,1) = dx.y;
	Q(0,2) = x.z;   dQ(0,2) = dx.z;
	Q(1,0) = y.x;   dQ(1,0) = dy.x;
	Q(1,1) = y.y;   dQ(1,1) = dy.y;
	Q(1,2) = y.z;   dQ(1,2) = dy.z;
	Q(2,0) = z.x;   dQ(2,0) = 0;
	Q(2,1) = z.y;   dQ(2,1) = 0;
	Q(2,2) = z.z;   dQ(2,2) = 0;

	Vector r_EMB,v_EMB; //地月系质心在地心惯性系的矢量
	DE405::Instance()->PlanetEphemeris(t,EARTH,EARTH_MOON_BARYCENTER,r_EMB,v_EMB);
	Vector r0 = -r_SE*gamaL1 + r_EMB; // 地心惯性系下L1点位置
	Vector v0 = -v_SE*gamaL1 + v_EMB; // 地心惯性系下L1点速度
	
	r_RLP = Q*(r_GCI-r0);
	v_RLP = dQ*(r_GCI-r0) + Q*(v_GCI-v0); 
}

void RLP_ECI(const CDateTime& t,Vector& r_RLP,Vector& v_RLP,Vector& r_GCI,Vector& v_GCI)
{
	Matrix Q,dQ;  // ECI==>RLP转换矩阵

	Vector r_SE,v_SE;
	DE405::Instance()->PlanetEphemeris(t,SUN,EARTH_MOON_BARYCENTER,r_SE,v_SE);
	double R = r_SE.Module();
	double V = v_SE.Module();

	Vector x = r_SE/R;
	Vector y = Cross(Cross(r_SE,v_SE),r_SE);
	Vector z = Cross(r_SE,v_SE);
	Vector dx = v_SE/R - Dot(r_SE,v_SE)/R/R/R*r_SE;
	Vector dy = Cross(z,v_SE)/R/z.Module() - Dot(r_SE,v_SE)/R/R/R/z.Module()*y;
	y = y/y.Module();
	z = z/z.Module();

	// 这里的Q与ECI_RLP中的Q互为转置
	Q(0,0) = x.x;	dQ(0,0) = dx.x;
	Q(1,0) = x.y;   dQ(0,1) = dx.y;
	Q(2,0) = x.z;   dQ(0,2) = dx.z;
	Q(0,1) = y.x;   dQ(1,0) = dy.x;
	Q(1,1) = y.y;   dQ(1,1) = dy.y;
	Q(2,1) = y.z;   dQ(1,2) = dy.z;
	Q(0,2) = z.x;   dQ(2,0) = 0;
	Q(1,2) = z.y;   dQ(2,1) = 0;
	Q(2,2) = z.z;   dQ(2,2) = 0;

	Vector r_EMB,v_EMB; //地月系质心在地心惯性系的矢量
	DE405::Instance()->PlanetEphemeris(t,EARTH,EARTH_MOON_BARYCENTER,r_EMB,v_EMB);
	Vector r0 = -r_SE*gamaL1 + r_EMB;
	Vector v0 = -v_SE*gamaL1 + v_EMB;

	r_GCI = r0 + Q*r_RLP;
	v_GCI = v0 - Q*dQ*Q*r_RLP + Q*v_RLP; 
}


CLibrationSat::CLibrationSat(void)
{
	LL = 1;
	Perturbation = ODP_SOLAR_CENT|ODP_LUNAR_CENT;
}

CLibrationSat::~CLibrationSat(void)
{
}

void CLibrationSat::Initialize(const CDateTime& dt,const Vector& r,const Vector& v)
{
	CSatelliteBase::Initialize(dt,r,v);
}

void CLibrationSat::RefreshKepler()
{
}

void TestLibrationSat()
{
	/*
	Time (UTCG):  12 Feb 2013 17:00:00.00
	x (km):       808144.181749 
	y (km):       -1340918.106143
	z (km):       -450154.530038
	vx (km/sec):  0.170620      
	vy (km/sec):  0.241594      
	vz (km/sec):  0.104125      
	*/
	
//	Vector r(759126.022571,-1402743.726564,-476956.923856);  // Ay = 750000
//	Vector v(0.164261,0.245838,0.105964);

	CDateTime epoch(2013,2,12,17,0,0);
	cout.precision(12);

	Vector r_RLP,v_RLP,r,v;
//	Vector r(808144.181749,-1340918.106143,-450154.530038); // Ay = 666672
//	Vector v(0.170620,0.241594,0.104125);

	//r_RLP.SetValue(0.0,666672.0,120000.0); // Ay = 666672
	//v_RLP.SetValue(0.15144106229,0.0,0.0);

	//r_RLP.SetValue(0.0,672.0,0); // Ay = 666672
	//v_RLP.SetValue(8.64454691702713e-005,0.0,0.0);

	r_RLP.SetValue(0.0,0.0,0.0); // Ay = 666672
	v_RLP.SetValue(0.0,0.00005,0.0);

	RLP_ECI(epoch,r_RLP,v_RLP,r,v);
	cout<<r<<endl<<v<<endl;
		
	CLibrationSat sat;
	sat.Initialize(epoch,r,v);

	fstream fxyz("sat.dat",ios::out);	fxyz.precision(12);
	fstream fem("EM.dat",ios::out);    fem.precision(12);
    
	Vector rEEM,vEEM,rM,vM;
	Vector rEEM_L1,vEEM_L1,rM_L1,vM_L1;
	Vector rE,vE;

	double step = 7200;
	for(double t=0;t<=86400*365;t+=step)
	{
		DE405::Instance()->PlanetEphemeris(epoch+t,EARTH,EARTH_MOON_BARYCENTER,rEEM,vEEM); // 地月系质心的位置速度
		DE405::Instance()->PlanetEphemeris(epoch+t,EARTH,MOON,rM,vM); // 月球的位置速度

		ECI_RLP(epoch+t,sat.Pos(),sat.Vel(),r_RLP,v_RLP);
		ECI_RLP(epoch+t,rE,vE,rEEM_L1,vEEM_L1);
		ECI_RLP(epoch+t,rM,vM,rM_L1,vM_L1);

		fxyz<<t/86400<<TAB<<sat.Pos()<<TAB<<sat.Vel()<<TAB<<r_RLP<<TAB<<v_RLP<<endl;
		fem<<t/86400<<TAB<<rEEM_L1<<TAB<<rM_L1<<endl; // “L1-地月质心”和“L1-月球”矢量
		
		sat.Propagate(step,step);
	}
}

/* draw m code
%%
dat = load('sat.dat');
em = load('EM.dat');
% stk = load('stk.dat');
% plot6([dat(:,1) dat(:,2:7)-stk(:,:)]);
figure,plot3(dat(:,8),dat(:,9),dat(:,10)),grid on,hold on,xlabel('x'),ylabel('y'),zlabel('z');
plot(0,0,'.'),plot(1497620.88943337,0,'.');
plot3(em(:,5),em(:,6),em(:,7));plot3(em(:,2),em(:,3),em(:,4));

figure
subplot 121,plot(dat(:,8),dat(:,9)),grid on,hold on,xlabel('x'),ylabel('y');
% plot(0,0,'.'),plot(1497620.88943337,0,'.')%,text(0,0,'L1')%,text(1.49e6,0,'Earth');
subplot 122,plot(dat(:,8),dat(:,10)),grid on,hold on,xlabel('x'),ylabel('z');
% plot(0,0,'.'),plot(1497620.88943337,0,'.')%,text(0,0,'L1')%,text(1.49e6,0,'Earth');

%%
miu = (GEarth+GMoon)/(GSun+GMoon+GEarth);
gamaL1 = 0.01001097735839;
BL1 = (1-miu)/(1-gamaL1)^3 + miu/gamaL1^3;
CL1 = (1-miu)/(1-gamaL1)^4 + miu/gamaL1^4;
DL1 = (1-miu)/(1-gamaL1)^5 + miu/gamaL1^5;
wxy = sqrt(1/2)*sqrt(-BL1+2+sqrt(9*BL1^2-8*BL1))
wz = sqrt(BL1)
k = 2*wxy/(wxy^2+2*BL1+1)

%%
TU = sqrt(AU^3/(GSun+GMoon+GEarth));
VU = AU/TU;

%%
% s = dsolve('D2x-2*Dy-(2*BL+1)*x=0','D2y+2*Dx+(BL-1)*y=0')
% lamda = solve('x^4 - (BL-2)*x^2-(2*BL+1)*(BL-1)')
% s = dsolve('D2x-2*Dy-9.1221479*x=0','D2y+2*Dx+3.06107395*y=0','x(0)=x0','y(0)=y0')
% x = C1*exp(2.064i*t)+C2*exp(-2.064i*t)+C3*exp(2.496*t)+C4*exp(-2.496*t)
% y = 3.2i*C1*exp(2.064i*t)-3.2i*C2*exp(-2.064i*t)-0.54259*C3*exp(2.496*t)+0.54259*C4*exp(-2.496*t)

% solve('x^5 - (3-3.04042351784876e-006)*x^4 + (3-2*3.04042351784876e-006)*x^3 - 3.04042351784876e-006*x^2 + 2*3.04042351784876e-006*x - 3.04042351784876e-006=0')
*/