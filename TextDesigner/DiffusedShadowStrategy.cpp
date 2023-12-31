﻿/*
Text Designer Outline Text Library 0.3.0

Copyright (c) 2009 Wong Shao Voon

The Code Project Open License (CPOL)
http://www.codeproject.com/info/cpol10.aspx
*/

#include "StdAfx.h"
#include "DiffusedShadowStrategy.h"
#include "GDIPath.h"

using namespace TextDesigner;

DiffusedShadowStrategy::DiffusedShadowStrategy(void)
:
m_nThickness(8),
m_bOutlinetext(false),
m_pbrushText(NULL),
m_bClrText(true)
{
}

DiffusedShadowStrategy::~DiffusedShadowStrategy(void)
{
}

void DiffusedShadowStrategy::Init(
	Gdiplus::Color clrText, 
	Gdiplus::Color clrOutline, 
	int nThickness,
	bool bOutlinetext)
{
	m_clrText = clrText; 
	m_bClrText = true;
	m_clrOutline = clrOutline; 
	m_nThickness = nThickness; 
	m_bOutlinetext = bOutlinetext;
}

void DiffusedShadowStrategy::Init(
	Gdiplus::Brush* pbrushText, 
	Gdiplus::Color clrOutline, 
	int nThickness,
	bool bOutlinetext)
{
	m_pbrushText = pbrushText; 
	m_bClrText = false;
	m_clrOutline = clrOutline; 
	m_nThickness = nThickness; 
	m_bOutlinetext = bOutlinetext;
}

bool DiffusedShadowStrategy::DrawString(
	Gdiplus::Graphics* pGraphics, 
	Gdiplus::FontFamily* pFontFamily,
	Gdiplus::FontStyle fontStyle,
	int nfontSize,
	const wchar_t*pszText, 
	Gdiplus::Point ptDraw, 
	Gdiplus::StringFormat* pStrFormat)
{
	using namespace Gdiplus;
	GraphicsPath path;
	Status status = path.AddString(pszText,wcslen(pszText),pFontFamily,fontStyle,nfontSize,ptDraw,pStrFormat);
	if(status!=Ok)
		return false;

	for(int i=1; i<=m_nThickness; ++i)
	{
		Pen pen(m_clrOutline,i);
		pen.SetLineJoin(LineJoinRound);
		pGraphics->DrawPath(&pen, &path);
	}
	
	Status status2 = Ok;
	if(m_bOutlinetext==false)
	{
		for(int i=1; i<=m_nThickness; ++i)
		{
			SolidBrush brush(m_clrText);
			status2 = pGraphics->FillPath(&brush, &path);
		}
	}
	else
	{
		SolidBrush brush(m_clrText);
		status2 = pGraphics->FillPath(&brush, &path);
	}

	return status2 == Ok;
}

bool DiffusedShadowStrategy::DrawString(
	Gdiplus::Graphics* pGraphics, 
	Gdiplus::FontFamily* pFontFamily,
	Gdiplus::FontStyle fontStyle,
	int nfontSize,
	const wchar_t*pszText, 
	Gdiplus::Rect rtDraw, 
	Gdiplus::StringFormat* pStrFormat)
{
	using namespace Gdiplus;
	GraphicsPath path;
	Status status = path.AddString(pszText,wcslen(pszText),pFontFamily,fontStyle,nfontSize,rtDraw,pStrFormat);
	if(status!=Ok)
		return false;

	for(int i=1; i<=m_nThickness; ++i)
	{
		Pen pen(m_clrOutline,i);
		pen.SetLineJoin(LineJoinRound);
		pGraphics->DrawPath(&pen, &path);
	}

	Status status2 = Ok;
	if(m_bOutlinetext==false)
	{
		for(int i=1; i<=m_nThickness; ++i)
		{
			SolidBrush brush(m_clrText);
			status2 = pGraphics->FillPath(&brush, &path);
		}
	}
	else
	{
		SolidBrush brush(m_clrText);
		status2 = pGraphics->FillPath(&brush, &path);
	}

	return status2 == Ok;
}

