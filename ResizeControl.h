// ResizeControl.h: interface for the CResizeControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESIZECONTROL_H__5F3BB72F_1190_4B42_A57A_3BFC83AFC3C3__INCLUDED_)
#define AFX_RESIZECONTROL_H__5F3BB72F_1190_4B42_A57A_3BFC83AFC3C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "afxtempl.h" 
typedef struct _tagResize
{
	UINT ID;
	CRect rt;
	int type1;
	int type2;
}tagResize;
enum{PK_TOP_LEFT,PK_TOP_RIGHT,PK_BOTTOM_LEFT,PK_BOTTOM_RIGHT};

class CResizeControl  
{
public:
	void SetResize(UINT ID,int type1,int type2);
	void OnSize(int cx,int cy);
	void SetOwner(CWnd *pWnd);
	CResizeControl();
	virtual ~CResizeControl();

private:
	CWnd *m_pWnd;
	CArray <tagResize, tagResize>m_arrResize;
	CRect m_rtParent;

};

#endif // !defined(AFX_RESIZECONTROL_H__5F3BB72F_1190_4B42_A57A_3BFC83AFC3C3__INCLUDED_)
