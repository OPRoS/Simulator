#include "StdAfx.h"
#include "KETI_Simulator.h"
#include "rDevice/rDeviceType.h"
#include "Simulator.h"
#include "RobotControlData.h"
#include "KETI_SimulatorDlg.h"
#include "RobotControlServer.h"

RobotControlServer	gc_RobotControlServer;
RobotControlServer	g_RobotControlServer;
RobotControlData	*g_RobotControlData;


RobotControlData::RobotControlData(void)
{
}

RobotControlData::~RobotControlData(void)
{
}

void RobotControlData::registerSimulator(Simulator* simulator)
{
	_simulator = simulator;
}

void RobotControlData::initRobotControlData(rxSystem **sys, rxDevice **device , int cnt_sys, int cnt_device[4])
{
	//m_sys = new rxSystem [cnt_sys];
	//m_device = new rxDevice [cnt_device];
	CKETI_SimulatorDlg* pMain = (CKETI_SimulatorDlg*)AfxGetMainWnd();

	for (int i = 0 ; i < cnt_sys ; i++)
	{
		m_sys[i] = sys[i];
	}
	for (int i = 0 ; i < cnt_device[3] ; i++)
	{
		m_device[i] = device[i];
	}

	RobotCnt = cnt_sys;
	for (int i = 0 ; i < 4 ; i++)
	{
		DeviceCnt[i] = cnt_device[i];
	}
	g_RobotControlData = this;
	g_RobotControlServer.StartServer( pMain->PortNumber );	
	gc_RobotControlServer.StartServer( pMain->PortNumber );	
}