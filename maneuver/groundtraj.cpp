#include <OrbitDyn.h>
#include "mandef.h"

using namespace Constant;

string outfilename, lanfilename, orbitfilename;
int RepeatDays = 0, RepeatRevs = 0;
double FirstLon, LonErr, i0;
vector<double> lonlist;

// 节点周期
double Tnode(double a,double i)
{
	return PI2/sqrt(GE/a/a/a)*(1-1.5*J2*(Re/a)*(Re/a)*(3.0-2.5*sin(i)*sin(i)));
}

// 升交点赤经变化率
double dotOmega(double a,double i)
{
	return -1.5*sqrt(GE/a/a/a)*J2*(Re/a)*(Re/a)*cos(i);
}

// 回归轨道满足的方程，返回方程的值，当方程的值为0时，输入a和i构成回归轨道
double repeatfun(double a,double i,int rev,int day)
{
	double n = sqrt(GE/a/a/a);
	double TN = Tnode(a,i);
	double dOmega = dotOmega(a,i);
	return rev*TN*(We - dOmega) - day*PI2;
}

// fit x-y as a palabolic:
// y = a + b*x + c*x^2
void parabolafit(double* x, double* y, int n, double& a, double& b, double& c)
{
	double matrix[3][4], ratio, aa;
	double sum_x = 0, sum_y = 0, sum_x2 = 0, sum_x3 = 0, sum_x4 = 0, sum_xy = 0, sum_x2y = 0;
	int i, j, k;
	for (i = 0; i < n; i++) {
		sum_x += x[i];
		sum_y += y[i];
		sum_x2 += pow(x[i], 2);
		sum_x3 += pow(x[i], 3);
		sum_x4 += pow(x[i], 4);
		sum_xy += x[i] * y[i];
		sum_x2y += pow(x[i], 2) * y[i];
	}
	matrix[0][0] = n;
	matrix[0][1] = sum_x;
	matrix[0][2] = sum_x2;
	matrix[0][3] = sum_y;
	matrix[1][0] = sum_x;
	matrix[1][1] = sum_x2;
	matrix[1][2] = sum_x3;
	matrix[1][3] = sum_xy;
	matrix[2][0] = sum_x2;
	matrix[2][1] = sum_x3;
	matrix[2][2] = sum_x4;
	matrix[2][3] = sum_x2y;
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			if (i != j) {
				ratio = matrix[j][i] / matrix[i][i];
				for (k = 0; k < 4; k++) {
					matrix[j][k] -= ratio * matrix[i][k];
				}
			}
		}
	}
	for (i = 0; i < 3; i++) {
		aa = matrix[i][i];
		for (j = 0; j < 4; j++) {
			matrix[i][j] /= aa;
		}
	}
	a = matrix[0][3];
	b = matrix[1][3];
	c = matrix[2][3];
}

