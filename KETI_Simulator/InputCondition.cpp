// InputCondition.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KETI_Simulator.h"
#include "InputCondition.h"


// CInputCondition 대화 상자입니다.

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


// CInputCondition 메시지 처리기입니다.

void CInputCondition::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	count = ConditionList.GetCurSel();
	if(count < 0 )
	{
		AfxMessageBox(TEXT("조건을 입력해야만 합니다."));
		return;
	}
	OnOK();
}

BOOL CInputCondition::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	ConditionList.InsertString(0,_T("=="));
	ConditionList.InsertString(1,_T(">"));
	ConditionList.InsertString(2,_T(">="));
	ConditionList.InsertString(3,_T("<"));
	ConditionList.InsertString(4,_T("<="));
	ConditionList.InsertString(5,_T("!="));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CInputCondition::OnCbnCloseupCombo1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

}
