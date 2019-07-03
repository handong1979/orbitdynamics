#pragma once
#include <OrbitDyn.h>
class ColorNoise
{
public:

	ColorNoise(void)
		:sigma(1),mean(0),delta(1),a(0.2),s(0)
	{
	}

	ColorNoise(double sg,double mn)
		:sigma(sg),mean(mn),delta(1),a(0.2),s(0)
	{
	}

	ColorNoise(double sg,double mn,double dlt,double aa)
		:sigma(sg),mean(mn),delta(dlt),a(aa),s(0)
	{
	}

	~ColorNoise(void)
	{
	}

	double sigma; // 驱动白噪声的方差
	double mean;  // 驱动白噪声的均值

	double delta; // 大约为方差
	double a;     // 自相关系数0<a<1,a=0时为白噪声

	double s;     // 状态

	// 取个新数
	double GetNum()
	{
		s = delta*WhiteNoise(sigma*3,mean) + a*s;
		return s;
	}
};
