#pragma once


// DepthView ��ȭ �����Դϴ�.

class DepthView : public CDialog
{
	DECLARE_DYNAMIC(DepthView)

public:
	DepthView(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~DepthView();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DEPTHVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void InitData(rxDevice *dev);

	rxDevice* device;
	afx_msg void OnPaint();
	afx_msg void OnNcDestroy();
};
