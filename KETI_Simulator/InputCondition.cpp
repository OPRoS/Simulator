// InputCondition.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "KETI_Simulator.h"
#include "InputCondition.h"


// CInputCondition ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CInputCondition, CDialog)

CInputCondition::CInputCondition(CWnd* pParent /*=NULL*/)
	: CDialog(CInputCondition::IDD, pParent)
	, Value(0)
{

}

CInputCondition::~CInputCondition()
{
}

void CInputCondition::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_Value, Value);
	DDX_Control(pDX, IDC_COMBO1, ConditionList);
}


BEGIN_MESSAGE_MAP(CInputCondition, CDialog)
	ON_BN_CLICKED(IDOK, &CInputCondition::OnBnClickedOk)
	ON_CBN_CLOSEUP(IDC_COMBO1, &CInputCondition::OnCbnCloseupCombo1)
END_MESSAGE_MAP()


// CInputCondition �޽��� ó�����Դϴ�.

void CInputCondition::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	count = ConditionList.GetCurSel();
	if(count < 0 )
	{
		AfxMessageBox(TEXT("������ �Է��ؾ߸� �մϴ�."));
		return;
	}
	OnOK();
}

BOOL CInputCondition::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	ConditionList.InsertString(0,_T("=="));
	ConditionList.InsertString(1,_T(">"));
	ConditionList.InsertString(2,_T(">="));
	ConditionList.InsertString(3,_T("<"));
	ConditionList.InsertString(4,_T("<="));
	ConditionList.InsertString(5,_T("!="));

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CInputCondition::OnCbnCloseupCombo1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

}
