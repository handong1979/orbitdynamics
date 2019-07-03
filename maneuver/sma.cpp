// semi-major axis maneuver
#include <OrbitDyn.h>

using namespace Constant;




int main(int argc, char* argv[])
{
	double dv;

	dv = sma_nearcircle(7000, 10);
	printf("dv = sma_nearcircle(7000, 10) = %f\n",dv);

	dv = sma_ellipse(7000, 0.2, 0, 10);
	printf("dv = sma_ellipse(7000, 0.2, 0, 10) = %f\n",dv);

	return 0;
}
