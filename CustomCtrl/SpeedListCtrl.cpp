//*****************************************************************************
// Filename	: 	SpeedListCtrl.cpp
// Created	:	2015/8/17 - 16:15
// Modified	:	2015/8/17 - 16:15
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#include "stdafx.h"
#include "SpeedListCtrl.h"

CSpeedListCtrl::CSpeedListCtrl()
{
	//m_arLabels.SetSize(100);
	m_LabelCount = 0;
	memset(hColumn, 0, sizeof(hColumn));
}

CSpeedListCtrl::~CSpeedListCtrl()
{
	m_arLabels.RemoveAll();
}

//BEGIN_MESSAGE_MAP(CSpeedListCtrl, CListCtrl)
BEGIN_MESSAGE_MAP(CSpeedListCtrl, CMFCListCtrl)
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnGetdispinfo)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
END_MESSAGE_MAP()

int CSpeedListCtrl::AddItem(LPCTSTR lpszItem)
{
	ASSERT(::IsWindow(m_hWnd));
	return InsertItem(LVIF_TEXT, 0, lpszItem, 0, 0, 0, 0);
}

int CSpeedListCtrl::InsertItem(UINT nMask, int nItem, LPCTSTR lpszItem, UINT nState, UINT nStateMask, int nImage, LPARAM lParam)
{
	ASSERT(::IsWindow(m_hWnd));

	CLabelItem Label;
	Label.sItem.Add(lpszItem);
	//Label.sItem.SetAtGrow(0, lpszItem);

	m_LabelCount++;
	m_arLabels.Add(Label);
	//m_arLabels.SetAtGrow(m_LabelCount++, Label);
	return m_LabelCount - 1;
}

void CSpeedListCtrl::Complete()
{
	SetItemCountEx(m_LabelCount);
}

BOOL CSpeedListCtrl::SetItemText(int nItem, int nSubItem, LPCTSTR lpszText)
{	
	m_arLabels[nItem].sItem.SetAt(nSubItem, lpszText);
	//m_arLabels[nItem].sItem.SetAtGrow(nSubItem, lpszText);

	return TRUE;
}

void CSpeedListCtrl::OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	LV_ITEM* pItem = &(pDispInfo)->item;
	CLabelItem rLabel = m_arLabels.ElementAt(pItem->iItem);

	if (pItem->mask & LVIF_TEXT)
		lstrcpy(pItem->pszText, rLabel.sItem.GetSize() > pItem->iSubItem ? rLabel.sItem[pItem->iSubItem] : _T(""));

	*pResult = 0;
}

BOOL CSpeedListCtrl::DeleteAllItems()
{
	//for (int i = m_arLabels.GetCount() - 1; i >= 0; i--)
		//m_arLabels.RemoveAt(i);
	m_arLabels.RemoveAll();
	m_LabelCount = 0;
	SetItemCountEx(m_LabelCount);

	//m_arLabels.SetSize(100);
	return TRUE;
}

BOOL CSpeedListCtrl::DeleteItem(int nItem)
{
	return FALSE;
}

int __cdecl CompareText(const TCHAR* p1, const TCHAR* p2)
{
	TCHAR *sz1 = (TCHAR*)p1;
	TCHAR *sz2 = (TCHAR*)p2;
	//TCHAR *sz1 = ((CString*)(p1))->GetBuffer(0);
	//TCHAR *sz2 = ((CString*)(p2))->GetBuffer(0);
	int nPos1 = -1;
	int nPos2 = -1;
	int nEndPos1;
	int nEndPos2;
	int nResult;
	while (1)
	{
		nPos1++;
		nPos2++;
		// Make sure we haven't hit the end of either of the strings
		if (sz1[nPos1] == 0 && sz2[nPos2] == 0)
			return 0;
		if (sz1[nPos1] == 0)
			return -1;
		if (sz2[nPos2] == 0)
			return 1;
		// See if this part of both strings is a number
		if (sz1[nPos1] >= _T('0') && sz1[nPos1] <= _T('9') &&
			sz2[nPos2] >= _T('0') && sz2[nPos2] <= _T('9'))
		{
			// Find the end of each number
			nEndPos1 = nPos1;
			do
			{
				nEndPos1++;
			} while (sz1[nEndPos1] >= _T('0') && sz1[nEndPos1] <= _T('9'));
			nEndPos2 = nPos2;
			do
			{
				nEndPos2++;
			} while (sz2[nEndPos2] >= _T('0') && sz2[nEndPos2] <= _T('9'));
			while (true)
			{
				if (nEndPos1 - nPos1 == nEndPos2 - nPos2)
				{
					// Both numbers are the same length, just compare them
					nResult = _tcsnicmp(sz1 + nPos1, sz2 + nPos2, nEndPos1 - nPos1);
					if (nResult == 0)
					{
						nPos1 = nEndPos1 - 1;
						nPos2 = nEndPos2 - 1;
						break;
					}
					else
					{
						return nResult;
					}
				}
				else if (nEndPos1 - nPos1 > nEndPos2 - nPos2)
				{
					// First number is longer, so if it's not zero padded, it's bigger
					if (sz1[nPos1] == _T('0'))
						nPos1++;
					else
						return 1;
				}
				else
				{
					// Second number is longer, so if it's not zero padded, it's bigger
					if (sz2[nPos2] == _T('0'))
						nPos2++;
					else
						return -1;
				}
			}
		}
		else
		{
			// One or both characters is not a number, so just compare them as a string
			nResult = _tcsnicmp(sz1 + nPos1, sz2 + nPos2, 1);
			if (nResult != 0)
			{
				return nResult;
			}
		}
	}
}

