#ifndef __STOPCONDITION_H
#define __STOPCONDITION_H

#include "Config.h"

class CSatelliteBase;

/*! ������Ƶ�ֹͣ����

*/
class ORBITDYN_API CStopCondition
{
protected:
	//! ��Ӧһ������
	CSatelliteBase* Sat;
	//! ��һ����ֵ
	double PreValue;
	//! Ŀ��ֵ
	double Trip;
	//! Ŀ��ֵ���
	double Tolerance;
public:
	CStopCondition();
	virtual ~CStopCondition();

	//! �Ƿ�ֹͣ��
	virtual bool Stop() = 0;
	//! �Ƿ񳬹�(��Ҫ����),���ز�����������
	virtual double Over() = 0;
};

#endif  //__STOPCONDITION_H
