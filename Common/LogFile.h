//*****************************************************************************
// Filename	: LogFile.h
// Created	: 2010/10/14
// Modified	: 2012/1/19 - 11:15
//
// Author   : G.H Lee (���� : PiRing)
//
// Purpose	: 
//*****************************************************************************
#ifndef LogFile_h__
#define LogFile_h__

#pragma once

//=============================================================================
// CLogFile
//=============================================================================
class CLogFile
{
public:    
	CLogFile(LPCTSTR lpszLogName = NULL, LPCTSTR lpszSubDirName = NULL);
    ~CLogFile();

private:
	long		m_nThreadId;

	CFile		m_fileLog;

	//�α������� ���� �ۼ��� �α������̸��� �����ϴ��� �˻�
	BOOL		FindLogFile			();
	void		GetLogFilePath		(CString& strPath);
	CString		Logfilename			();
	void		CreateLogFolder		(CString strPath );	
	CString		GetTime				();

	// �⺻ �α� ���
	void		OnAdd_Log				(LPCTSTR lpszLog, 
									 LPCTSTR lpszSubDir = NULL,
									 LPCTSTR lpszLogName = NULL,
									 UINT	nFileDivType = 0 // �α� ���� ���� ����(none, am / pm, filesize, hour)
									);

	CString		m_strPath;
	CString		m_strLogName;
	CString		m_strSubDirName;
	BYTE		m_byFileDivType;

	BOOL		m_bUseWriteTime;

	CString		m_strFileExt;

public:

	void		SetDefPath			(LPCTSTR lpszPath)
	{
		m_strPath = lpszPath;
	};

	void		SetUseWriteTime		(BOOL bUse = TRUE)
	{
		m_bUseWriteTime = bUse;
	};

	void		LogWrite			(LPCTSTR lpszMsg);
	void		LogWriteWithHeader	(LPCTSTR lpszMsg, LPCTSTR lpszHeader = NULL);

	// Log �⺻ ���ϸ� ���� (���ϸ� + ��¥ + �ð� or ī��Ʈ)
	void		SetLogName			(LPCTSTR lpszLogName);

	void		SetSubDirName		(LPCTSTR lpszSubDirName);

	// �α� ���� ���� ���� (none, am/pm, filesize, hour)
	void		SetFileDivType		(BYTE byDivType);


	//  ��¥ ���� �������� (��, ��, ��)
	// all, ������, �����, ��������
	//void		SetUseDateSubDir	(BYTE byDateDirLimit = 0);

	// ���� ����Ǵ� �α� ������ �н��� ��ȯ
	CString		GetFilePath();

	// �߰� ���� Ȯ���� ���� (�⺻ .log)
	void		SetFileExt(LPCTSTR lpszExt);
};

#endif // LogFile_h__
