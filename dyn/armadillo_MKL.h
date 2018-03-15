/*
armadillo连接Intel MKL数学库，该数学库针对Intel CPU进行优化，运算速度较快

经过简单测试，对于较小的矩阵，其运算速度反而没有blas_win32、lapack_win32快。
可能对于大矩阵有一定优势，需要进一步试用
*/

#pragma once

#ifndef ARMA_USE_BLAS
#define ARMA_USE_BLAS
#endif

#ifndef ARMA_USE_LAPACK
#define ARMA_USE_LAPACK
#endif

// MKL include folder: 
//    C:\Program Files\Intel\Composer XE 2013\mkl\include
#include <mkl.h>

#include <armadillo.h>

// 是否动态链接，若动态链接，需要保证相关DLL在路径中
// MKL的DLL文件在以下目录中：
//     C:\Program Files\Intel\Composer XE 2013\redist
#define DYNAMIC_DLL   1

// sequential or MultiTread
#define SEQUENTIAL    1

// MKL的lib文件在以下目录中：
//     C:\Program Files\Intel\Composer XE 2013\mkl\lib
#if DYNAMIC_DLL
	#pragma comment(lib,"mkl_intel_c_dll.lib")
	#pragma comment(lib,"mkl_core_dll.lib")
	#if SEQUENTIAL
		#pragma comment(lib,"mkl_sequential_dll.lib")
	#else
		#pragma comment(lib,"mkl_intel_thread_dll.lib")
		#pragma comment(lib,"libiomp5md.lib")
	#endif
#else
	#pragma comment(lib,"mkl_intel_c.lib")
	#pragma comment(lib,"mkl_core.lib")
	#if SEQUENTIAL
		#pragma comment(lib,"mkl_sequential.lib")
	#else
		#pragma comment(lib,"mkl_intel_thread.lib")
		#pragma comment(lib,"libiomp5md.lib")
	#endif
#endif  // DYNAMIC_DLL