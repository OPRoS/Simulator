// KETI_Simulator.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CKETI_SimulatorApp:
// See KETI_Simulator.cpp for the implementation of this class
//

class CKETI_SimulatorApp : public CWinApp
{
public:
	CKETI_SimulatorApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CKETI_SimulatorApp theApp;