//! 地面轨迹维持
void groundtraj()
{
	CSatellite sat;
	InitSat(sat,orbitfilename);
	cout << "sat's Mean:" << sat.MedianElement() << endl;

	CSpherical lla;
	FILE* flla;
	flla = fopen("groundtraj.txt", "w");
	// 节点周期
	double w = PI2 / (RepeatDays * 86164.09 / RepeatRevs);
	double a0 = pow(GE/w/w,1.0/3.0);
	double a;
	for (int kk = 0; kk < 10; kk++)
	{
		// 数值微分求导数
		double f1 = repeatfun(a0,i0*RAD,RepeatRevs,RepeatDays);
		double f2 = repeatfun(a0+0.001,i0*RAD,RepeatRevs,RepeatDays);
		double df = (f2-f1)/0.001;

		a = a0 - f1 / df;
		if(fabs(a-a0)<0.001)
			break;
		a0 = a;
	}
	//a0 = a0 + 12.721;
	//a0 = 6861.72566;
	cout << "Guess a0 = " << a0 << endl;
	
	// 外推，拟合出经度偏差漂移率和漂移加速度
	const int nrev = 100;
	double dlarray[nrev] = { 0 }, time[nrev] = { 0 }, da[nrev] = { 0 };
	CDateTime ep0 = sat.CurrentEpoch();
	for (int i = 0;i < nrev;i++)
	{
		sat.Propagate2EquatorAscNode();
		lla = sat.GetLLA();
		
		double dl = lla.Longitude - lonlist[i%RepeatRevs];
		if (dl > 350) dl -= 360;
		else if (dl < -350) dl += 360;
		dl = dl * RAD * 6378;

		dlarray[i] = dl;
		time[i] = sat.CurrentEpoch() - ep0;
		Kepler cm = Mean(sat.GetOrbitElements());
		da[i] = cm.a - a0;

		cout << "Lon: " << lla.Longitude 
			//<< " Lat: " << lla.Latitude
			<< "\tLon err:" << dl << "km"
			<< "\t da :" << da[i] << "km" << endl;
		fprintf(flla,"%e\t%e\t%e\n",time[i], dlarray[i],da[i]);
	}
	fclose(flla);

	// 拟合
	double L0, dotL, AccL;
	parabolafit(da, dlarray, nrev, L0, dotL, AccL);
	// x = a + b * (y + c) ^ 2 = b*y^2 + 2*b*c*y + b*c^2 + a
	//                         = Accl*da^2 + dotL*da + L0
	// 顶点(y=-c,x=a)

	// find c:
	double modifya = dotL / 2 / AccL;
	printf("a0 = %lf\n", a0);
	a0 = a0 - modifya;
	if(1)
	{
		printf("L0 = %lf\n", L0 );  // init lon error
		printf("dL = %lf\n", dotL );  // lon error rate
		printf("ddL = %lf\n", AccL ); // lon error accelerate
		printf("a0 = %lf\n", a0);
		printf("modifya = %lf\n", modifya);
	}
//////////////////////////////////////////////////////////////////////////
	//InitSat(sat,orbitfilename);
	//flla = fopen("groundtraj.txt", "w");
	//for (int i = 0;i < nrev;i++)
	//{
	//	sat.Propagate2EquatorAscNode();
	//	lla = sat.GetLLA();

	//	double dl = lla.Longitude - lonlist[i%RepeatRevs];
	//	if (dl > 350) dl -= 360;
	//	else if (dl < -350) dl += 360;
	//	dl = dl * RAD * 6378;

	//	dlarray[i] = dl;
	//	time[i] = sat.CurrentEpoch() - ep0;
	//	Kepler cm = Mean(sat.GetOrbitElements());
	//	da[i] = cm.a - a0;

	//	cout << "Lon: " << lla.Longitude 
	//		//<< " Lat: " << lla.Latitude
	//		<< "\tLon err:" << dl << "km" 
	//		<< "\t da :" << da[i] << "km" << endl;
	//	fprintf(flla,"%e\t%e\t%e\n",time[i], dlarray[i],da[i]);
	//}
	//fclose(flla);

//	return;
	// 预报和控制计算
	InitSat(sat, orbitfilename);
	double dlr = 0;	
	int i = 0,controlrev = 0;
	Kepler cm;
	//while (fabs(dlr) * 2 < LonErr){
	//for (i = 0; i < 50; i++){
	while(1){
		sat.Propagate2EquatorAscNode();
		lla = sat.GetLLA();

		dlr = lla.Longitude - lonlist[i%RepeatRevs];
		if (dlr > 350) dlr -= 360;
		else if (dlr < -350) dlr += 360;
		dlr = dlr * RAD * 6378;

		cm = Mean(sat.GetOrbitElements());
		double da = cm.a - a0;
		
		cout << "Rev " << i++ << "  Lon err:" << dlr << "km" << "\t da: " << da << "km" << endl;
		// control
		/*if (dlr < -LonErr)
		{
			// 超出西边界			
			Kepler cm = Mean(sat.GetOrbitElements());
			double abias = 0;
			double da = a0 + abias - cm.a;
			vec3 dv = "0.0,0.0,0.0";
			dv(0) = sqrt(GE / cm.a / cm.a / cm.a) / 2 * da;
			sat.ImpluseManeuver(dv);
			cout << "超出西边界,开始计算轨道维持速度增量" << endl;			

			break;
		}
		else*/ if (dlr > LonErr && i - controlrev > 3)
		{
			controlrev = i;
			// 超出东边界			
			cout << "超出东边界,开始计算轨道维持速度增量" << endl;			
			break;
		}
	}
	double maxdlr = dlr;
	CDateTime tdv = sat.CurrentEpoch();
	Kepler kpdv = sat.GetOrbitElements();

	double abias = 1.4;
	double deltaa = a0 + abias - cm.a;
	vec3 dv = "0.0,0.0,0.0";
	dv(0) = sqrt(GE / cm.a / cm.a / cm.a) / 2 * deltaa;
	sat.ImpluseManeuver(dv);  cout << "dv = " << dv(0) << endl;
	// 查找西边最小点
	double mindlr = maxdlr;
	int cntIncrease = 0;
	i = controlrev;
	while(cntIncrease<10){
		sat.Propagate2EquatorAscNode();
		lla = sat.GetLLA();

		dlr = lla.Longitude - lonlist[i%RepeatRevs];
		if (dlr > 350) dlr -= 360;
		else if (dlr < -350) dlr += 360;
		dlr = dlr * RAD * 6378;

		cm = Mean(sat.GetOrbitElements());
		double da = cm.a - a0;

		cout << "Rev " << i++ << "\tmindlr:" << mindlr << "\t Lon err:" 
			<< dlr << "km" << "\t da: " << da << "km" << endl;

		if(dlr < mindlr)
			mindlr = dlr;
		else
			cntIncrease++;
	}
	// 西边最小点与西边界的差
	double dddlr1 = mindlr - ( -LonErr );
	
	// 增加速度增量，求微分
	sat.Initialize(tdv,kpdv);
	sat.ImpluseManeuver(dv*0.95);  cout << "dv = " << dv(0)*0.95 << endl;
	// 查找西边最小点
	mindlr = maxdlr;
	cntIncrease = 0;
	i = controlrev;
	while(cntIncrease<10){
		sat.Propagate2EquatorAscNode();
		lla = sat.GetLLA();

		dlr = lla.Longitude - lonlist[i%RepeatRevs];
		if (dlr > 350) dlr -= 360;
		else if (dlr < -350) dlr += 360;
		dlr = dlr * RAD * 6378;

		cm = Mean(sat.GetOrbitElements());
		double da = cm.a - a0;

		cout << "Rev " << i++ << "\tmindlr:" << mindlr << "\t Lon err:" 
			<< dlr << "km" << "\t da: " << da << "km" << endl;

		if(dlr < mindlr)
			mindlr = dlr;
		else
			cntIncrease++;
	}
	// 西边最小点与西边界的差
	double dddlr2 = mindlr - ( -LonErr );
	dv(0) = dv(0) - dddlr1/((dddlr2 - dddlr1)/(-dv(0)*0.05));

	cout << dddlr1 << TAB << dddlr2 << endl;
	
	sat.Initialize(tdv,kpdv);
	sat.ImpluseManeuver(dv*0.98);  cout << "dv = " << dv(0)*0.98 << endl;
	// 查找西边最小点
	mindlr = maxdlr;
	cntIncrease = 0;
	i = controlrev;
	while(cntIncrease<10){
		sat.Propagate2EquatorAscNode();
		lla = sat.GetLLA();

		dlr = lla.Longitude - lonlist[i%RepeatRevs];
		if (dlr > 350) dlr -= 360;
		else if (dlr < -350) dlr += 360;
		dlr = dlr * RAD * 6378;

		cm = Mean(sat.GetOrbitElements());
		double da = cm.a - a0;

		cout << "Rev " << i++ << "\tmindlr:" << mindlr << "\t Lon err:" 
			<< dlr << "km" << "\t da: " << da << "km" << endl;

		if(dlr < mindlr)
			mindlr = dlr;
		else
			cntIncrease++;
	}
	// 西边最小点与西边界的差
	double dddlr3 = mindlr - ( -LonErr );

	cout << dddlr1 <<TAB<< dddlr2 <<TAB<< dddlr3 << endl;

	fstream fo(outfilename, ios::out);
	fo << "轨道维持控制时间 = " << tdv << endl;
	fo << "半长轴控制速度增量 = " << dv(0)*0.98 * 1000 << endl;

	fo.close();
}

