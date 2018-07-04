#include <direct.h>
#include <engine.h>
#pragma comment(lib,"libeng.lib")
#pragma comment(lib,"libmx.lib")

class MatlabEngine
{
	Engine* ep;
public:
	MatlabEngine(){
		ep = engOpen(NULL);
		engSetVisible(ep,1);
	}

	~MatlabEngine(){
		//engClose(ep); // 不关闭，下次启动更快
	}
	void GotoCurrentDir(){
		char cwd[128];
		getcwd(cwd,128);
		char cdcwd[130] = "cd ";
		strcpy(cdcwd+3,cwd);
		engEvalString(ep,cdcwd);		
	}
	void PutVar(std::string varname,int ndim,double* pd){
		mxArray *mxa = mxCreateDoubleMatrix(1, ndim, mxREAL);
		memcpy((void *)(mxGetPr(mxa)), pd, sizeof(double)*ndim);
		engPutVariable(ep,varname.c_str(),mxa);
	}
	void Eval(std::string cmd){
		engEvalString(ep,cmd.c_str());
	}
	void Init(){
		engEvalString(ep,"clear");
		engEvalString(ep,"close all");
		engEvalString(ep,"[hs ds] = Init");
	}
	void Update(){
		engEvalString(ep,"Update");
	}
};