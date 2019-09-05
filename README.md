# orbitdynamics
a C++ program for orbit dynamics, include compments:

  orbit propagator
  
  formation
  
  maneuver
  
  etc
  
* RKF78 propagator
* gravity field file supported
* DE405/DE421 for planet's ephemeris
* MSISE-00 atmosphere model
* virtual base class and easy to extend
* use armadillo library as vector and matrix operation
* use hdf5 data file

C++轨道动力学计算程序
卫星、航天器轨道动力学的精密计算，包括主要特征包括：

* RKF78高精度积分器
* 支持地球引力场系数文件
* DE405/DE421行星历表
* MSISE-00大气模型
* 采用合适的C++面向对象结构，方面扩展
* 使用armadillo进行矢量、矩阵运算
* 可保持HDF5数据文件

   轨道动力学计算中，需要涉及惯性坐标系、地固坐标系、轨道坐标系等左边转换功能，其中较为复杂的地固系与惯性系的转换需要计算岁差、章动、极移以及在计算中使用UTC、原子时、动力学时等多种时间系统。为此，轨道动力学计算需要建立在天文参数计算、坐标转换、时间系统等基础功能之上，从基础进行独立开发的难度较大。
   
  为了精确计算轨道动力学，需要考虑地球非球形引力、日月引力、大气阻力、光压等多种摄动，其中需要地球引力场模型、日月星历模型、地球大气模型等多种模型，用于摄动力模型计算。

Example:
   // a satellite
   
   CSatellite sat;
   
   // orbit epoch and elements
   
	CDateTime epoch(2006,5,1,0,0,0);
	
	Kepler ZY_Orbit(7157,			0.001,		98.5*RAD,	60*RAD,			90*RAD,		60*RAD);
	
   // set satellite's param
   
	sat.Mass0 = 1000;  // use default value
	
	sat.Cr = 1.0;
	
	sat.LightPressArea = 20.0;
	
	sat.Cd = 2.2;
	
	sat.AirDragArea = 20.0;
	
   // initialize CSatellite
   
	sat.Initialize(epoch,ZY_Orbit);
	
	// set force model
	
	sat.SetForce(21,ODP_EARTH_ZONAL|ODP_EARTH_TESSERAL|ODP_AIR_DRAG|ODP_SOLAR_CENT|ODP_SOLAR_PRESSURE|ODP_LUNAR_CENT|ODP_POSTNEWTON);
	
	// propagate the orbit and print position and velocity
	
	double t;
	
	for(t=0; t<86400; t+=60)
	{
		cout << t << TAB << sat.Pos().t() << TAB << sat.Vel().t() << endl;
		
		sat.Propagate(60,60);
		
	}