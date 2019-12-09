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
	Kepler kp(25000, 0.3, 48.4 * RAD, 120 * RAD, 50 * RAD, 0);
	CSatellite sat;
	sat.Name = "ca";
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
	for (int i = 0; i < duration; i++)
	{
		mat33 coi = GetCoi(sat.GetOrbitElements());

		//飞行路径角, 即U方向与T方向的夹角
		double beta = atan2(sat.e*sin(sat.f), 1.0 + sat.e * cos(sat.f));
		mat33 cui = RotationY(beta)*coi;
		sat.qbi = cui;

#if 0
		// 时间最优控制
		if (sat.a > 25500)
		{
			if (sat.ThrustIsOn == true)
				cout << "关机时间:" << sat.CurrentEpoch() << endl;
			sat.ThrustIsOn = false;
		}
#else
		// 燃料最优控制
		double percent = 0.7;
		if (sat.f < percent*PI || sat.f > (2.0-percent)*PI)
		{
			if (!finish && sat.a > 25500)
			{
				finish = true;
				cout << "关机时间:" << sat.CurrentEpoch() << endl;
			}
			if ( !finish )
				sat.ThrustIsOn = true;
		}
		else
		{
			sat.ThrustIsOn = false;
		}
#endif

		sat.Propagate(step, step);
	}

	cout <<  "dm = " << sat.BurnedFuel << endl;

}