void RepeatOrbit(int days, int revs, double lan0)
{
	double time = days * 86164.09;
	double Tn = time / revs;
	double dlan = Tn * We * DEG;
	FILE *fp;
	fp = fopen("rolan.txt", "w");
	fprintf(fp, "%3d   %11.6lf\n",0, lan0);
	lonlist.clear();
	double lan = lan0;
	lonlist.push_back(lan);
	for (int i = 1;i < revs;i++)
	{
		lan -= dlan;
		if (lan < 0)
			lan += 360;
		lonlist.push_back(lan);
		fprintf(fp, "%3d   %11.6lf\n",i, lan);
	}
	fclose(fp);
}

//void RepeatOrbit(int days, int revs, double iclination)
//{
//	double time = days * 86164.09;
//	double Tn = time / revs;
//	// node period
//	// Tn = 2*pi/n*(1 + 1.5*J2*(Re/a)^2*(1-4*cos(i)*cos(i)))
//	// So:
//	// (Re/a)^2 = (Tn*n/2/pi - 1) / 1.5 /J2 / (1-4*cos(i)*cos(i))
//	// And:
//	// a = Re / sqrt( (Tn*n/2/pi - 1) / 1.5 /J2 / (1-4*cos(i)*cos(i)) )
//	double n = PI2 / (Tn - 10);
//	double lasta = 0;
//	double a = pow(GE/n/n,1.0/3.0);
//	double Tntmp = 0;
//	for (int i = 0;i < 35;i++) {
//		n = sqrt(GE / a / a / a);
//		Tntmp = PI2 / n * (1.0 + 1.5*J2*(Re / a)*(Re / a) * (1 - 4 * cos(i)*cos(i)));
//		n = PI2 / (PI2 / n - 0.1*(Tntmp - Tn));
//		a = pow(GE / n / n, 1.0 / 3.0);
//		printf("lasta = %lf  a = %lf\n", lasta, a);
//		lasta = a;
//	}
//
//}