bool DiffusedShadowStrategy::GdiDrawString(
	Gdiplus::Graphics* pGraphics, 
	LOGFONTW* pLogFont,
	const wchar_t*pszText, 
	Gdiplus::Point ptDraw)
{
	using namespace Gdiplus;
	Gdiplus::GraphicsPath* pPath=NULL;
	bool b = GDIPath::GetStringPath(
		pGraphics, 
		&pPath, 
		pszText, 
		pLogFont,
		ptDraw);

	if(false==b)
	{
		if(pPath)
		{
			delete pPath;
			pPath = NULL;
		}
		return false;
	}

	for(int i=1; i<=m_nThickness; ++i)
	{
		Pen pen(m_clrOutline,i);
		pen.SetLineJoin(LineJoinRound);
		pGraphics->DrawPath(&pen, pPath);
	}

	Status status2 = Ok;
	if(m_bOutlinetext==false)
	{
		for(int i=1; i<=m_nThickness; ++i)
		{
			SolidBrush brush(m_clrText);
			status2 = pGraphics->FillPath(&brush, pPath);
		}
	}
	else
	{
		SolidBrush brush(m_clrText);
		status2 = pGraphics->FillPath(&brush, pPath);
	}

	if(pPath)
	{
		delete pPath;
		pPath = NULL;
	}
	return status2 == Ok;
}

bool DiffusedShadowStrategy::GdiDrawString(
	Gdiplus::Graphics* pGraphics, 
	LOGFONTW* pLogFont,
	const wchar_t*pszText, 
	Gdiplus::Rect rtDraw)
{
	using namespace Gdiplus;
	Gdiplus::GraphicsPath* pPath=NULL;
	bool b = GDIPath::GetStringPath(
		pGraphics, 
		&pPath, 
		pszText, 
		pLogFont,
		rtDraw);

	if(false==b)
	{
		if(pPath)
		{
			delete pPath;
			pPath = NULL;
		}
		return false;
	}

	for(int i=1; i<=m_nThickness; ++i)
	{
		Pen pen(m_clrOutline,i);
		pen.SetLineJoin(LineJoinRound);
		pGraphics->DrawPath(&pen, pPath);
	}

	Status status2 = Ok;
	if(m_bOutlinetext==false)
	{
		for(int i=1; i<=m_nThickness; ++i)
		{
			SolidBrush brush(m_clrText);
			status2 = pGraphics->FillPath(&brush, pPath);
		}
	}
	else
	{
		SolidBrush brush(m_clrText);
		status2 = pGraphics->FillPath(&brush, pPath);
	}

	if(pPath)
	{
		delete pPath;
		pPath = NULL;
	}
	return status2 == Ok;
}

bool DiffusedShadowStrategy::MeasureString(
	Gdiplus::Graphics* pGraphics, 
	Gdiplus::FontFamily* pFontFamily,
	Gdiplus::FontStyle fontStyle,
	int nfontSize,
	const wchar_t*pszText, 
	Gdiplus::Point ptDraw, 
	Gdiplus::StringFormat* pStrFormat,
	float* pfDestWidth,
	float* pfDestHeight )
{
	using namespace Gdiplus;
	GraphicsPath path;
	Status status = path.AddString(pszText,wcslen(pszText),pFontFamily,fontStyle,nfontSize,ptDraw,pStrFormat);
	if(status!=Ok)
		return false;

	*pfDestWidth= ptDraw.X;
	*pfDestHeight= ptDraw.Y;
	bool b = GDIPath::MeasureGraphicsPath(pGraphics, &path, pfDestWidth, pfDestHeight);

	if(false==b)
		return false;

	float pixelThick = 0.0f;
	b = GDIPath::ConvertToPixels(pGraphics,m_nThickness,0.0f,&pixelThick,NULL);

	if(false==b)
		return false;

	*pfDestWidth += pixelThick;
	*pfDestHeight += pixelThick;

	return true;
}

