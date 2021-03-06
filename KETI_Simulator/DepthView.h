#pragma once


// DepthView 대화 상자입니다.

class DepthView : public CDialog
{
	DECLARE_DYNAMIC(DepthView)

public:
	DepthView(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~DepthView();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DEPTHVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void InitData(rxDevice *dev);

	rxDevice* device;
	afx_msg void OnPaint();
	afx_msg void OnNcDestroy();
};
