#include <cmath>
#include "..\orbitdyn\armadillo.h"
#include "..\orbitdyn\Quaternion.h"
#include "..\orbitdyn\Euler.h"

mat Ew(vec3 w)
{
	mat Omega(4, 3);
	Omega(0, 0) = 0;
	Omega(0, 1) = w(2);
	Omega(0, 2) = -w(1);
	Omega(0, 3) = w(0);

	Omega(1, 0) = -w(2);
	Omega(1, 1) = 0;
	Omega(1, 2) = w(0);
	Omega(1, 3) = w(1);

	Omega(2, 0) = w(1);
	Omega(2, 1) = -w(0);
	Omega(2, 2) = 0;
	Omega(2, 3) = w(2);

	Omega(3, 0) = -w(0);
	Omega(3, 1) = -w(1);
	Omega(3, 2) = -w(2);
	Omega(3, 3) = 0;

	return Omega;
}

mat Eq(CQuaternion q)
{
	mat e(4, 3);
	e(0, 0) =  q.qs;  e(0, 1) = -q.qz;  e(0, 2) =  q.qy;
	e(1, 0) =  q.qz;  e(1, 1) =  q.qs;  e(1, 2) = -q.qx;
	e(2, 0) = -q.qy;  e(2, 1) =  q.qx;  e(2, 2) =  q.qs;
	e(3, 0) = -q.qx;  e(3, 1) = -q.qy;  e(3, 2) = -q.qz;
	return e;
}

vec7 rigid(double t, vec7 x,mat33 Is, vec3 Torque)
{
	vec3 w = x.subvec(4, 6);
	mat Omega = Ew(w);
	vec4 dq = 0.5*Omega*x.subvec(0, 3);
	vec dw = solve(Is, Torque + Omega.submat(0, 0, 2, 2)*(Is*w))
	return join_vert(dq, dw);
}

double Gill4(vec7 dynfuc(double,vec7,mat33,vec3), 
	double h, vec& x,mat33 Is,vec3 Torque, vec& xh)
{
	static const double s2 = sqrt(2.0);
	static vec x1;
	static vec y[4];
	for (int ii = 0;ii<4;ii++) y[ii].copy_size(x);

	double t = 0; //动力学方程中不含t
	x1 = x;
	y[0] = dynfuc(t, x1, Is, Torque);

	x1 = x + h / 2 * y[0];
	y[1] = dynfuc(t, x1, Is, Torque);

	x1 = x + h * ((s2 - 1) / 2 * y[0] + (1 - s2 / 2)*y[1]);
	y[2] = dynfuc(t, x1, Is, Torque);

	x1 = x + h * (-s2 / 2 * y[1] + (1 + s2 / 2)*y[2]);
	y[3] = dynfuc(t, x1, Is, Torque);

	xh = x + h / 6 * (y[0] + (2 - s2)*y[1] + (2 + s2)*y[2] + y[3]);

	return 0;
}