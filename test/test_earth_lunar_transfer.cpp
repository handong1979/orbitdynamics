/*!
\file test.cpp
\author HanDle
测试程序
*/
#include <OrbitDyn.h>
using namespace Constant;

//! 测试地月转移轨道
void test_earth_lunar_transfer()
{
	fstream sat("satECI.dat",ios::out);
	sat.precision(15);
//	fstream moon("moonECI.dat",ios::out);
//	moon.precision(15);

	CDateTime end(2007,4,28,16,42,22.77);
	//CDateTime end(2007,4,28,16,42,23.78);
	Kepler Ep3(212859.603,   0.9670074, 30.983*RAD,179.984*RAD,   180.483*RAD,   0);
	//Kepler Ep3(212860,   0.96720074, 30.983*RAD, 180.483*RAD, 179.984*RAD,  0);

	CSatellite CE1;
	//CE1.Name = "CE1";
	CE1.SetAutoSave();
	CE1.Initialize(CDateTime(2007,4,23,22,41,32),Ep3, 'i');
	CE1.SetForce(8,ODP_EARTH_TESSERAL|ODP_LUNAR_CENT|ODP_SOLAR_CENT);

	double Step = 360;
	vec3 Pos,Vel; // 月心惯性系位置速度
	CDateTime time = CE1.CurrentEpoch();

	Kepler elem;
	//ECI_MCI(time,CE1.Pos(),CE1.Vel(),Pos,Vel);
	//elem = Cartesian_Kepler(Pos,Vel,GM);
	//sat<<0<<TAB<<CE1.Pos()<<TAB<<CE1.Vel()<<TAB<<Pos<<TAB<<Vel<<TAB<<elem<<endl;

    while(time < end)
	{
		if( (time+=Step) > end){
			Step -= time-end;
			time = end;
		}
		CE1.Propagate(Step,Step);
		ECI_MCI(time,CE1.Pos(),CE1.Vel(),Pos,Vel);
		if(norm(Pos,2)<LunarGrvSph/2){
            elem = Cartesian_Kepler(Pos,Vel,GM);
        	sat<<CE1.t()<<TAB<<CE1.Pos()<<TAB<<CE1.Vel()<<TAB<<Pos<<TAB<<Vel<<TAB<<elem<<endl;
		}
	}
	cout<<time<<endl<<Pos<<TAB<<Vel<<endl;
}

int main(int argc, char* argv[])
{
	try
	{
		cout.precision(12);

		test_earth_lunar_transfer();
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
