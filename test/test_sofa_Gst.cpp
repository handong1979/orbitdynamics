/*!
\file test.cpp
\author HanDle
≤‚ ‘≥Ã–Ú
*/
#include <OrbitDyn.h>
#include <PerfTimer.h>
#include <sofa.h>
#pragma comment(lib,"sofa.lib")

using namespace Constant;

void test_sofa_Gst()
{
	double ut11,ut12;
	iauUtcut1(2400000.5,53736.0,0.3388134,&ut11,&ut12);
	double tai1,tai2;
	iauUtctai(2400000.5,53736.0,&tai1,&tai2);
	double tt1,tt2;
	iauTaitt(tai1,tai2,&tt1,&tt2);
	double gst06a = iauGst06a(ut11,ut12,tt1,tt2);
	double gst00a = iauGst00a(ut11,ut12,tt1,tt2);
	double gst00b = iauGst00b(ut11,ut12);
	double sg = GetSG(CDateTime(2006,1,1,0,0,0));
	cout << "   GetSG  = " << sg << endl;
	cout << "iauGst06a = " << gst06a << endl;
	cout << "iauGst00a = " << gst00a << endl;
	cout << "iauGst00b = " << gst00b << endl;
	cout << (sg - gst06a)*DEG << endl;
	cout << (sg - gst00a)*DEG << endl;
	cout << (sg - gst00b)*DEG << endl;
}

int main(int argc, char* argv[])
{
	CPerfTimer timer;
	timer.Start();

	try
	{
		cout.precision(12);

		test_sofa_Gst();
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