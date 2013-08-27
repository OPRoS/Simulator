// JoystickDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Simulator.h"
#include "KETI_Simulator.h"
#include "KETI_SimulatorDlg.h"
#include "JoystickDlg.h"

// CJoystickDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CJoystickDlg, CDialog)

CJoystickDlg::CJoystickDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CJoystickDlg::IDD, pParent)
	, Vforward(0)
	, Vangular(0)
{

}

CJoystickDlg::~CJoystickDlg()
{
}

void CJoystickDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ROBOTLIST, RobotName);
	DDX_Control(pDX, IDC_COMBO2, LWheelName);
	DDX_Control(pDX, IDC_COMBO3, RWheelName);
	DDX_Text(pDX, IDC_EDIT_FORWARD, Vforward);
	DDX_Text(pDX, IDC_EDIT_ANGULAR, Vangular);
}


BEGIN_MESSAGE_MAP(CJoystickDlg, CDialog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_SCONTROL, &CJoystickDlg::OnBnClickedScontrol)
	ON_CBN_CLOSEUP(IDC_ROBOTLIST, &CJoystickDlg::OnCbnCloseupRobotlist)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CJoystickDlg �޽��� ó�����Դϴ�.

BOOL CJoystickDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	m_joystick.SubclassDlgItem(IDC_JOYSTICK, this);
	m_joystick.SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CJoystickDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CPoint moveDir = m_joystick.GetMoveDir();
	Vforward = (float)-(moveDir.y - 1) * 0.025;
	Vangular = (float)(moveDir.x - 1) * 0.025;

	float buf[2];
	buf[0] = (float)( Vforward + Vangular);
	buf[1] = (float)( Vforward + Vangular);
	rDevice[Lid]->writeDeviceValue((void*)buf, sizeof(float)*2);
	buf[0] = (float)( Vforward - Vangular);
	buf[1] = (float)( Vforward - Vangular);
	rDevice[Rid]->writeDeviceValue((void*)buf, sizeof(float)*2);

	UpdateData(FALSE);

	CDialog::OnTimer(nIDEvent);
}

void CJoystickDlg::registerSimulator(Simulator* simulator)
{
	_simulator = simulator;
}
void CJoystickDlg::OnBnClickedScontrol()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	int rSelect = RobotName.GetCurSel();
	int ldSelect = LWheelName.GetCurSel();
	int rdSelect = RWheelName.GetCurSel();

	Lid = rInfo.subCount[rSelect] + ldSelect;
	Rid = rInfo.subCount[rSelect] + rdSelect;
	RobotName.EnableWindow(FALSE);
	LWheelName.EnableWindow(FALSE);
	RWheelName.EnableWindow(FALSE);
	SetTimer(10, 50 , NULL);
}

void CJoystickDlg::SysDataInit(rxSystem** sys, rxDevice** device,RobotInfo rData)
{
	rInfo = rData;
	for (int i = 0 ; i < rData.rCount ; i++)
	{
		rSys[i] = sys[i];
		RobotName.InsertString(i,rSys[i]->name().c_str());
	}
	for (int i = 0 ; i < rData.dCount ; i++)
	{
		rDevice[i] = device[i];
	}
}

void CJoystickDlg::OnCbnCloseupRobotlist()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	LWheelName.ResetContent();
	RWheelName.ResetContent();

	int rSelect = RobotName.GetCurSel();

	for (int i = rInfo.subCount[rSelect] ; i < rInfo.subCount[rSelect+1] ; i++)
	{
		LWheelName.InsertString(i-rInfo.subCount[rSelect],rDevice[i]->name().c_str());
		RWheelName.InsertString(i-rInfo.subCount[rSelect],rDevice[i]->name().c_str());
	}
}

void CJoystickDlg::OnDestroy()
{
	CDialog::OnDestroy();
	/*CWnd* m_pParent;

	if (m_pParent)
	{
		((CKETI_SimulatorDlg*)m_pParent)->m_JoystickDlg = NULL;
	}*/
	delete this;
}

