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
#include "Utility.h"

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

#pragma comment(lib,"OrbitDyn.lib")

#endif // __ORBITDYN_H
