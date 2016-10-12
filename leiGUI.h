// democontrol.h : main header file for the DEMOCONTROL application
//

#if !defined(AFX_LEIGUI_H__4EE9C306_530F_4C0A_B5F9_89AC11D00048__INCLUDED_)
#define AFX_LEIGUI_H__4EE9C306_530F_4C0A_B5F9_89AC11D00048__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CLeiGUIApp:
// See leiGUI.cpp for the implementation of this class
//

class CLeiGUIApp : public CWinApp
{
public:
	CLeiGUIApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLeiGUIApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CLeiGUIApp)
	afx_msg void OnOpenFile();
	afx_msg void OnFindIt();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMOCONTROL_H__4EE9C306_530F_4C0A_B5F9_89AC11D00048__INCLUDED_)