bool DiffusedShadowStrategy::MeasureString(
	Gdiplus::Graphics* pGraphics, 
	Gdiplus::FontFamily* pFontFamily,
	Gdiplus::FontStyle fontStyle,
	int nfontSize,
	const wchar_t*pszText, 
	Gdiplus::Rect rtDraw,
	Gdiplus::StringFormat* pStrFormat,
	float* pfDestWidth,
	float* pfDestHeight )
{
	using namespace Gdiplus;
	GraphicsPath path;
	Status status = path.AddString(pszText,wcslen(pszText),pFontFamily,fontStyle,nfontSize,rtDraw,pStrFormat);
	if(status!=Ok)
		return false;

	*pfDestWidth= rtDraw.GetLeft();
	*pfDestHeight= rtDraw.GetTop();
	bool b = GDIPath::MeasureGraphicsPath(pGraphics, &path, pfDestWidth, pfDestHeight);

	if(false==b)
		return false;

	float pixelThick = 0.0f;
	b = GDIPath::ConvertToPixels(pGraphics,m_nThickness,0.0f,&pixelThick,NULL);

	if(false==b)
		return false;

	*pfDestWidth += pixelThick;
	*pfDestHeight += pixelThick;

	return true;
}

bool DiffusedShadowStrategy::GdiMeasureString(
	Gdiplus::Graphics* pGraphics, 
	LOGFONTW* pLogFont,
	const wchar_t*pszText, 
	Gdiplus::Point ptDraw,
	float* pfDestWidth,
	float* pfDestHeight )
{
	using namespace Gdiplus;
	Gdiplus::GraphicsPath* pPath=NULL;
	bool b = GDIPath::GetStringPath(
		pGraphics, 
		&pPath, 
		pszText, 
		pLogFont,
		ptDraw);

	if(false==b)
	{
		if(pPath)
		{
			delete pPath;
			pPath = NULL;
		}
		return false;
	}

	*pfDestWidth= ptDraw.X;
	*pfDestHeight= ptDraw.Y;
	b = GDIPath::MeasureGraphicsPath(pGraphics, pPath, pfDestWidth, pfDestHeight);

	if(false==b)
	{
		if(pPath)
		{
			delete pPath;
			pPath = NULL;
		}
		return false;
	}

	float pixelThick = 0.0f;
	b = GDIPath::ConvertToPixels(pGraphics,m_nThickness,0.0f,&pixelThick,NULL);

	if(false==b)
	{
		if(pPath)
		{
			delete pPath;
			pPath = NULL;
		}
		return false;
	}

	*pfDestWidth += pixelThick;
	*pfDestHeight += pixelThick;

	if(pPath)
	{
		delete pPath;
		pPath = NULL;
	}
	return true;
}

bool DiffusedShadowStrategy::GdiMeasureString(
	Gdiplus::Graphics* pGraphics, 
	LOGFONTW* pLogFont,
	const wchar_t*pszText, 
	Gdiplus::Rect rtDraw,
	float* pfDestWidth,
	float* pfDestHeight )
{
	using namespace Gdiplus;
	Gdiplus::GraphicsPath* pPath=NULL;
	bool b = GDIPath::GetStringPath(
		pGraphics, 
		&pPath, 
		pszText, 
		pLogFont,
		rtDraw);

	if(false==b)
	{
		if(pPath)
		{
			delete pPath;
			pPath = NULL;
		}
		return false;
	}

	*pfDestWidth= rtDraw.GetLeft();
	*pfDestHeight= rtDraw.GetTop();
	b = GDIPath::MeasureGraphicsPath(pGraphics, pPath, pfDestWidth, pfDestHeight);

	if(false==b)
	{
		if(pPath)
		{
			delete pPath;
			pPath = NULL;
		}
		return false;
	}

	float pixelThick = 0.0f;
	b = GDIPath::ConvertToPixels(pGraphics,m_nThickness,0.0f,&pixelThick,NULL);

	if(false==b)
	{
		if(pPath)
		{
			delete pPath;
			pPath = NULL;
		}
		return false;
	}

	*pfDestWidth += pixelThick;
	*pfDestHeight += pixelThick;

	if(pPath)
	{
		delete pPath;
		pPath = NULL;
	}
	return true;
}

