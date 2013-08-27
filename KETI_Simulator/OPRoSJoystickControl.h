#pragma once

#include "Joystick.h"
#include "afxwin.h"


// COPRoSJoystickControl ��ȭ �����Դϴ�.

class COPRoSJoystickControl : public CDialog
{
	DECLARE_DYNAMIC(COPRoSJoystickControl)

public:
	COPRoSJoystickControl(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~COPRoSJoystickControl();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_WHEELCONTROL_OPRoS };
	CJoystick		m_joystick;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
