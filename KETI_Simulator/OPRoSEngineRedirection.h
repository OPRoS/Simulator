#include "Redir.h"

#pragma once

class CKETI_SimulatorDlg;

class OPRoSEngineRedirection
{
public:
	OPRoSEngineRedirection(void);
	~OPRoSEngineRedirection(void);
	
	void registerSimulator(Simulator* simulator);
	void LoadAppList(void);
	void viewComponentList(CString sIndexValue);
	BOOL RunApp(int AppIdx);
	void DelRunAppList(int idx);
	void EngineStart(void);
	void EngineAppRun(CString appname);
	void EngineAppStop(CString appname);
	void EngineExit(void);
	bool GetProcess(CString processName);
	CRedirector m_redir;
	
private:
	Simulator* _simulator;
	
	int	RunAppIndex;
	int EngineStateFlag;
};
