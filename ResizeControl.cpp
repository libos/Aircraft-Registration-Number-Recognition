// ResizeControl.cpp: implementation of the CResizeControl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "ResizeControl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CResizeControl::CResizeControl()
{

//	m_pWnd=pWnd;
}

CResizeControl::~CResizeControl()
{

}

void CResizeControl::SetOwner(CWnd *pWnd)
{

	m_pWnd=pWnd;
	pWnd->GetClientRect(&m_rtParent);

}

void CResizeControl::OnSize(int cx,int cy)
{
	int offsetx,offsety;
	CRect rt;
	offsetx=cx-m_rtParent.right-m_rtParent.left;
	offsety=cy-m_rtParent.bottom-m_rtParent.top;
	for(int i=0;i<m_arrResize.GetSize();i++)
	{
		rt=m_arrResize.GetAt(i).rt;
		switch(m_arrResize.GetAt(i).type1)
		{
		case PK_TOP_LEFT:
			break;
		case PK_TOP_RIGHT:
			rt.left+=offsetx;
			break;
		case PK_BOTTOM_LEFT:
			rt.top+=offsety;
			break;
		case PK_BOTTOM_RIGHT:
			rt.top+=offsety;
			rt.left+=offsetx;
			break;
		}
		switch(m_arrResize.GetAt(i).type2)
		{
		case PK_TOP_LEFT:
			break;
		case PK_TOP_RIGHT:
			rt.right+=offsetx;
			break;
		case PK_BOTTOM_LEFT:
			rt.bottom+=offsety;
			break;
		case PK_BOTTOM_RIGHT:
			rt.bottom+=offsety;
			rt.right+=offsetx;
			break;
		}
		if(m_pWnd->GetDlgItem(m_arrResize.GetAt(i).ID)->m_hWnd)
			m_pWnd->GetDlgItem(m_arrResize.GetAt(i).ID)->MoveWindow(rt);

	}
}

void CResizeControl::SetResize(UINT ID, int type1, int type2)
{
	tagResize iCon;
	CRect rt;
	iCon.ID=ID;
	m_pWnd->GetDlgItem(ID)->GetWindowRect(&rt);
	m_pWnd->ScreenToClient(rt);
	iCon.rt=rt;
	iCon.type1=type1;
	iCon.type2=type2;
	m_arrResize.Add(iCon);
}
