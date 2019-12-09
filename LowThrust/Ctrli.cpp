// Ctrla.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>

#include <string>
using namespace std;

#include <orbitdyn.h>
using namespace Constant;

int main()
{
	CDateTime epoch(2019, 1, 1, 0, 0, 0);
	Kepler kp(25000, 0.3, 48.5 * RAD, 120 * RAD, 50 * RAD, 0);
	CSatellite sat;
	sat.Name = "ci";
	sat.Initialize(epoch, kp);
	sat.SetEngine(1000, 0.01);
	sat.AirDragArea = 0.0289;
	sat.Mass0 = 100;
	sat.ThrustDirection = "1,0,0";
	sat.SetAutoSave();

	double step = 60;
	int duration = 86400 * 30 / (int)step;

	sat.ThrustIsOn = true;
	bool finish = false;
	mat33 cbi;

	for (int i = 0; i < duration; i++)
	{
		mat33 coi = GetCoi(sat.GetOrbitElements());

		//飞行路径角, 即U方向与T方向的夹角
		double beta = atan2(sat.e*sin(sat.f), 1.0 + sat.e * cos(sat.f));
		mat33 cui = RotationY(beta)*coi;
#if 0
		// 时间最优控制
		if (sat.u > 1.5*PI || sat.u < 0.5*PI)
		{
			cbi = RotationZ(PI / 2)*cui;
		}
		else
		{
			cbi = RotationZ(-PI / 2)*cui;
		}
		if (sat.i < 48 * RAD && !finish)
		{
			finish = true;
			sat.ThrustIsOn = false;
			cout << "关机时间:" << sat.CurrentEpoch() << endl;
		}
#else
		// 燃料最优控制
		double percent = 0.4; // 0~0.5
		if (!finish)
		{
			if (sat.u > (2 - percent)*PI || sat.u < percent*PI) 
			{
				sat.ThrustIsOn = true;
				cbi = RotationZ(PI / 2)*cui;
			}
			else if (sat.u > (1 - percent)*PI && sat.u < (1 + percent)*PI)
			{
				sat.ThrustIsOn = true;
				cbi = RotationZ(-PI / 2)*cui;
			}
			else {
				sat.ThrustIsOn = false;
			}
			if (sat.i < 48 * RAD)
			{
				finish = true;
				sat.ThrustIsOn = false;
				cout << "关机时间:" << sat.CurrentEpoch() << endl;
			}
		}
		else
			sat.ThrustIsOn = false;
#endif
		sat.qbi = cbi;

		sat.Propagate(step, step);
	}

	cout << "dm = " << sat.BurnedFuel << endl;

}
