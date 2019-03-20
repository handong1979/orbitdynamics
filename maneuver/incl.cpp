// semi-major axis maneuver
#include <OrbitDyn.h>
//#include <PerfTimer.h>

using namespace Constant;



int main(int argc, char* argv[])
{
	double dv;

	dv = leo_inclination(7000, 98.2, 98.22,1);

	vec3 dvb = leo_Binc(7000, 98.2, 98.22, 0);

	return 0;
}
