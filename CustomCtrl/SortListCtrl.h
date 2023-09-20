
class CSortClass
{
public:
	CSortClass(CListCtrl * _pWnd, const int _iCol, const bool _bIsNumeric);
	virtual ~CSortClass();

	int iCol;
	CListCtrl * pWnd;
	bool bIsNumeric;
	void Sort(const bool bAsc);

	static int CALLBACK CompareAsc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	static int CALLBACK CompareDes(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

	static int CALLBACK CompareAscI(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	static int CALLBACK CompareDesI(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

	afx_msg void	OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);

public:
	class CSortItem
	{
	public:
		virtual ~CSortItem();
		CSortItem(const DWORD_PTR _dw, const CString &_txt);
		CString txt;
		DWORD_PTR dw;
	};
	class CSortItemInt
	{
	public:
		CSortItemInt(const DWORD_PTR _dw, const CString &_txt);
		int iInt;
		DWORD_PTR dw;
	};
};