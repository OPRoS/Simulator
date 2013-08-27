// Joystick.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KETI_Simulator.h"
#include "Joystick.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CJoystick 대화 상자입니다.

IMPLEMENT_DYNAMIC(CJoystick, CDialog)

CJoystick::CJoystick(CWnd* pParent /*=NULL*/)
	: CDialog(CJoystick::IDD, pParent)
{
	hasFocus			= FALSE;

	stickHandle.CreateRectRgn(0, 0, 0, 0);

	m_TrackMouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
	m_TrackMouseEvent.dwFlags = TME_LEAVE;
	m_TrackMouseEvent.dwHoverTime = HOVER_DEFAULT;

}

CJoystick::~CJoystick()
{
}

void CJoystick::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CJoystick, CDialog)

	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_NCDESTROY()
END_MESSAGE_MAP()


void CJoystick::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	//	All regions need to be initialized only once

	CRgn firstRgn, secondRgn, netRgn;
	CRect winRect, squareRect;
	GetClientRect(&winRect);

	// form the square of the client Rect
	squareRect = winRect;
	squareRect.right = (winRect.Width() < winRect.Height()) ? winRect.right : winRect.bottom;

	CPoint center = winRect.CenterPoint();
	int offset = squareRect.right / 2;
	squareRect.SetRect(center, center);
	squareRect.InflateRect(offset, offset);
	spinInnerRect = spinOuterRect = stickRect = squareRect;

	// Initialize each of the rect regions
	spinOuterRect.DeflateRect(5, 5, 5, 5);	// outermost of the rectangle
	spinInnerRect.DeflateRect(SPIN_RING_WIDTH, SPIN_RING_WIDTH);
	stickRect.DeflateRect(5, 5, 5, 5);	// stick's outer rectangle

	spinOuterRadius		= spinOuterRect.Width() / 2;
	spinInnerRadius		= spinInnerRect.Width() / 2;
	stickRadius			= stickRect.Width() / 2;
	spinCenter			= stickCenter = stickRect.CenterPoint();
	spinCenter.y		= ((spinOuterRect.top + spinInnerRect.top) / 2) + 1;
	spinArcRadius		= (spinOuterRadius + spinInnerRadius) / 2;

	// Create individual regions of the rectangles
	firstRgn.CreateEllipticRgnIndirect(&spinOuterRect);
	secondRgn.CreateEllipticRgnIndirect(&spinInnerRect);
	stickRgn.CreateEllipticRgnIndirect(&stickRect);

	// ring regions is the area common to outer rect region and the inner rect region
	secondRgn.CombineRgn(&firstRgn, &secondRgn, RGN_DIFF);

	// create the top half of the region
	firstRgn.DeleteObject();
	CRect top_half(spinOuterRect);
	top_half.DeflateRect(0, spinOuterRect.Width() / 2, 0, 0);
	firstRgn.CreateRectRgnIndirect(&top_half);

	// the spin reqion is the area common to ring region and the top region
	spinRgn.CreateRectRgn(0, 0, 0, 0);
	spinRgn.CombineRgn(&secondRgn, &firstRgn, RGN_DIFF);

	firstRgn.DeleteObject();
	secondRgn.DeleteObject();

	// the net window region will be the combination of the stick and spin regions
	netRgn.CreateRectRgn(0, 0, 0, 0);
	netRgn.CombineRgn(&spinRgn, &stickRgn, RGN_OR);

	// backup the net rgn so that the backup copy can be used for drawing purposes
	joyRgn.CreateRectRgn(0, 0, 0, 0);
	joyRgn.CopyRgn(&netRgn);

	// set the window region
	SetWindowRgn(NULL, FALSE);
	SetWindowRgn(netRgn, TRUE);

	updateStickPos(stickCenter);

	float angle = 0;
	for (int i = 0; i < HALF_DIVISON; i++)
	{
		vertices[i].x = (int) ((cos(angle) * stickRadius) + stickCenter.x);
		vertices[i].y = (int) ((sin(angle) * stickRadius) + stickCenter.y);
		vertices[i + HALF_DIVISON].x = (int) ((cos(angle + PI) * (spinOuterRadius)) + stickCenter.x);
		vertices[i + HALF_DIVISON].y = (int) ((sin(angle + PI) * (spinOuterRadius)) + stickCenter.y);
	
		angle += ANGLE;
	}

	//	Create the pens for the gradient fill
	int pen_width = (int) stickRadius / NUM_PENS;
	for (int i = 0; i < NUM_PENS; i++)
		m_Pen[i].CreatePen(PS_SOLID, 2, RGB(130, 150, 255 - (i * 4)));

	GradientFill(dc);
	updateJoystick(dc);

}

void CJoystick::GradientFill(CDC& dc)
{
	CPen* pOldPen = dc.GetCurrentPen();
	CRect rect(stickCenter, stickCenter);
	
	float choice = (float) (1.0 * NUM_DIVISON / (float) stickRadius);
	for(int i = 0; i < stickRadius; i++)
	{
		rect.InflateRect(1, 1, 1, 1);
		dc.SelectObject(&m_Pen[(int) (i * choice)]);
		dc.Arc(&rect, CPoint(rect.left, rect.top + (rect.Width() / 2)), CPoint(rect.left, rect.top + (rect.Width() / 2)));
	}

	dc.SelectObject(pOldPen);
}

