//*****************************************************************************
// Filename	: 	NTLinksBar.h
//
// Created	:	2010/11/25 - 11:27
// Modified	:	2010/11/25 - 11:27
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#pragma once


//=============================================================================
// CNTLinksBar
//=============================================================================
class CNTLinksBar : public CMFCToolBar
{
	DECLARE_DYNCREATE(CNTLinksBar)
	// Construction
public:
	CNTLinksBar ();

	virtual ~CNTLinksBar ();

	// Implementation
public:
	virtual CSize	CalcFixedLayout			(BOOL, BOOL);

	virtual int GetRowHeight () const
	{
		return 1;
	}

	void SetCurrentCommand (UINT nCommand)
	{
		m_CurrentCommand = nCommand;
	}

	UINT GetCurrentCommand () const
	{
		return m_CurrentCommand;
	}

protected:
	DECLARE_MESSAGE_MAP()

private:
	UINT m_CurrentCommand;
};