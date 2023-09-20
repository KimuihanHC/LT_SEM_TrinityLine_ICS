
#ifndef Dlg_MessageBox_h__
#define Dlg_MessageBox_h__

#pragma once

#include "VGStatic.h"

class CDlg_MessageBox : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_MessageBox)

public:
	CDlg_MessageBox(LPCTSTR IN_szTitle, LPCTSTR IN_szText, UINT IN_nMB_Type = MB_OK, CWnd* pParent = nullptr);   // 표준 생성자입니다.
	CDlg_MessageBox(LPCTSTR IN_szText, UINT IN_nMB_Type = MB_OK, CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlg_MessageBox();

protected:
	afx_msg int		OnCreate			(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize				(UINT nType, int cx, int cy);
	afx_msg void	OnDestroy			();

	virtual BOOL	OnInitDialog		();
	virtual void	OnOK				();
	virtual void	OnCancel			();

	DECLARE_MESSAGE_MAP()

private:
	void CreateResource();
	void ReleaseResource();

	CFont m_font;

	UINT		m_nMB_Type		= MB_OK;

	CVGStatic	m_st_Border;
	CVGStatic	m_st_Line;

	CVGStatic	m_st_Title;		// 제목
	CVGStatic	m_st_Text;		// 메세지
	CVGStatic	m_st_Solution;	// 해결방법

	CButton		m_bn_Ok;
	CButton		m_bn_Cancel;

	CString		m_szTitle;
	CString		m_szText;
	CString		m_szSolution;

public:
	void		Set_Solution	(__in LPCTSTR IN_szSolution);

};

#endif // Dlg_MessageBox_h__