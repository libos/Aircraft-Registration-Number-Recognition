// democontrolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "leiGUI.h"
#include "leiGUIDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLeiGUIDlg dialog

CLeiGUIDlg::CLeiGUIDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLeiGUIDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLeiGUIDlg)
	m_KnnOrSVM = -1;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLeiGUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLeiGUIDlg)
	DDX_Control(pDX, IDC_Result, m_result);
	DDX_Control(pDX, IDC_STATIC_FileName, m_file_name);
	DDX_Radio(pDX, IDC_KNN, m_KnnOrSVM);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLeiGUIDlg, CDialog)
	//{{AFX_MSG_MAP(CLeiGUIDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_OpenFile, OnOpenFile)
	ON_BN_CLICKED(IDC_FindIt, OnFindIt)
	ON_BN_CLICKED(IDC_Recognize, OnRecognize)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_KNN, OnKnn)
	ON_BN_CLICKED(IDC_SVM, OnSvm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLeiGUIDlg message handlers

BOOL CLeiGUIDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_time = 0.0;
	m_resultString ="";
	m_rect = NULL;
 
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	 

	((CButton *)GetDlgItem(IDC_SVM))->SetCheck(TRUE);
	m_KnnOrSVM = 1;

	m_Resize.SetOwner(this);

	m_Resize.SetResize(IDC_OpenFile,PK_TOP_RIGHT,PK_TOP_RIGHT); 
	m_Resize.SetResize(IDC_FindIt,PK_TOP_RIGHT,PK_TOP_RIGHT); 
	m_Resize.SetResize(IDC_Recognize,PK_TOP_RIGHT,PK_TOP_RIGHT); 
	m_Resize.SetResize(IDC_STATIC_Origin,PK_TOP_LEFT,PK_BOTTOM_LEFT ); 
	m_Resize.SetResize(IDC_STATIC_Split,PK_BOTTOM_LEFT,PK_BOTTOM_LEFT); 
	m_Resize.SetResize(IDC_Result,PK_BOTTOM_RIGHT,PK_BOTTOM_RIGHT);  
 
 
	CWnd *pEdit=GetDlgItem(IDC_Result);
	CFont *pfont =	pEdit->GetFont();
	LOGFONT logfont;
	pfont->GetLogFont(&logfont);
	logfont.lfHeight   =   -38;
	m_font.CreateFontIndirect(&logfont);
	pEdit->SetFont(&m_font);


	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLeiGUIDlg::OnPaint() 
{
	if(m_path != "")
	{
			CPaintDC dc(this); // device context for painting
			HBITMAP bmpHandle = (HBITMAP)LoadImage(NULL,
				m_path,
				IMAGE_BITMAP,
				0,
				0,
				LR_LOADFROMFILE);
			CBitmap bmpPicture;
			CDC mdcPicture;
			
			CBitmap *bmpFromHandle = bmpPicture.FromHandle(bmpHandle);
			
			CRect rctPicture;
			mdcPicture.CreateCompatibleDC(&dc); 
			CBitmap * bmpPrevious = mdcPicture.SelectObject(bmpFromHandle);

			BITMAP bm;
			GetDlgItem(IDC_STATIC_Origin)->GetWindowRect(&rctPicture);

			ScreenToClient(&rctPicture);
			
			dc.SetStretchBltMode(HALFTONE);
			bmpFromHandle->GetBitmap(&bm);

			dc.StretchBlt(rctPicture.left, rctPicture.top,
					 rctPicture.Width(), rctPicture.Height(),
					&mdcPicture, 0, 0,  bm.bmWidth, bm.bmHeight ,SRCCOPY);
 

			if(m_rect != NULL)
			{
				CClientDC tdc(this);
				float rx = bm.bmWidth *1.0 / rctPicture.Width();
				float ry = bm.bmHeight *1.0/ rctPicture.Height();
			    
				CPen pen;
			    pen.CreatePen(PS_SOLID, 3, RGB(255,0 , 0));
	   		    CPen* pOldPen = tdc.SelectObject(&pen);

				CBrush *brushBlue = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));//创建一个透明的画刷
			    CBrush *pOldBrush = tdc.SelectObject(brushBlue);
			 
				CRect rect((int)(m_rect->x *1.0 / rx),(int)(m_rect->y *1.0/ry) ,(int)((m_rect->x+m_rect->width) *1.0/ rx),(int)((m_rect->y+m_rect->height)*1.0 /ry));
				rect.OffsetRect(rctPicture.left,rctPicture.top);
				tdc.Rectangle(rect);

				tdc.SelectObject( pOldBrush );
				tdc.SelectObject( pOldPen );
				
			}
			
			if(m_rect != NULL)
			{
					
				GetDlgItem(IDC_STATIC_Split)->GetWindowRect(&rctPicture);
				ScreenToClient(&rctPicture);
				dc.SetStretchBltMode(HALFTONE);
 
				dc.StretchBlt(rctPicture.left, rctPicture.top,
					 rctPicture.Width(), rctPicture.Height(),
					 &mdcPicture, m_rect->x, m_rect->y, m_rect->width, m_rect->height ,SRCCOPY);
			} 
			dc.SelectObject(bmpPrevious);
	}
	if( m_resultString != "")
	{
		m_result.SetWindowText(m_resultString);
	}
	else
	{
		m_result.SetWindowText("");
	}
	if(m_time != 0.0)
	{
		CString tt ;
		tt.Format("共用时%fs",m_time);
		GetDlgItem(IDC_TIME)->SetWindowText(tt);
	}
	else
	{
			GetDlgItem(IDC_TIME)->SetWindowText("");
	}
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);

		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLeiGUIDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CLeiGUIDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	m_Resize.OnSize(cx,cy);
	
}
 
