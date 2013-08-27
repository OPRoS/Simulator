#pragma once
#include "rDevice/rDeviceType.h"
#include "Simulator.h"
#include <afxsock.h>

class RobotControlData
{
public:
	RobotControlData(void);
	~RobotControlData(void);
	Simulator* _simulator;

	rxSystem*	m_sys[3];
	rxDevice*	m_device[128];
	int			RobotCnt;
	int			DeviceCnt[4];

	void registerSimulator(Simulator* simulator);
	void initRobotControlData(rxSystem **sys, rxDevice **device, int cnt_sys, int cnt_device[4]);
};
