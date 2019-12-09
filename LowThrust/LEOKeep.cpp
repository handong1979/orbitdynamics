// LEOKeep.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>

#include <string>
using namespace std;

#include <orbitdyn.h>
using namespace Constant;

int main()
{
	CDateTime epoch(2019, 1, 1, 0, 0, 0); // 初始时间
	Kepler kp(6678.0, 0.001, 98.4 * RAD, 120 * RAD, 50 * RAD, 0); // 初始轨道参数
	CSatellite sat;  // 声明轨道计算类对象
	sat.Initialize(epoch, kp); // 卫星轨道初始化
	sat.SetEngine(200, 0.0003); // 设置发动机比冲和推力
	sat.AirDragArea = 0.0289;  // 设置卫星大气阻力面积
	sat.Mass0 = 10;           // 设置卫星初始质量
	sat.ThrustDirection = "1,0,0";  // 设置推力器安装方向
	sat.SetAutoSave();     // 设置自动保存数据文件

	double step = 60;   // 仿真步长
	int duration = 86400 * 5 / 60;  // 仿真时间
	Kepler km = sat.MedianElement(); // 初始轨道平根数
	double am0 = km.a;  // 初始平半长轴
	fstream fm("amean.txt", ios::out);  // 保存平根数的文件
	for (int i = 0; i < duration; i++)
	{
		km = sat.MedianElement();  // 计算平根数
		if (km.a - am0 < -0.5)     // 轨道平半长轴降低-0.5km时开启推力器
			sat.ThrustIsOn = true;
		else if (km.a - am0 > 0.5)  // 轨道平半长轴升高到+0.5km时关闭推力器
			sat.ThrustIsOn = false;

		fm << sat.t() << "\t" << sat.MedianElement() << endl; // 保存平根数
		
		cout << i << "\t" << km.a - am0 << endl; // 屏幕输出
		sat.qbi = GetCoi(sat.GetOrbitElements()); // 设置卫星姿态
		sat.Propagate(step, step);   // 轨道外推一步
	}
}