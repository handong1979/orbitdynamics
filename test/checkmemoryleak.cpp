/*!
\file test.cpp
\author HanDle
*/
#include <OrbitDyn.h>
using namespace Constant;

void checkmemoryleak()
{
	// check memory leak
	vector<CSatellite*> vs;
	vector<CSatellite*>::iterator itvsat;
	while(1){
		CSatellite *sat = new CSatellite;
		vs.push_back(sat);

		vs[0]->Initialize(CDateTime(2015,1,1,0,0,0),Kepler(7044,0.001,98.5*RAD,0,0,0));
		vs[0]->Propagate(60,300);
		vs[0]->GetOrbitElements();

		delete vs[0];
		itvsat = vs.begin();
		vs.erase(itvsat);
	}
}

int main(int argc, char* argv[])
{
	try
	{
		cout.precision(12);

		checkmemoryleak();
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