int compare_column = 0;
int __cdecl CompareByLabelNameI(const void *elem1, const void *elem2)
{	
// 	USES_CONVERSION;
// 	LPCWSTR lpszName1 = A2CW((LPSTR)(LPCSTR)((CString*)(elem1))->GetBuffer(0));
// 	LPCWSTR lpszName2 = A2CW((LPSTR)(LPCSTR)((CString*)(elem2))->GetBuffer(0));
// 	return StrCmpLogicalW(lpszName1, lpszName2);

	CLabelItem *p1 = (CLabelItem*)elem1;
	CLabelItem *p2 = (CLabelItem*)elem2;

	return CompareText(p1->sItem.GetData()[compare_column], p2->sItem.GetData()[compare_column]);
	//return CompareText(p1->sItem[compare_column], p2->sItem[compare_column]);
	//return _tcscmp(p1->sItem[compare_column], p2->sItem[compare_column]);
	//return strcmp(p1->sItem[compare_column], p2->sItem[compare_column]);
}

int __cdecl CompareByLabelNameD(const void *elem1, const void *elem2)
{
// 	USES_CONVERSION;
// 	LPCWSTR lpszName1 = A2CW((LPSTR)(LPCSTR)((CString*)(elem2))->GetBuffer(0));
// 	LPCWSTR lpszName2 = A2CW((LPSTR)(LPCSTR)((CString*)(elem1))->GetBuffer(0));
//	return StrCmpLogicalW(lpszName1, lpszName2);

	CLabelItem *p1 = (CLabelItem*)elem1;
	CLabelItem *p2 = (CLabelItem*)elem2;
	
	return CompareText(p2->sItem.GetData()[compare_column], p1->sItem.GetData()[compare_column]);
	//return CompareText(p2->sItem[compare_column], p1->sItem[compare_column]);
	//return _tcscmp(p2->sItem[compare_column], p1->sItem[compare_column]);
	//return strcmp(p2->sItem[compare_column], p1->sItem[compare_column]);
}

void CSpeedListCtrl::SortByCol(const int ColIndex)
{
	compare_column = ColIndex;
	hColumn[ColIndex] = !hColumn[ColIndex];

	::AfxGetApp()->DoWaitCursor(TRUE);
	if (hColumn[ColIndex])
		//qsort(static_cast<void*>(&m_arLabels[0]), m_LabelCount, sizeof(CLabelItem), CompareByLabelNameI);
		qsort(static_cast<void*>(&m_arLabels[0]), m_LabelCount, sizeof(CLabelItem), CompareByLabelNameI);
	else
		//qsort(static_cast<void*>(&m_arLabels[0]), m_LabelCount, sizeof(CLabelItem), CompareByLabelNameD);
		qsort(static_cast<void*>(&m_arLabels[0]), m_LabelCount, sizeof(CLabelItem), CompareByLabelNameD);
	::AfxGetApp()->DoWaitCursor(FALSE);
}

void CSpeedListCtrl::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	LVCOLUMN Col;
	CString ColName;

	Col.mask = LVCF_TEXT;

	SortByCol(pNMListView->iSubItem);

	Invalidate();

	*pResult = 0;
}