void loadlon(string lanfilename) {
	fstream flan(lanfilename, ios::in);
	double lon;
	string name, value;
	while (!flan.eof()) {
		//# 回归周期的天数
		//RepeatDays = 10
		//# 回归周期的圈数
		//RepeatRevs = 141
		//# 第一圈的升交点地理经度
		//FirstLon = 12.3
		//# 升交点地理经度允许的最大偏差(km)
		//LonErr = 10
		if (ReadLine(&flan, name, value)) {
			if (name == "RepeatDays")
				sscanf(value.c_str(), "%d", &RepeatDays);
			else if (name == "RepeatRevs")
				sscanf(value.c_str(), "%d", &RepeatRevs);
			else if (name == "FirstLon")
				sscanf(value.c_str(), "%lf", &FirstLon);
			else if (name == "LonErr")
				sscanf(value.c_str(), "%lf", &LonErr);
			else if (name == "i0")
				sscanf(value.c_str(), "%lf", &i0);
		}
	}
	if (RepeatDays != 0 && RepeatRevs != 0)
	{
		RepeatOrbit(RepeatDays, RepeatRevs, FirstLon);
	}
}

int main(int argc, char* argv[])
{
	if (argc < 4)
	{
		if (argc == 2)
		{
			loadlon(argv[1]);
			return 1;
		}

		printf("错误：未指定输入文件!!\n\n");
		printf("请使用以下指令调用：\n");
		printf("  groundtraj orbitleotraj.txt gtrc.txt gtout.txt\n");
		printf("  第一个文件为轨道参数文件\n");
		printf("  第二个文件为轨道维持精度设置文件\n");
		printf("  第三个文件为轨道维持控制输出文件\n");
		printf(" 用法2： 生成一个回归轨道的标称升交点地理经度表\n");
		printf(" groundtraj gtrc.txt\n");
		printf(" 根据文件中的回归轨道参数，生成一个rolan.txt文件，保存各个圈次的标称升交点地理经度\n");
		return 0;
	}

	try {
		orbitfilename = string(argv[1]);
		lanfilename = string(argv[2]);
		outfilename = string(argv[3]);

		loadlon(lanfilename);
		groundtraj();
	}
	catch (BaseException& e)
	{
		cerr << e.GetFullMessage() << endl;
	}
	catch (exception* e)
	{
		cerr << ((BaseException*)e)->what() << endl;
	}
	catch (string * e)
	{
		cerr << *e << endl;
	}



	
	
	
	return 0;
}