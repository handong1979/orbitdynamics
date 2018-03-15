#include "OrbitDyn.h"

#include <mex.h>
#pragma comment(lib,"libmx.lib")
#pragma comment(lib,"libmex.lib")

static std::vector<CSatellite*> vsat;

bool debug = true;

char* stoupper( char* s )
  {
  char* p = s;
  while (*p = toupper( *p )) p++;
  return s;
  }

void Warning(std::string msg)
{
	mexWarnMsgTxt(msg.c_str());
}

void Error(std::string msg)
{
	mexErrMsgTxt(msg.c_str());
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	// check for proper number of arguments
	if(nrhs<2)
		mexErrMsgTxt("Two input required at least.");
	else if(nlhs > 1) 
		mexErrMsgTxt("Too many output arguments.");

	/* input must be a string */
	if ( mxIsChar(prhs[0]) != 1)
		mexErrMsgTxt("First input must be a string.");
	if ( mxIsChar(prhs[1]) != 1)
		mexErrMsgTxt("Second input must be a string.");

	/* copy the string data from prhs[0] into a C string input_ buf.    */
	char *sat_name;
	sat_name = mxArrayToString(prhs[0]);
	if(sat_name == NULL) 
		mexErrMsgTxt("Could not convert first input to string.");
	std::string name = sat_name;

	char *char_oper;
	char_oper = mxArrayToString(prhs[1]);
	char* char_upper_oper = stoupper(char_oper);
	if(char_oper==NULL)
		mexErrMsgTxt("Could not convert second input to string.");
	std::string operat = char_upper_oper;
    
	try{
	size_t nsat = vsat.size();
	size_t i;
	std::vector<CSatellite*>::iterator itvsat;
	// ����vsat������Ϊname������,��¼��λ��i�͵����itvsat
	itvsat = vsat.begin();
	for(i=0; i<nsat; i++)
	{
		if( vsat[i]->Name == name )
			break;
		itvsat++;
	}
    
	std::string msg;
	if(i==nsat)
	{// û���ҵ���Ӧ���ֵ�����,�����һ��������?
		if(operat == "INIT")
		{
			size_t N = mxGetN(prhs[2]);
			if(N<12) Error("Init parameter is not enough.");
			double* tmp = mxGetPr(prhs[2]);
			CDateTime t((int)tmp[0],(int)tmp[1],(int)tmp[2],(int)tmp[3],(int)tmp[4],tmp[5]);
			CDateTimeView tv = t;
			Kepler k(tmp[6],tmp[7],tmp[8]*RAD,tmp[9]*RAD,tmp[10]*RAD,tmp[11]*RAD);
            if(debug)
            {
                mexPrintf("Input Epoch: %d %d %d %d %d %f\n",(int)tmp[0],(int)tmp[1],(int)tmp[2],(int)tmp[3],(int)tmp[4],tmp[5]);
                mexPrintf("Input Elements: %f %f %f %f %f %f\n",tmp[6],tmp[7],tmp[8]*RAD,tmp[9]*RAD,tmp[10]*RAD,tmp[11]*RAD);
                mexPrintf("Input Mass: %f\n",tmp[12]);
                mexPrintf("Input Kepler: %f %f %f %f %f %f\n",k.a,k.e,k.i*DEG,k.o*DEG,k.w*DEG,k.M*DEG);
            }
            CSatellite* sat = new CSatellite;
			vsat.push_back(sat);
            
			//vsat[i] = new CSatellite;		
			vsat[i]->Initialize(t-28800,k);
			vsat[i]->SetForce(6,ODP_EARTH_ALL);
			vsat[i]->Name = name;
            if(N==13) vsat[i]->Mass0 = tmp[12];
            
            if(debug)
            {
                mexPrintf("Name = %s\n",vsat[i]->Name.c_str());
                mexPrintf("Output Elements: %f %f %f %f %f %f\n",vsat[i]->a,vsat[i]->e,vsat[i]->i,vsat[i]->Omega,vsat[i]->w,vsat[i]->M);
            }
		}
		else
		{
			Error("satellite '" + name + "' does not exist, please initialize first or check its name.");
		}
	}
	else
	{ // ���ҵ�name��Ӧvsat�ĵ�i������
		if(operat == "STEP") // ����һ��,prhs[2]Ϊ����
		{
			double* tmp = mxGetPr(prhs[2]);
			if(vsat[i]==NULL)
				Error("satellite '" + name +"' is not initialized or has been removed.");
			else
			{
				size_t N = mxGetN(prhs[2]);
				if(N==9) // ��������
				{
					double dm;
					vec3 F;
					CQuaternion qbi;
					qbi.qx = *(tmp+1);
					qbi.qy = *(tmp+2);
					qbi.qz = *(tmp+3);
					qbi.qs = *(tmp+4);
					F[0] = *(tmp+5);
					F[1] = *(tmp+6);
					F[2] = *(tmp+7);
					dm = *(tmp+8);					
					double fm = norm(F,2);//sqrt(F(0)*F(0)+F(1)*F(1)+F(2)*F(2));
					double Isp;
					if(fm>0.0)
					{
						if(dm<0)
						{
							Isp = -fm/dm/g;
							vsat[i]->SetEngine(Isp,fm);
							vsat[i]->ThrustDirection = F/fm;
							vsat[i]->qbi = qbi;
							vsat[i]->ThrustIsOn = true;
							if(*tmp>0)
								vsat[i]->Propagate(*tmp,*tmp);
							else if(*tmp<0)
								vsat[i]->PropagateBackward(*tmp,*tmp);
						}
						else
							Error("Force is positive, but /dotm is positive.");
					}
					else // �ϳ�����Ϊ0�������ƽ�������
					{
						vsat[i]->ThrustIsOn = false;
						if(*tmp>0)
							vsat[i]->Propagate(*tmp,*tmp);
						else if(*tmp<0)
								vsat[i]->PropagateBackward(*tmp,*tmp);
						vsat[i]->BurnedFuel += *tmp*(-dm);
					}
				}
				else // ����������
				{
					vsat[i]->ThrustIsOn = false;
					if(*tmp>0)
						vsat[i]->Propagate(*tmp,*tmp);
					else if(*tmp<0)
						vsat[i]->PropagateBackward(*tmp,*tmp);
				}
			}
		}
		else if(operat == "INIT")
		{ // prhs[2]Ϊʱ�䡢�������?
			size_t N = mxGetN(prhs[2]);
			if(N<12) Error("Init parameter is not enough.");
			double* tmp = mxGetPr(prhs[2]);
			CDateTime t((int)tmp[0],(int)tmp[1],(int)tmp[2],(int)tmp[3],(int)tmp[4],tmp[5]);
			CDateTimeView tv = t;
			Kepler k(tmp[6],tmp[7],tmp[8]*RAD,tmp[9]*RAD,tmp[10]*RAD,tmp[11]*RAD);
			if(vsat[i]!=NULL)
				Warning("satellite '" + name + "' had been initialized,re-initializing...");
			vsat[i]->Initialize(t-28800,k); // ���³�ʼ��ʱ�����Զ��޸��㶯ģ�ͣ�����������������
			if(N==13) vsat[i]->Mass0 = tmp[12];
		}
		else if(operat == "PROPAGATE")
		{
			double* tmp = mxGetPr(prhs[2]);
			vsat[i]->Propagate(60,*tmp);
		}
		else if(operat == "PROPAGATEBACKWARD")
		{
			double* tmp = mxGetPr(prhs[2]);
			vsat[i]->PropagateBackward(-60,*tmp);
		}
		else if(operat == "REMOVE")  // ɾ��
		{
			delete vsat[i];
			vsat.erase(itvsat);
			return;
		}
		else if(operat == "SETFORCE") // �����㶯ģ��
		{
			double* tmp = mxGetPr(prhs[2]);
			vsat[i]->SetForce((int)tmp[0],(unsigned int)tmp[1]);
		}
		else if(operat == "GETMEAN") // ���ƽ����?
		{
			Kepler km = vsat[i]->MedianElement();
			plhs[0] = mxCreateDoubleMatrix(1,6,mxREAL);
			double* out = mxGetPr(plhs[0]);
			out[0] = km.a;
			out[1] = km.e;
			out[2] = km.i;
			out[3] = km.o;
			out[4] = km.w;
			out[5] = km.M;
			return;
		}
		else if(operat == "SETMASS")
		{
			double* tmp = mxGetPr(prhs[2]);
			vsat[i]->Mass0 = *tmp;
			vsat[i]->BurnedFuel = 0;
		}
		else if(operat == "SETSRP")
		{
			double* tmp = mxGetPr(prhs[2]);
			vsat[i]->LightPressArea = *tmp;
		}
		else if(operat == "SETAD")
		{
			double* tmp = mxGetPr(prhs[2]);
			vsat[i]->AirDragArea = *tmp;
		}
		else if(operat == "SETATTITUDE")
		{
			double* tmp = mxGetPr(prhs[2]);
			vsat[i]->qbi.qx = *tmp;
			vsat[i]->qbi.qy = *(tmp+1);
			vsat[i]->qbi.qz = *(tmp+2);
			vsat[i]->qbi.qs = *(tmp+3);
		}
        else if(operat == "GETECF")
        {
            mat33 J2000_WGS84 = ECI_ECF(vsat[i]->CurrentEpoch());
            vec3 we;
            we[0] = 0; we[1] = 0; we[2] = We;
            vec3 r_WGS84 = J2000_WGS84*vsat[i]->Pos();
            vec3 v_WGS84 = J2000_WGS84*vsat[i]->Vel() - cross(we,r_WGS84);
			plhs[0] = mxCreateDoubleMatrix(1,6,mxREAL);
			double* out = mxGetPr(plhs[0]);
            out[0] = r_WGS84[0];
			out[1] = r_WGS84[1];
			out[2] = r_WGS84[2];
			out[3] = v_WGS84[0];
			out[4] = v_WGS84[1];
			out[5] = v_WGS84[2];
            return;
        }
	}
    if(debug)
    {   
        mexPrintf("Output Elements: %f %f %f %f %f %f\n",vsat[i]->a,vsat[i]->e,vsat[i]->i,vsat[i]->Omega,vsat[i]->w,vsat[i]->M);
    }
	
	plhs[0] = mxCreateDoubleMatrix(1,13,mxREAL);
	double* out = mxGetPr(plhs[0]);
	out[0] = vsat[i]->CurrentEpoch().GetMJD()+1.0/3.0;
	out[1] = vsat[i]->a;
	out[2] = vsat[i]->e;
	out[3] = vsat[i]->i;
	out[4] = vsat[i]->Omega;
	out[5] = vsat[i]->w;
	out[6] = vsat[i]->M;
	out[7] = vsat[i]->Pos()[0];
	out[8] = vsat[i]->Pos()[1];
	out[9] = vsat[i]->Pos()[2];
	out[10] = vsat[i]->Vel()[0];
	out[11] = vsat[i]->Vel()[1];
	out[12] = vsat[i]->Vel()[2];
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