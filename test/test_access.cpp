/*!
\file test.cpp
\author HanDle
*/
#include <OrbitDyn.h>
using namespace Constant;

bool facaccess(CFacility * fac,CSatellite* sat)
{
	vec3 p = sat->Pos();
	mat33 cfi = ECI_ECF(sat->CurrentEpoch());
	vec3 pf = cfi * p;

	vec3 pfac = fac->ECFPosition;
	mat33 C = VehicleCoord(fac->Longitude, fac->Latitude);
	vec3 s = C * (pf - pfac);

	double rou = norm(s,2);
	double azimuth = atan2(s(1), s(0))*DEG;
	double elevation = asin(s(2) / rou)*DEG;
	return elevation > 5 ? true : false;
}

void test_access()
{
	CDateTime t(2019,1,1,0,0,0);
	Kepler k(7157.7833, 0.001215,	  98.5160*RAD,	  202.2820*RAD,  143.8963*RAD,    122.135*RAD);
	CSatellite a;
	a.Initialize(t,k);
	
	CFacility fac(120,40,0.5);
	bool acc = false, acclast = false;
	int rev = 0;
	for (int i = 0;i < 8640;i++)
	{
		acc = facaccess(&fac, &a);
		if (acclast == false && acc == true)
		{
			rev++;
			cout << "access " << rev << " start " << a.CurrentEpoch() << "   ";
		}
		else if (acclast == true && acc == false)
		{
			cout << " end " << a.CurrentEpoch() << endl;
		}
		a.Propagate(10, 10);
		acclast = acc;
	}
}

int main(int argc, char* argv[])
{
    try
	{
		cout.precision(12);

		test_access();
	}
	catch(BaseException& e)
	{
		cerr << e.GetFullMessage() << endl;
	}
 	catch(exception* e)
 	{
 		cerr << ((BaseException*)e)->what() << endl;
 	}

	return 0;
}
