#ifndef __ORBITDYN_H
#define __ORBITDYN_H

#include "Config.h"
#include "BaseException.h"
#include "CDateTime.h"
#include "StringUtility.h"
#include "PlanetEphemeris.h"
#include "Euler.h"
#include "Facility.h"
#include "Satellite.h"
#include "utility.h"

#define TAB "\t"

#define ORBITDYN_VERSION_MAJOR 1
#define ORBITDYN_VERSION_MINOR 7
#define ORBITDYN_VERSION_PATCH 3
#define ORBITDYN_VERSION_SVN   555
#define ORBITDYN_VERSION_NAME  "Han Dong"

struct orbitdyn_version
{
	static const unsigned int major = ORBITDYN_VERSION_MAJOR;
	static const unsigned int minor = ORBITDYN_VERSION_MINOR;
	static const unsigned int patch = ORBITDYN_VERSION_PATCH;
	static const unsigned int svn   = ORBITDYN_VERSION_SVN;

	static
		inline
		std::string
		as_string()
	{
		const char* nickname = ORBITDYN_VERSION_NAME;

		std::stringstream ss;
		ss << orbitdyn_version::major
			<< '.'
			<< orbitdyn_version::minor
			<< '.'
			<< orbitdyn_version::patch
			<< "("
			<< orbitdyn_version::svn
			<< ")"
			<< " ["
			<< nickname
			<< ']';

		return ss.str();
	}
};


#ifdef ORBITDYN_STATIC
//#ifndef ORBITDYN_DLL
	// static library
	// VC2005
	#if _MSC_VER >= 1400
		#ifdef _MT
			#ifdef _DLL
				#ifdef _DEBUG
					#pragma comment(lib,"OrbitDynMDllD.lib")
				#else
					#pragma comment(lib,"OrbitDynMDLLR.lib")
				#endif //_DEBUG
			#else //_DLL
				#ifdef _DEBUG
					#pragma comment(lib,"OrbitDynMD.lib")
				#else
					#pragma comment(lib,"OrbitDynMR.lib")
				#endif //_DEBUG
			#endif //_DLL
		#else
			#error 没有定义_MT,无法确定使用的库文件
		#endif //_MT
	#else // _MSC_VER
	//#error 不支持VC2005以下版本的VC
		#ifdef _DEBUG
			#pragma comment(lib,"OrbitDynSD.lib")
		#else
			#pragma comment(lib,"OrbitDynSR.lib")
		#endif //_DEBUG
	#endif // _MSC_VER

#else  //ORBITDYN_STATIC
// Dynamic Link Library
	#ifndef ORBITDYN_EXPORTS
		#if _MSC_VER == 1500
            // VC2008
			#pragma comment(lib,"OrbitDynVC90.lib")
		#else
			#ifdef _DEBUG
				#ifdef NONE_DATA_FILE_MODE
					#pragma comment(lib,"OrbitDynNFD.lib")
				#else
					#pragma comment(lib,"OrbitDynD.lib")
				#endif //NONE_DATA_FILE_MODE
			#else
				#ifdef NONE_DATA_FILE_MODE
					#pragma comment(lib,"OrbitDynNF.lib")
				#else
					#pragma comment(lib,"OrbitDyn.lib")
				#endif //NONE_DATA_FILE_MODE
			#endif // _DEBUG
		#endif
	#endif //ORBITDYN_EXPORTS
#endif // ORBITDYN_STATIC


#endif // __ORBITDYN_H
