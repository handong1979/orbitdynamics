/*!
\file test.cpp
\author HanDle
测试程序
*/
#include <OrbitDyn.h>
using namespace Constant;

//! 测试平根数和瞬根数的计算
void test_mean_inst()
{
	FILE * cla = fopen("Mean\\cla.txt","r");
	FILE * BLL = fopen("Mean\\Brouwer-Lyd Mean Long.txt","r");
	FILE * BLS = fopen("Mean\\Brouwer-Lyd Mean Short.txt","r");
	FILE * KI = fopen("Mean\\Kozai-Izsak Mean.txt","r");

	fstream result("Mean\\MY_Mean.txt",ios::out);
	double elem[7];
	Kepler inst,STK_KI,STK_BLL,STK_BLS;
	while(!feof(cla))
	{
		fscanf(cla,"%lf%lf%lf%lf%lf%lf%lf",elem,elem+1,elem+2,elem+3,elem+4,elem+5,elem+6);
		elem[3] *= RAD;
		elem[4] *= RAD;
		elem[5] *= RAD;
		elem[6] *= RAD;
		inst.SetElements(elem+1);
		result<<elem[0]<<"\t"<<Mean(inst)<<endl;

//		fscanf(BLL,"lf%lf%lf%lf%lf%lf%lf%",elem,elem+1,elem+2,elem+3,elem+4,elem+5,elem+6);
//		STK_BLL.SetElements(elem+1);
//
//		fscanf(BLS,"lf%lf%lf%lf%lf%lf%lf%",elem,elem+1,elem+2,elem+3,elem+4,elem+5,elem+6);
//		STK_BLS.SetElements(elem+1);
//
//		fscanf(KI,"lf%lf%lf%lf%lf%lf%lf%",elem,elem+1,elem+2,elem+3,elem+4,elem+5,elem+6);
//		STK_KI.SetElements(elem+1);
	}
}

int main(int argc, char* argv[])
{
	try
	{
		cout.precision(12);

		test_mean_inst();
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
