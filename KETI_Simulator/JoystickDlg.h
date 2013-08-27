#pragma once

#include "Joystick.h"
#include "afxwin.h"

// CJoystickDlg 대화 상자입니다.

class CJoystickDlg : public CDialog
{
	DECLARE_DYNAMIC(CJoystickDlg)

public:
	CJoystickDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CJoystickDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_WHEELCONTROL };
	CJoystick		m_joystick;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	void registerSimulator(Simulator* simulator);

	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	 Simulator* _simulator;	 
public:
	RobotInfo	rInfo;
	rxSystem*	rSys[3];
	rxDevice*	rDevice[256];
	afx_msg void OnBnClickedScontrol();
	CComboBox RobotName;
	CComboBox LWheelName;
	CComboBox RWheelName;
	float Vforward;
	float Vangular;
	int Lid;
	int Rid;
	void SysDataInit(rxSystem** sys, rxDevice** device, RobotInfo rData);
	afx_msg void OnCbnCloseupRobotlist();
	afx_msg void OnDestroy();
};
