#pragma once
#include "Config.h"
#include "Constant.h"
#include "CDateTime.h"
#include "armadillo_BLAS_LAPACK.h"
using namespace arma;
#include "nrlmsise-00.h"
/*!
 \class ��Msise00ģ�Ͱ�װ�������ʽ
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

	//��������ܶ�
	double Density(const CDateTime& t,const vec3& ECFr);
};

ORBITDYN_VAR CMsise00 Msise;
