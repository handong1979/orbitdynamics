#ifndef __STOPCONDITION_H
#define __STOPCONDITION_H

#include "config.h"

class CSatelliteBase;

/*! 轨道外推的停止条件

*/
class ORBITDYN_API CStopCondition
{
protected:
	//! 对应一颗卫星
	CSatelliteBase* Sat;
	//! 上一步的值
	double PreValue;
	//! 目标值
	double Trip;
	//! 目标值误差
	double Tolerance;
public:
	CStopCondition();
	virtual ~CStopCondition();

	//! 是否停止？
	virtual bool Stop() = 0;
	//! 是否超过(需要回退),返回步长缩放因子
	virtual double Over() = 0;
};

#endif  //__STOPCONDITION_H
