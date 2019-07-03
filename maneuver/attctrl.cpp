// semi-major axis maneuver
#include "mandef.h"
using namespace Constant;

string orbitfilename, facfilename, attfilename;
 
double GLon, GLat, GAlt, Elmin = 5.0;
CFacility fac;

void loadfac(string filename)
{
	fstream file(filename, ios::in);
	if (!file.is_open())
		throw (string("Can't open file") + filename);
	std::string name, value;
	while (!file.eof()) {
		//# 地面站位置信息
		//GLon = 120
		//GLat = 40
		//GAlt = 0.2
		if (ReadLine(&file, name, value)) {
			if (name == "GLon")
				sscanf(value.c_str(), "%lf", &GLon);
			else if (name == "GLat")
				sscanf(value.c_str(), "%lf", &GLat);
			else if (name == "GAlt")
				sscanf(value.c_str(), "%lf", &GAlt);
			else if (name == "Elmin")
				sscanf(value.c_str(), "%lf", &Elmin);
		}
	}
	fac.SetGeodetic(GLon, GLat, GAlt);
}

// 计算卫星指向地面点时的姿态角，并且判断可见性
void attctrl()
{
	CSatellite sat;
	InitSat(sat, orbitfilename);

	fstream fa(attfilename.c_str(), ios::out);
	vec3 satr,facr,sf,sfo,sfou;
	mat33 coi;
	double phi, theta;
	bool see = false;
	int nsee = 1;
	for (int i = 0; i <= 86400; i++)
	{
		satr = sat.Pos();
		facr = fac.ECIPos(sat.CurrentEpoch());
		sf = facr - satr;
		coi = GetCoi(sat.GetOrbitElements());
		sfo = coi * sf;
		sfou = sfo / norm(sfo, 2);
		
		// 判断可见性
		vec3 u = fac.ECFPosition;
		vec3 R = ECI_ECF(sat.CurrentEpoch())*sat.Pos();
		mat33 C = VehicleCoord(fac.Longitude, fac.Latitude);
		vec3 s = C * (R - u);
		double rou = norm(s, 2);
		double elevation = asin(s(2) / rou)*DEG;

		if (elevation > Elmin)
		{
			if (see == false)
			{
				cout << nsee <<". 可见开始时间: " << sat.CurrentEpoch() + 8*3600.0;
				see = true;
			}
			// 1-2转序旋转的求逆
			//[cos(theta), sin(phi)*sin(theta), -cos(phi)*sin(theta)]
			//[0, cos(phi), sin(phi)]
			//[sin(theta), -cos(theta)*sin(phi), cos(phi)*cos(theta)]
			phi = atan2(-sfou(1), sfou(2));
			theta = asin(sfou(0));
			fa << sat.CurrentEpoch() + 8 * 3600.0 <<
				TAB << phi * DEG << TAB << theta * DEG << //endl;
			TAB << sfou(0) << TAB << sfou(1) << TAB << sfou(2) << TAB << elevation << endl;
		}
		else
		{
			if (see == true) {
				cout << "  结束时间: " << sat.CurrentEpoch() + 8 * 3600.0 << endl;
				see = false;
				nsee++;
			}
		}

		sat.Propagate(1, 1);
	}
	fa.close();
}

int main(int argc, char* argv[])
{
	if (argc<4)
	{
		printf("错误：未指定输入文件!!\n\n");
		printf("请使用以下指令调用：\n");
		printf("  attctrl orbit.txt fac.txt attconangle.txt\n");
		printf("  第一个文件为轨道参数文件\n");
		printf("  第二个文件为指向的地面点文件\n");
		printf("  第三个文件为输出姿态文件\n");
		return 0;
	}
	
	try {
		orbitfilename = string(argv[1]);
		facfilename = string(argv[2]);
		attfilename = string(argv[3]);

		loadfac(facfilename);
		attctrl();
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
	
	return 1;
}
