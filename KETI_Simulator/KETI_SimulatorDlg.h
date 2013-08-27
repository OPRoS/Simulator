// KETI_SimulatorDlg.h : header file
//

#pragma once
#include "dataType.h"
#include "rMath/rMath.h"
using namespace rMath;
#include <list>
#include <map>
#include "afxwin.h"
#include "afxcmn.h"
using namespace std;
// CKETI_SimulatorDlg dialog
class Simulator;
class SimulationController;
class RobotController;
class SimulatorData;
class COpenModelDlg;
class OPRoSEngineRedirection;
class RobotControlData;
class CJoystickDlg;
class COPRoSJoystickControl;
class CManipulatorDlg;
class COPRoSManipulatorDlg;
class CSimulatorDebug;
class DepthView;

class CKETI_SimulatorDlg : public CDialog
{
// Construction
public:
	struct sItemInfo 
	{
		CString modelname;
		CString filename;
		int id;
		int	model_id;
		bool	is_system;
		HTransform T0;
	};
	CKETI_SimulatorDlg(CWnd* pParent = NULL);	// standard constructor
	~CKETI_SimulatorDlg();

// Dialog Data
	enum { IDD = IDD_KETI_SIMULATOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT OnLogMessage(WPARAM wParam, LPARAM lParam);
	
	friend void SimulationLoop(void* param);

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedOpenModelBtn();
private:
	CListCtrl m_modelListCtrl;
	map<int, sItemInfo> m_itemInfoes;
	SimulationController* m_simulationController;
	RobotController* m_robotController;
	SimulatorData* m_simulatorData;
	CSimulatorDebug			*m_SimulatorDebug;
	CJoystickDlg			*m_JoystickDlg;
	COPRoSJoystickControl	*m_OPRoSJoystick;
	CManipulatorDlg			*m_Manipulator;
	COPRoSManipulatorDlg	*m_OPRoSManipulator;
	DepthView				*m_DepthView;
	RobotControlData* m_RobotControlData;
	RobotInfo	rData;
	bool m_bRunSimulation;
	HANDLE m_hThread;

	HTransform	_transform_org;
	CRITICAL_SECTION   _hCriticalSection;

	bool	_aml_loaded;
	bool	_eml_loaded;

public:
	OPRoSEngineRedirection	*m_oprosEngineRedirection;
	Simulator* m_simulator;
	afx_msg void OnBnClickedStartSimBtn();
	afx_msg void OnBnClickedStopSimBtn();
private:
	CEdit m_logMsg;
public:
	afx_msg void OnBnClickedUnloadModelBtn();	
	afx_msg void OnBnClickedClearBtn();
	afx_msg void OnBnClickedResetBtn();
	afx_msg void OnClose();
	CButton _btnStartSimulation;
	CButton _btnStopSimulation;
	afx_msg void OnBnClickedEngineStart();
	CListCtrl AppList;
	afx_msg void OnBnClickedEngineExit();
	CEdit EngineState;
	CListBox AppRunList;
	WCHAR sysPath[255];
	CString	EnginePath;
	CString	RepositoryPath;
	CString ModelPath;
	CString EngineName;
	CString EngineBatchFile;
	int		PortNumber;
	afx_msg void OnNMClickAppList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkAppList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLbnDblclkAppRunlist();
	afx_msg void OnWheelrobotSimulatorcontrol();
	afx_msg void OnWheelrobotOprosenginecontrol();
	afx_msg void OnManipulatorSimulatorcontrol();
	afx_msg void OnManipulatorOprosenginecontrol();
	afx_msg void OnFilePathsetting();
	CTreeCtrl m_deviceTree;
	//CTabCtrl m_debugTab;
	//afx_msg void OnTcnSelchangeTab2(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg void OnTcnSelchangingTab2(NMHDR *pNMHDR, LRESULT *pResult);
	CTabCtrl m_debugTab;
	afx_msg void OnTcnSelchangeDebugtab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnSelchangingDebugtab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickTree2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();
};
