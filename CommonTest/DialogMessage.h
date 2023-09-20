
#ifndef DialogMessage_h__
#define DialogMessage_h__

#pragma once

class CVGStatic;

class CDialogMessage : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogMessage)

public:
	CDialogMessage(LPCTSTR IN_szTitle, LPCTSTR IN_szText, UINT IN_nMB_Type = MB_OK, CWnd* pParent = nullptr);   // 표준 생성자입니다.
	CDialogMessage(LPCTSTR IN_szText, UINT IN_nMB_Type = MB_OK, CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDialogMessage();

protected:
	afx_msg int		OnCreate			(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize				(UINT nType, int cx, int cy);
	afx_msg void	OnDestroy			();
	//afx_msg void	OnRangeButtonCtrl	(UINT nID);

	virtual BOOL	OnInitDialog		();
	virtual void	DoDataExchange		(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void	OnOK				();
	virtual void	OnCancel			();

	DECLARE_MESSAGE_MAP()

private:
	void CreateResource();
	void ReleaseResource();
	void StyleResource();
	void RecalcLayout();

private:

	CFont m_font;

	UINT		m_nMB_Type		= MB_OK;

	//CVGStatic*	m_pst_Background;
	CVGStatic*	m_pst_Line;

	CVGStatic*	m_pst_Title;	// 제목
	CVGStatic*	m_pst_Text;		// Comment (오류 조치 방법 등...)

	CButton*	m_pbn_Ok;
	CButton*	m_pbn_Cancel;

	CString		m_szTitle;
	CString		m_szText;

};

#endif // DialogMessage_h__