#include "orbitdyn.h"

#include <mex.h>
#pragma comment(lib,"libmx.lib")
#pragma comment(lib,"libmex.lib")


void Warning(std::string msg)
{
	mexWarnMsgTxt(msg.c_str());
}

void Error(std::string msg)
{
	mexErrMsgTxt(msg.c_str());
}


// input prhs[0]: MJD of epoch
//       prhs[1]: index of center body
//       prhs[2]: index of target body
// output: plhs[0]: Î»ÖÃËÙ¶È(km,km/s)
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	// check for proper number of arguments
	if(nrhs!=2 && nrhs!=3)
		mexErrMsgTxt("Two or three input required.");
	else if(nlhs > 1) 
		mexErrMsgTxt("Too many output arguments.");

	/* input must be a double */
	if ( mxIsDouble(prhs[0]) != 1)
		mexErrMsgTxt("First input must be a double.");
	if ( mxIsDouble(prhs[1]) != 1)
		mexErrMsgTxt("Second input must be a double.");
	if (nrhs == 3 && mxIsDouble(prhs[2]) != 1)
		mexErrMsgTxt("Second input must be a double.");

	/* input must be a scale */
	if (mxGetM(prhs[0])!=1 || mxGetN(prhs[0])!=1)
		mexErrMsgTxt("First input must be a scalar.");
	if (mxGetM(prhs[1])!=1 || mxGetN(prhs[1])!=1)
		mexErrMsgTxt("Second input must be a scalar.");
	if (nrhs == 3 && (mxGetM(prhs[2])!=1 || mxGetN(prhs[2])!=1))
		mexErrMsgTxt("Third input must be a scalar.");

	double* mjd = mxGetPr(prhs[0]);
	CDateTime epoch;
	epoch.SetMJD(*mjd-1.0/3.0);

	double* cent = mxGetPr(prhs[1]);
	int icent = (int)(*cent);

	try{
		if((PLANET)icent==NUTATIONS)
		{
			plhs[0] = mxCreateDoubleMatrix(1,2,mxREAL);
			double* out = mxGetPr(plhs[0]);
			DE421::Instance()->Nutations(epoch,*out,*(out+1));
		}
		else if((PLANET)icent==LIBRATIONS)
		{
			plhs[0] = mxCreateDoubleMatrix(1,3,mxREAL);
			double* out = mxGetPr(plhs[0]);
			DE421::Instance()->Librations(epoch,*out,*(out+1),*(out+2));
		}
		else
		{
			double* target = mxGetPr(prhs[2]);
			int itar = (int)(*target);
			plhs[0] = mxCreateDoubleMatrix(1,6,mxREAL);
			double* out = mxGetPr(plhs[0]);
			DE421::Instance()->PlanetEphemeris(epoch,(PLANET)icent,(PLANET)itar,out);
		}
	}
	catch(BaseException& e)
	{
		Error(e.GetFullMessage());
	}
	catch(exception &e)
	{
		Error(e.what());
	}
}