#pragma once
#include "Config.h"
#include "Constant.h"
#include "CDateTime.h"
#include "armadillo_BLAS_LAPACK.h"
using namespace arma;
#include "nrlmsise-00.h"
/*!
 \class 将Msise00模型包装成类的形式
*/
class ORBITDYN_API CMsise00
{
public:
	struct nrlmsise_input input;
	struct nrlmsise_flags flags;
	struct nrlmsise_output output;

public:
	CMsise00(void);
	~CMsise00(void);

	//计算大气密度
	double Density(const CDateTime& t,const vec3& ECFr);
};

ORBITDYN_VAR CMsise00 Msise;
