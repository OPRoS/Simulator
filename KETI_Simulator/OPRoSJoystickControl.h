#pragma once

#include "Joystick.h"
#include "afxwin.h"


// COPRoSJoystickControl 대화 상자입니다.

class COPRoSJoystickControl : public CDialog
{
	DECLARE_DYNAMIC(COPRoSJoystickControl)

public:
	COPRoSJoystickControl(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~COPRoSJoystickControl();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_WHEELCONTROL_OPRoS };
	CJoystick		m_joystick;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
//	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	void registerSimulator(Simulator* simulator);

	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	float Vforward;
	float Vangular;
	BOOL	InitSharedMemory(CString appname);
	HANDLE	m_hMap;
	LPBYTE	m_pSharedMemory;
	CString	appname;

private:
	 Simulator* _simulator;	
public:
	afx_msg void OnDestroy();
	CComboBox JoystickAppList;
	afx_msg void OnBnClickedStart();
	afx_msg void OnClose();
	afx_msg void OnNcDestroy();
};
