// democontrolDlg.h : header file
//

#if !defined(AFX_LEIGUIDLG_H__26317BCB_5256_4649_8869_153905653F6D__INCLUDED_)
#define AFX_LEIGUIDLG_H__26317BCB_5256_4649_8869_153905653F6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ResizeControl.h"
#include "cv.h"
#include "highgui.h"
#include "csrc\Detect.h"
#include <ctime>

/////////////////////////////////////////////////////////////////////////////
// CLeiGUIDlg dialog

class CLeiGUIDlg : public CDialog
{
// Construction
public:
	CLeiGUIDlg(CWnd* pParent = NULL);	// standard constructor
	CResizeControl  m_Resize;
	ShowFind(CvRect rect);
	CFont m_font;
// Dialog Data
	//{{AFX_DATA(CLeiGUIDlg)
	enum { IDD = IDD_RecognizeDialog };
	CEdit	m_result;
	CStatic	m_file_name;
	int		m_KnnOrSVM;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLeiGUIDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CLeiGUIDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnOpenFile();
	afx_msg void OnFindIt();
	afx_msg void OnRecognize();
	afx_msg void OnDestroy();
	afx_msg void OnKnn();
	afx_msg void OnSvm();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CString m_path;
	CString m_fileName;
	IplImage *m_origin;
	CvRect *m_rect;
	CString m_resultString;
	double m_time;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMOCONTROLDLG_H__26317BCB_5256_4649_8869_153905653F6D__INCLUDED_)
