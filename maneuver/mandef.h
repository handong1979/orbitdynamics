
#pragma once

#include <OrbitDyn.h>

typedef struct{
	CDateTime epoch;
	Kepler kp;
	double AirDragArea;
	double Mass;
}OrbitParam;

CDateTime string2epoch(string s);

OrbitParam LoadOrbitFile(string filename);

void InitSat(CSatellite& sat, string filename);

double sma_nearcircle(double a, double deltaa);
double sma_ellipse(double a, double e, double f, double deltaa);
double leo_inclination(double a, double i0, double it, int ascordes);
vec3 leo_Binc(double a, double i0, double it, int ascordes);

