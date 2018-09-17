#pragma once


#ifdef __APPLE__
	#define ORBITDYN_VAR extern
	#define ORBITDYN_API
#endif //__APPLE__

#ifdef _MSC_VER
	#ifdef OrbitDyn_EXPORTS
		#define ORBITDYN_VAR extern __declspec(dllexport)
		#define ORBITDYN_API __declspec(dllexport)
	#else
		#define ORBITDYN_VAR extern __declspec(dllimport)
		#define ORBITDYN_API __declspec(dllimport)
  #endif //ORBITDYN_EXPORTS
#endif // __MSVC__