BOOL CLeiGUIDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(WM_KEYDOWN == pMsg->message ) 
	{ 
		UINT nKey = (int) pMsg->wParam; 
		if( VK_RETURN == nKey || VK_ESCAPE == nKey ) 
			return TRUE ;	
	}	
	return CDialog::PreTranslateMessage(pMsg);
}

void CLeiGUIDlg::OnOpenFile() 
{
	CFileDialog fd(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"位图文件(*.bmp)|*.bmp|所有文件|*.*");
	
	if (IDOK == fd.DoModal())
	{		
		m_time = 0.0;
		m_resultString ="";
		m_rect = NULL;
		m_path= fd.GetPathName();
		m_fileName = fd.GetFileName();
		m_file_name.SetWindowText(m_path);
		m_origin = cvLoadImage(m_path);
		Invalidate();
	}
}

void CLeiGUIDlg::OnFindIt() 
{
	if(m_path == "")
	{
		MessageBox("请先打开文件！","步骤错误");
		return ;
	}
	char result[20];
	memset(result,0,20); 
	m_rect = (CvRect*)malloc(sizeof(CvRect));
	
	clock_t start = clock();
	
	bool useSvm = m_KnnOrSVM == 1 ? true : false;
	Detect(m_origin,result,useSvm,m_rect,true); 
	
	clock_t stop = clock();
	m_time = (double)(stop - start) / CLOCKS_PER_SEC;
	
	CRect rctPicture;
	GetDlgItem(IDC_STATIC_Split)->GetWindowRect(&rctPicture);
	ScreenToClient(&rctPicture);
	InvalidateRect(rctPicture,FALSE);
}
 
void CLeiGUIDlg::OnRecognize() 
{
	
	if(m_path == "")
	{
		MessageBox("请先打开文件！","步骤错误");
		return ;
	}
	char result[20];
	memset(result,0,20); 
	m_rect = (CvRect*)malloc(sizeof(CvRect));
	clock_t start = clock();
	bool useSvm = m_KnnOrSVM == 1 ? true : false;
	Detect(m_origin,result,useSvm,m_rect);
	
	clock_t stop = clock();
	m_time = (double)(stop - start) / CLOCKS_PER_SEC;
	
	CRect rctPicture;
	GetDlgItem(IDC_STATIC_Split)->GetWindowRect(&rctPicture);
	ScreenToClient(&rctPicture);
	InvalidateRect(rctPicture,FALSE);

	m_resultString.Format("%s",result);

}
 

void CLeiGUIDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	m_font.DeleteObject();	
}

void CLeiGUIDlg::OnKnn() 
{
	m_KnnOrSVM = 0;
	
}

void CLeiGUIDlg::OnSvm() 
{
	m_KnnOrSVM	= 1;
}