void CJoystick::updateJoystick(CDC& dc)
{
	CPen *pNewPen = NULL, *pOldPen = NULL;
	CBrush *pNewBrush = NULL, *pOldBrush = NULL;

	// Draw the angled lines 
	pNewPen = new CPen(PS_SOLID, 1, LINE_COLOR);
	pOldPen = dc.SelectObject(pNewPen);

	for (int i = 1; i < HALF_DIVISON; i++)
	{
		dc.MoveTo(vertices[i]);
		dc.LineTo(vertices[i + HALF_DIVISON]);
	}

	// Draw each of the circle at a distance of 1/4
	CRect rect(stickRect);
	int quarter_radius = stickRadius / 4;
	rect.DeflateRect(quarter_radius, quarter_radius, quarter_radius, quarter_radius);
	dc.Arc(rect, CPoint(stickCenter.x, rect.top), CPoint(stickCenter.x, rect.top));
	rect.DeflateRect(quarter_radius, quarter_radius, quarter_radius, quarter_radius);
	dc.Arc(rect, CPoint(stickCenter.x, rect.top), CPoint(stickCenter.x, rect.top));
	rect.DeflateRect(quarter_radius, quarter_radius, quarter_radius, quarter_radius);
	dc.Arc(rect, CPoint(stickCenter.x, rect.top), CPoint(stickCenter.x, rect.top));

	pNewPen = dc.SelectObject(pOldPen);
	delete pNewPen;

	// Darken the vertical and horizontal lines and also draw the arc for the spinner
	pNewPen = new CPen(PS_SOLID, 1, RGB(0, 0, 0));
	pOldPen = dc.SelectObject(pNewPen);
	dc.MoveTo(vertices[HALF_DIVISON / 2].x, vertices[HALF_DIVISON / 2].y);
	dc.LineTo(vertices[(int) (1.5 * HALF_DIVISON)].x, vertices[(int) (1.5 * HALF_DIVISON)].y);
	dc.MoveTo(vertices[HALF_DIVISON].x, vertices[HALF_DIVISON].y);
	dc.LineTo(vertices[(int) (0)].x, vertices[(int) (0)].y);	
	dc.SelectObject(pOldPen);
	delete pNewPen;

	updateStickRgn(dc);
}

void CJoystick::updateJoystick()
{
	CClientDC dc(this);
	updateJoystick(dc);
}



void CJoystick::updateStickPos(CPoint point)
{
	point.x = point.x + 1;
	point.y = point.y + 1;
	stickPos = point;

	CRect stickPosRect(point, point);
	stickPosRect.InflateRect(STICK_SIZE, STICK_SIZE);

	CRgn Temp;
	Temp.CreateEllipticRgnIndirect(&stickPosRect);
	stickHandle.CombineRgn(&Temp, &stickRgn, RGN_AND);
	Temp.DeleteObject();
}

void CJoystick::updateStickRgn(CDC& dc, COLORREF color)
{
	CBrush brush(color);
	dc.FillRgn(&stickHandle, &brush);
	brush.DeleteObject();
}

void CJoystick::updateStickRgn(COLORREF color)
{
	CClientDC dc(this);
	updateStickRgn(dc, color);
}

// CJoystick 메시지 처리기입니다.

CPoint CJoystick::GetMoveDir()
{
	CPoint result = stickPos - stickCenter;

	TRACE("%d %d\n", result.x, result.y);

	return result;
}

int CJoystick::GetMoveRate()
{
	CPoint diff = GetMoveDir();
	double result = sqrt(double(diff.x * diff.x) + double(diff.y * diff.y));

	result /= stickRadius;

	TRACE("Position Update = %d\n", (int) ceil(result * 10));

	return (int) ceil(result * 10);
}

void CJoystick::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	if (nFlags & (MK_LBUTTON | MK_RBUTTON))
	{
		//	Initialize the mouse leave event as we know that the mouse
		//	is now in the window
		if (! hasFocus && (nFlags & MK_LBUTTON))
		{
			m_TrackMouseEvent.hwndTrack = this->m_hWnd;
			::_TrackMouseEvent(&m_TrackMouseEvent);
			hasFocus = TRUE;
		}

		updateStickRgn(MOVE_COLOR);
		updateStickPos(stickRgn.PtInRegion(point) ? point : stickCenter);

		updateJoystick();
	}

	CDialog::OnMouseMove(nFlags, point);
}

LRESULT CJoystick::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your message handler code here

	CPaintDC dc(this); // device context for painting

	hasFocus = FALSE;

	//	If the mouse has moved from the stick region
	updateStickRgn(MOVE_COLOR);
	updateStickPos(stickCenter);
	active = NONE;
	updateJoystick();

	//	If the mouse has moved from the spin regio
	return 0;	
}

void CJoystick::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	updateStickRgn(MOVE_COLOR);
	updateStickPos(point);
	updateJoystick();

	CDialog::OnLButtonDown(nFlags, point);
}

void CJoystick::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	updateStickRgn(MOVE_COLOR);
	updateStickPos(stickCenter);
	updateJoystick();

	CDialog::OnLButtonUp(nFlags, point);
}


void CJoystick::OnNcDestroy()
{
	CDialog::OnNcDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	delete this;
}
