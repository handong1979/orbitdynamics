#pragma once

#include <cmath>
#include "..\orbitdyn\armadillo.h"
#include "..\orbitdyn\Quaternion.h"
#include "..\orbitdyn\Euler.h"

class AttDyn
{
public:
	AttDyn();
	~AttDyn();

	vec7 rigid(double t, vec7 x);
	double rigidstep(double h);

	CQuaternion q;
	vec3 w;
	mat33 Is;
	vec3 Torque;
	
	vec3 bc;	   //卫星质心位置,m
	
	int NThrustor; //电推力器个数
	vec3* ThrPos;  //电推力器安装位置,m
	vec3* ThrDir;  //电推力器推力方向
	void SetThrustNum(int n) {
		NThrustor = n;
		ThrPos = new vec3[n];
		ThrDir = new vec3[n];
	}
	vec3 TThrust; // 推力器合成力矩
	void Thrust(int * Open, double * FThr);
};

