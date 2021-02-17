/*
armadillo use blas lapack

*/

#pragma once

#ifdef _MSC_VER
#ifdef _WIN64
#define ARMA_BLAS_UNDERSCORE

#ifndef ARMA_USE_BLAS
#define ARMA_USE_BLAS
#endif

#ifndef ARMA_USE_LAPACK
#define ARMA_USE_LAPACK
#endif

#include "armadillo.h"

#pragma comment(lib,"blas_win64_MT.lib")
#pragma comment(lib,"lapack_win64_MT.lib")

#else
#define ARMA_BLAS_UNDERSCORE
#define ARMA_USE_BLAS
#define ARMA_USE_LAPACK

#include "armadillo.h"

#pragma comment(lib,"blas_win32.lib")
#pragma comment(lib,"lapack_win32.lib")

#endif // _WIN64


#endif // _MSC_VER

#if __APPLE__

#ifndef ARMA_USE_BLAS
#define ARMA_USE_BLAS
#endif

#ifndef ARMA_USE_LAPACK
#define ARMA_USE_LAPACK
#endif

#define ARMA_BLAS_CAPITALS

#include "armadillo.h"

#endif //__APPLE__

#ifdef linux

#ifndef ARMA_USE_BLAS
#define ARMA_USE_BLAS
#endif

#ifndef ARMA_USE_LAPACK
#define ARMA_USE_LAPACK
#endif

#include "armadillo.h"
#endif