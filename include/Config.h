#pragma once

#ifndef _CONFIG_H

#ifdef ORBITDYN_STATIC
//#ifndef ORBITDYN_DLL
	#define ORBITDYN_VAR extern
	#define ORBITDYN_API
#else
	#ifdef OrbitDyn_EXPORTS
		#define ORBITDYN_VAR extern __declspec(dllexport)
		#define ORBITDYN_API __declspec(dllexport)
	#else
		#define ORBITDYN_VAR extern __declspec(dllimport)
		#define ORBITDYN_API __declspec(dllimport)
	#endif //ORBITDYN_EXPORTS
#endif //ORBITDYN_DLL

// 是否使用数据文件，未定义时，使用DE405星历、地球自转参数、和引力场文件
// 定义时，不需要这些文件，日月星历采用近似解析解，暂时没有其他行星的星历

//#define NONE_DATA_FILE_MODE

#endif

