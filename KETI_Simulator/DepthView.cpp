// DepthView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Simulator.h"
#include "KETI_Simulator.h"
#include "DepthView.h"


// DepthView 대화 상자입니다.

IMPLEMENT_DYNAMIC(DepthView, CDialog)

DepthView::DepthView(CWnd* pParent /*=NULL*/)
	: CDialog(DepthView::IDD, pParent)
{

}

DepthView::~DepthView()
{
}

void DepthView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DepthView, CDialog)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_NCDESTROY()
END_MESSAGE_MAP()


// DepthView 메시지 처리기입니다.

void DepthView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	unsigned char vision[320*240*3] = {0,};
	CClientDC dc(this);
	CBrush brush;
	CPen pen;
	CBrush* oldbrush;
	CPen* oldpen;

	int cnt = device->readDeviceValue((void*)vision,sizeof(float)*320*240*3);

	for (int i = 0 ; i < cnt ; i=i+3)
	{
		/*pen.DeleteObject();
		brush.DeleteObject();*/

		dc.SetPixel(((i/3)%320), 240-((i/3)/320),RGB(vision[i+2],vision[i+1],vision[i]));

		/*pen.CreatePen(PS_SOLID,1,RGB(vision[i+2],vision[i+1],vision[i]));
		oldpen = dc.SelectObject(&pen);
		brush.CreateSolidBrush(RGB(vision[i+2],vision[i+1],vision[i]));
		oldbrush = dc.SelectObject(&brush);

		dc.Rectangle(((i/3) % 320)+10 , 250 - (unsigned char)((i/3) / 240)+10,
			((i/3) % 320)+10 , 250 - (unsigned char)((i/3) / 240)+10);

		dc.SelectObject(oldpen);
		dc.SelectObject(oldbrush);*/
	}
	CDialog::OnTimer(nIDEvent);
}

void DepthView::InitData(rxDevice *dev)
{
	device = dev;
	SetTimer(1,50,0);
	//SubclassDlgItem(IDC_Depth,this);
	//SetFocus();
	UpdateData(FALSE);
}

void DepthView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialog::OnPaint()을(를) 호출하지 마십시오.
}

void DepthView::OnNcDestroy()
{
	CDialog::OnNcDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	delete this;
}