inline UINT DiffusedShadowStrategy::Alphablend(UINT dest, UINT source, BYTE nAlpha)
{
	if( 0 == nAlpha )
		return dest;

	if( 255 == nAlpha )
		return source;

	BYTE nInvAlpha = ~nAlpha;

	BYTE nSrcRed   = (source & 0xff0000) >> 16; 
	BYTE nSrcGreen = (source & 0xff00) >> 8; 
	BYTE nSrcBlue  = (source & 0xff); 

	BYTE nDestRed   = (dest & 0xff0000) >> 16; 
	BYTE nDestGreen = (dest & 0xff00) >> 8; 
	BYTE nDestBlue  = (dest & 0xff); 

	BYTE nRed  = ( nSrcRed   * nAlpha + nDestRed * nInvAlpha   )>>8;
	BYTE nGreen= ( nSrcGreen * nAlpha + nDestGreen * nInvAlpha )>>8;
	BYTE nBlue = ( nSrcBlue  * nAlpha + nDestBlue * nInvAlpha  )>>8;

	return 0xff000000 | nRed << 16 | nGreen << 8 | nBlue;
}

bool DiffusedShadowStrategy::GdiMeasureStringRealHeight(
	Gdiplus::Graphics* pGraphics, 
	LOGFONTW* pLogFont,
	const wchar_t*pszText, 
	Gdiplus::Point ptDraw,
	float* pfDestWidth,
	float* pfDestHeight )
{
	using namespace Gdiplus;
	Gdiplus::GraphicsPath* pPath=NULL;
	bool b = GDIPath::GetStringPath(
		pGraphics, 
		&pPath, 
		pszText, 
		pLogFont,
		ptDraw);

	if(false==b)
	{
		if(pPath)
		{
			delete pPath;
			pPath = NULL;
		}
		return false;
	}

	*pfDestWidth= ptDraw.X;
	*pfDestHeight= ptDraw.Y;
	b = GDIPath::MeasureGraphicsPathRealHeight(pGraphics, pPath, pfDestWidth, pfDestHeight);

	if(false==b)
	{
		if(pPath)
		{
			delete pPath;
			pPath = NULL;
		}
		return false;
	}

	float pixelThick = 0.0f;
	b = GDIPath::ConvertToPixels(pGraphics,m_nThickness,0.0f,&pixelThick,NULL);

	if(false==b)
	{
		if(pPath)
		{
			delete pPath;
			pPath = NULL;
		}
		return false;
	}

	*pfDestWidth += pixelThick;
	*pfDestHeight += pixelThick;

	if(pPath)
	{
		delete pPath;
		pPath = NULL;
	}
	return true;
}

bool DiffusedShadowStrategy::GdiMeasureStringRealHeight(
	Gdiplus::Graphics* pGraphics, 
	LOGFONTW* pLogFont,
	const wchar_t*pszText, 
	Gdiplus::Rect rtDraw,
	float* pfDestWidth,
	float* pfDestHeight )
{
	using namespace Gdiplus;
	Gdiplus::GraphicsPath* pPath=NULL;
	bool b = GDIPath::GetStringPath(
		pGraphics, 
		&pPath, 
		pszText, 
		pLogFont,
		rtDraw);

	if(false==b)
	{
		if(pPath)
		{
			delete pPath;
			pPath = NULL;
		}
		return false;
	}

	*pfDestWidth= rtDraw.GetLeft();
	*pfDestHeight= rtDraw.GetTop();
	b = GDIPath::MeasureGraphicsPathRealHeight(pGraphics, pPath, pfDestWidth, pfDestHeight);

	if(false==b)
	{
		if(pPath)
		{
			delete pPath;
			pPath = NULL;
		}
		return false;
	}

	float pixelThick = 0.0f;
	b = GDIPath::ConvertToPixels(pGraphics,m_nThickness,0.0f,&pixelThick,NULL);

	if(false==b)
	{
		if(pPath)
		{
			delete pPath;
			pPath = NULL;
		}
		return false;
	}

	*pfDestWidth += pixelThick;
	*pfDestHeight += pixelThick;

	if(pPath)
	{
		delete pPath;
		pPath = NULL;
	}
	return true;
}