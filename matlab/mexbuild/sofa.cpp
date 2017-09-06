#include "OrbitDyn.h"

#include "sofa.h"
#pragma comment(lib,"sofa.lib")

#include <mex.h>
#pragma comment(lib,"libmx.lib")
#pragma comment(lib,"libmex.lib")

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	// check for proper number of arguments
	if(nrhs<2)
		mexErrMsgTxt("Two input required at least.");

	/* input must be a string */
	if ( mxIsChar(prhs[0]) != 1)
		mexErrMsgTxt("First input must be a string.");

	/* copy the string data from prhs[0] into a C string input_ buf.    */
	char *char_oper;
	char_oper = mxArrayToString(prhs[0]);
	char* char_upper_oper = strupr(char_oper);
	if(char_oper==NULL)
		mexErrMsgTxt("Could not convert second input to string.");
	std::string operat = char_oper;

    if(operat == "GMST06")
    {
        double* p1 = mxGetPr(prhs[1]);
        CDateTime epoch;
        epoch.SetMJD(*p1);
        double dut1 = Earth::Instance()->GetdUT1(epoch);
        double ut11,ut12;
        iauUtcut1(2400000.5,*p1,dut1,&ut11,&ut12);
        double tai1,tai2;
        iauUtctai(2400000.5,*p1,&tai1,&tai2);
        double tt1,tt2;
        iauTaitt(tai1,tai2,&tt1,&tt2);
        
        plhs[0] = mxCreateDoubleMatrix(1,1,mxREAL);
        double* out = mxGetPr(plhs[0]);
        out[0] = iauGmst06(ut11,ut12,tt1,tt2);
    }
    else if(operat == "GST06A")
    {
        double* p1 = mxGetPr(prhs[1]);
        CDateTime epoch;
        epoch.SetMJD(*p1);
        double dut1 = Earth::Instance()->GetdUT1(epoch);
        double ut11,ut12;
        iauUtcut1(2400000.5,*p1,dut1,&ut11,&ut12);
        double tai1,tai2;
        iauUtctai(2400000.5,*p1,&tai1,&tai2);
        double tt1,tt2;
        iauTaitt(tai1,tai2,&tt1,&tt2);
        
        plhs[0] = mxCreateDoubleMatrix(1,1,mxREAL);
        double* out = mxGetPr(plhs[0]);
        out[0] = iauGst06a(ut11,ut12,tt1,tt2);
    }
    else if(operat == "GST06")
    {
        double* p1 = mxGetPr(prhs[1]);
        CDateTime epoch;
        epoch.SetMJD(*p1);
        double dut1 = Earth::Instance()->GetdUT1(epoch);
        double ut11,ut12;
        iauUtcut1(2400000.5,*p1,dut1,&ut11,&ut12);
        double tai1,tai2;
        iauUtctai(2400000.5,*p1,&tai1,&tai2);
        double tt1,tt2;
        iauTaitt(tai1,tai2,&tt1,&tt2);
        double rnpb[3][3];
        iauPnm06a(tt1,tt2,rnpb);
       
        plhs[0] = mxCreateDoubleMatrix(1,1,mxREAL);
        double* out = mxGetPr(plhs[0]);
        out[0] = iauGst06(ut11,ut12,tt1,tt2,rnpb);
    }
    else if(operat == "GST00A")
    {
        double* p1 = mxGetPr(prhs[1]);
        CDateTime epoch;
        epoch.SetMJD(*p1);
        double dut1 = Earth::Instance()->GetdUT1(epoch);
        double ut11,ut12;
        iauUtcut1(2400000.5,*p1,dut1,&ut11,&ut12);
        double tai1,tai2;
        iauUtctai(2400000.5,*p1,&tai1,&tai2);
        double tt1,tt2;
        iauTaitt(tai1,tai2,&tt1,&tt2);
        
        plhs[0] = mxCreateDoubleMatrix(1,1,mxREAL);
        double* out = mxGetPr(plhs[0]);
        out[0] = iauGst00a(ut11,ut12,tt1,tt2);
    }
    else if(operat == "GST00B")
    {
        double* p1 = mxGetPr(prhs[1]);
        CDateTime epoch;
        epoch.SetMJD(*p1);
        double dut1 = Earth::Instance()->GetdUT1(epoch);
        double ut11,ut12;
        iauUtcut1(2400000.5,*p1,dut1,&ut11,&ut12);
       
        plhs[0] = mxCreateDoubleMatrix(1,1,mxREAL);
        double* out = mxGetPr(plhs[0]);
        out[0] = iauGst00b(ut11,ut12);;
    }
    else if(operat == "GMST82")
    {
        double* p1 = mxGetPr(prhs[1]);
        CDateTime epoch;
        epoch.SetMJD(*p1);
        double dut1 = Earth::Instance()->GetdUT1(epoch);
        double ut11,ut12;
        iauUtcut1(2400000.5,*p1,dut1,&ut11,&ut12);
        
        plhs[0] = mxCreateDoubleMatrix(1,1,mxREAL);
        double* out = mxGetPr(plhs[0]);
        out[0] = iauGmst82(ut11,ut12);
    }
    else if(operat == "GST94")
    {
        double* p1 = mxGetPr(prhs[1]);
        CDateTime epoch;
        epoch.SetMJD(*p1);
        double dut1 = Earth::Instance()->GetdUT1(epoch);
        double ut11,ut12;
        iauUtcut1(2400000.5,*p1,dut1,&ut11,&ut12);
        
        plhs[0] = mxCreateDoubleMatrix(1,1,mxREAL);
        double* out = mxGetPr(plhs[0]);
        out[0] = iauGst94(ut11,ut12);
    }
    else
    {
        mexErrMsgTxt("Can't find function");
    }
}