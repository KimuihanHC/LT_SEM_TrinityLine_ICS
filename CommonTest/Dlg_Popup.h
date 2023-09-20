#pragma once

#include "resource.h"
#include "VGStatic.h"

// CDlg_Popup 대화 상자입니다.

class CDlg_Popup : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_Popup)

public:
	CDlg_Popup(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlg_Popup();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_POPUP };

	CButton				m_bn_Cancel;
	CButton				m_bn_Ok;
	CFont				m_font_Default;
	CString				m_strMassage;
	CVGStatic			m_st_frame;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnBnClickedButtenOK();
	afx_msg void	OnBnClickedButtenCancel();

	void			PopupMessage(CString str);

};
