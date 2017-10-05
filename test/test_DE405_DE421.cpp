/*!
\file test.cpp
\author HanDle
≤‚ ‘≥Ã–Ú
*/
#include <OrbitDyn.h>
#include <PerfTimer.h>

using namespace Constant;

//! ≤‚ ‘Ã´—Ù°¢‘¬«Ú–«¿˙
void test_DE405_DE421()
{
	CDateTime t0(2000,1,1,0,0,0);
	CDateTime tend(2029,1,1,0,0,0);

	fstream eph("DE405test.eph",ios::out);
	eph.precision(8); eph.setf(ios::scientific);

	fstream eph21("DE421test.eph",ios::out);
	eph21.precision(8); eph21.setf(ios::scientific);

	DE405 * de = DE405::Instance();
	DE421 * de421 = DE421::Instance();
	vec3 MoonPos,MoonVel,SunPos,SunVel;
	vec3 VenusPos,VenusVel,MarsPos,MarsVel,NeptunePos,NeptuneVel;
	for(CDateTime t = t0; t<=tend; t+=86400 )
	{
		de->PlanetEphemeris(t,EARTH,MOON,MoonPos,MoonVel);
		de->PlanetEphemeris(t,SUN,EARTH,SunPos,SunVel);
		de->PlanetEphemeris(t,SUN,VENUS,VenusPos,VenusVel);
		de->PlanetEphemeris(t,SUN,MARS,MarsPos,MarsVel);
		de->PlanetEphemeris(t,MARS,NEPTUNE,NeptunePos,NeptuneVel);
		eph<<t.GetUTC()<<TAB<<MoonPos<<TAB<<MoonVel<<TAB<<SunPos<<TAB<<SunVel<<TAB
			<<VenusPos<<TAB<<VenusVel<<TAB<<MarsPos<<TAB<<MarsVel<<TAB<<NeptunePos<<TAB<<NeptuneVel;
		eph<<endl;

		de421->PlanetEphemeris(t,EARTH,MOON,MoonPos,MoonVel);
		de421->PlanetEphemeris(t,SUN,EARTH,SunPos,SunVel);
		de421->PlanetEphemeris(t,SUN,VENUS,VenusPos,VenusVel);
		de421->PlanetEphemeris(t,SUN,MARS,MarsPos,MarsVel);
		de421->PlanetEphemeris(t,MARS,NEPTUNE,NeptunePos,NeptuneVel);
		eph21<<t.GetUTC()<<TAB<<MoonPos<<TAB<<MoonVel<<TAB<<SunPos<<TAB<<SunVel<<TAB
			<<VenusPos<<TAB<<VenusVel<<TAB<<MarsPos<<TAB<<MarsVel<<TAB<<NeptunePos<<TAB<<NeptuneVel;
		eph21<<endl;
	}
	eph.close();
	eph21.close();
}

int main(int argc, char* argv[])
{
	CPerfTimer timer;
	timer.Start();

	try
	{
		cout.precision(12);

		test_DE405_DE421();
	}
	catch(BaseException& e)
	{
		cerr << e.GetFullMessage() << endl;
	}
 	catch(exception* e)
 	{
 		cerr << ((BaseException*)e)->what() << endl;
 	}

	timer.Stop();
	cout<<"Total Timer:"<<timer.Elapsed()<<"s"<<endl;
	return 0;
}