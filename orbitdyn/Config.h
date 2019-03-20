#pragma once


#ifdef _MSC_VER
	#define ORBITDYN_DLL
	#ifdef OrbitDyn_EXPORTS
		#define ORBITDYN_VAR extern __declspec(dllexport)
		#define ORBITDYN_API __declspec(dllexport)
	#else
		#define ORBITDYN_VAR extern __declspec(dllimport)
		#define ORBITDYN_API __declspec(dllimport)
	#endif //ORBITDYN_EXPORTS
#endif //_MSC_VER

#ifndef ORBITDYN_DLL
	#define ORBITDYN_VAR extern
	#define ORBITDYN_API
#endif


#if defined _MSC_VER && _MSC_VER < 1300
	#include <iostream.h>
	#include <fstream.h>
	#include <math.h>
	#include <stdexcpt.h>
#else
	#include <iostream>
	#include <cmath>
	#include <stdexcept>
	using namespace std;
#endif // _MSC_VER