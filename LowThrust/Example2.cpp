// Example2.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
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
	Kepler kp(6895.0, 0.001, 98.4 * RAD, 120 * RAD, 50 * RAD, 0);
	CSatellite sat;
	sat.Initialize(epoch, kp);
	sat.SetEngine(200, 0.0003);
	sat.AirDragArea = 0.0289;
	sat.Mass0 = 10;
	sat.ThrustDirection = "1,0,0";
	sat.SetAutoSave();

	CSatellite tmp; // 用于寻找点火中间点时刻
	tmp.Initialize(epoch, kp);
	tmp.AirDragArea = 0.0289;
	tmp.Mass0 = 10;

	CDateTime ratime;

	int firemin = 45;  //每圈点火时长(分钟)
	int revs = 74;     //点火总圈数，可按总冲、总点火时间、每圈点火时间计算
	fstream fm("amean.txt", ios::out);
	for (int i = 0; i < revs; i++)
	{
		tmp.Propagate2AscendingNode(); // 以升交点为变轨弧段的中点
		ratime = tmp.CurrentEpoch();   // 升交点时刻
		sat.Propagate2Epoch(ratime- firemin*60.0/2.0); // 外推到点火点
		fm << sat.t() << "\t" << sat.MedianElement() << endl; // save mean elements
		cout << i << endl;
		cout << "ThrustOn:" << sat.CurrentEpoch() << " " << sat.GetOrbitElements() << endl;
		sat.ThrustIsOn = true;
		// 点火过程
		for (int ft = 0; ft < firemin; ft++)
		{
			sat.qbi = sat.Coi;
			sat.Propagate(60, 60);
		}
		sat.ThrustIsOn = false;
		cout << "ThrustOFF:" << sat.CurrentEpoch() << " " << sat.GetOrbitElements() << endl;
		sat.Propagate(60, 2400); // 外推一小段，保证不到下次点火点即可
		tmp.Initialize(sat.CurrentEpoch(), sat.GetOrbitElements());
	}
	
}