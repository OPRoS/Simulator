#pragma once
//#include "KETI_SimulatorDlg.h"

#define NUM_DIVISON		24
#define HALF_DIVISON	12
#define ANGLE (float)	0.26179 // or PI / NUM_DIVISON  = 15 degrees

#define STICK_SIZE		7
#define SPIN_SIZE		7
#define MIN_KB_INC		2
#define MAX_KB_INC		5

#define SPIN_RING_WIDTH	25

#define LINE_COLOR		RGB(0, 0, 0)
#define MOVE_COLOR		RGB(130, 150, 255)
#define BK_GND_CLR		RGB(120, 120, 250)
#define HANDLE_CLR		RGB(255, 0, 0)
#define NUM_PENS		64

#define PI				(float) 3.1415926
#define PI_HALF			(float) 1.5707963267948966
#define RAD2DEG			(float) 57.2957795130823208
#define DEG2RAD			(float) 0.01745329251994329

// CJoystick 대화 상자입니다.

class CJoystick : public CDialog
{
	DECLARE_DYNAMIC(CJoystick)

private:
	enum ACTIVE { NONE, STICK, SPIN };

	CRect	stickRect;			//	Rectangle enclosing the stick handle
	CRect	spinInnerRect;		//	Rectangle enclosing the spin inner circle
	CRect	spinOuterRect;		//	Rectangle enclosing the spin outer circle

	CRgn	stickRgn;			//	Region for the direction and velocity
	CRgn	spinRgn;			//	Region for the direction and orientation
	CRgn	joyRgn;				//	= stickRgn + spinRgn
	CRgn	stickHandle;		//	Region for the stick handle
	CRgn	spinHandle;			//	Region for the spin handle

	CPoint	stickCenter;		//	Center of the stick Rect
	CPoint	spinCenter;			//	Center of the spin Rect
	CPoint	stickPos;			//	Current position of the stick handle
	CPoint	spinPos;			//	Current position of the spin handle

	BOOL	hasFocus;			//	Does the window currently has focus or not

	int		stickRadius;		//	Radius of the biggest circle in stickrect
	int		spinInnerRadius;	//	Radius of the biggest circle in the spinInnerRect
	int		spinOuterRadius;	//	Radius of the biggest circle in the spinOuterRect
	int		spinArcRadius;		//	Radius of the arc used for orientation

	ACTIVE	active;				//	To keep track of the which region
								//	(i.e. the stick or spin region) was the 
								//	last to be activated

	CPen m_Pen[NUM_PENS];		//	Pens to draw the gradient view of the stick region

	TRACKMOUSEEVENT m_TrackMouseEvent;	//	Structure to activate the 
										//	WM_MOUSELEAVE message

	CPoint	vertices[NUM_DIVISON];		//	Hold the vertices for drawing the
										//	angled lines	

	void updateStickPos(CPoint point);
	void updateStickRgn(CDC& dc, COLORREF color = HANDLE_CLR);
	void updateStickRgn(COLORREF color = HANDLE_CLR);


	//	Function to gradient fill the stick region
	void GradientFill(CDC& dc);
	void updateJoystick(CDC& dc);
	void updateJoystick();

public:
	CJoystick(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CJoystick();
	CPoint GetMoveDir();
	int GetMoveRate();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_WHEELCONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNcDestroy();
};
