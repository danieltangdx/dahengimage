//CAutoFont class implementation
#include "stdafx.h"
#include "AutoFont.h"

CAutoFont::CAutoFont()
{
	lf.lfHeight=16;
	lf.lfWidth=0;
	lf.lfEscapement=0;
	lf.lfOrientation=0;
	lf.lfWeight=FW_NORMAL;
	lf.lfItalic=0;
	lf.lfUnderline=0;
	lf.lfStrikeOut=0;
	lf.lfCharSet=ANSI_CHARSET;
	lf.lfOutPrecision=OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision=CLIP_DEFAULT_PRECIS;
	lf.lfQuality=PROOF_QUALITY;
	lf.lfPitchAndFamily=VARIABLE_PITCH | FF_ROMAN;
	strcpy(lf.lfFaceName, "Times New Roman");

	CreateFontIndirect(&lf);
}

CAutoFont::CAutoFont(CString facename)
{
	lf.lfHeight=16;
	lf.lfWidth=0;
	lf.lfEscapement=0;
	lf.lfOrientation=0;
	lf.lfWeight=FW_NORMAL;
	lf.lfItalic=0;
	lf.lfUnderline=0;
	lf.lfStrikeOut=0;
	lf.lfCharSet=ANSI_CHARSET;
	lf.lfOutPrecision=OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision=CLIP_DEFAULT_PRECIS;
	lf.lfQuality=PROOF_QUALITY;
	lf.lfPitchAndFamily=VARIABLE_PITCH | FF_ROMAN;
	strcpy(lf.lfFaceName, (LPCTSTR)facename);

	CreateFontIndirect(&lf);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Function Header
CAutoFont& CAutoFont::operator =(CAutoFont &rhs)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
	GetLogFont(&rhs.lf);	
	SetColour(rhs.GetColour());
	return *this;
}

CAutoFont::CAutoFont(LOGFONT& logfont)
{
	lf=logfont;
	CreateFontIndirect(&lf);
	SetColour(0,0,0);
}

CAutoFont::CAutoFont(CFont font)
{
	HFONT hFont=(HFONT)font;
	Attach((HFONT)hFont);
	GetLogFont(&lf);
	SetColour(0,0,0);
}

CAutoFont::~CAutoFont()
{
}

LONG CAutoFont::SetHeight(LONG height)
{
	LONG l=lf.lfHeight;

	DeleteObject();
	lf.lfHeight=height;
	CreateFontIndirect(&lf);

	return l;
}

LONG CAutoFont::SetWidth(LONG width)
{
	LONG l=lf.lfWidth;

	DeleteObject();
	lf.lfWidth=width;
	CreateFontIndirect(&lf);

	return l;
}

LONG CAutoFont::SetEscapement(LONG esc)
{
	LONG l=lf.lfEscapement;

	DeleteObject();
	lf.lfEscapement=esc;
	CreateFontIndirect(&lf);

	return l;
}

LONG CAutoFont::SetOrientation(LONG or)
{
	LONG l=lf.lfOrientation;

	DeleteObject();
	lf.lfOrientation=or;
	CreateFontIndirect(&lf);

	return l;
}

LONG CAutoFont::SetWeight(LONG weight)
{
	LONG l=lf.lfWeight;

	DeleteObject();
	lf.lfWeight=weight;
	CreateFontIndirect(&lf);

	return l;
}

BYTE CAutoFont::SetCharset(BYTE charset)
{
	BYTE b=lf.lfCharSet;

	DeleteObject();
	lf.lfCharSet=charset;
	CreateFontIndirect(&lf);

	return b;
}

BYTE CAutoFont::SetOutPrecision(BYTE op)
{
	BYTE b=lf.lfOutPrecision;

	DeleteObject();
	lf.lfOutPrecision=op;
	CreateFontIndirect(&lf);
	
	return b;
}

BYTE CAutoFont::SetClipPrecision(BYTE cp)
{
	BYTE b=lf.lfClipPrecision;

	DeleteObject();
	lf.lfClipPrecision=cp;
	CreateFontIndirect(&lf);

	return b;
}

BYTE CAutoFont::SetQuality(BYTE qual)
{
	BYTE b=lf.lfQuality;

	DeleteObject();
	lf.lfQuality=qual;
	CreateFontIndirect(&lf);

	return b;
}

BYTE CAutoFont::SetPitchAndFamily(BYTE paf)
{
	BYTE b=lf.lfPitchAndFamily;

	DeleteObject();
	lf.lfPitchAndFamily=paf;
	CreateFontIndirect(&lf);

	return b;
}

CString CAutoFont::SetFaceName(CString facename)
{
	CString str=lf.lfFaceName;

	DeleteObject();
	strcpy(lf.lfFaceName, (LPCTSTR)facename);
	CreateFontIndirect(&lf);

	return str;
}

LPCTSTR CAutoFont::SetFaceName(LPCTSTR facename)
{
	LPCTSTR str=lf.lfFaceName;

	DeleteObject();
	strcpy(lf.lfFaceName, facename);
	CreateFontIndirect(&lf);

	return str;
}

BOOL CAutoFont::SetBold(BOOL B)
{
	BOOL b;

	if (B)
		b=SetWeight(FW_BOLD);
	else
		b=SetWeight(FW_NORMAL);

	if (b >= FW_MEDIUM)
		return TRUE;
	else
		return FALSE;
}

BOOL CAutoFont::SetItalic(BOOL i)
{
	BOOL b=(BOOL)lf.lfItalic;
    if(b!=i)
	{
		DeleteObject();
		lf.lfItalic=(BYTE)i;
		CreateFontIndirect(&lf);
	}


	return b;
}

BOOL CAutoFont::SetUnderline(BOOL u)
{
	BOOL b=(BOOL)lf.lfUnderline;
    if(b!=u)
	{
		DeleteObject();
		lf.lfUnderline=(BYTE)u;
		CreateFontIndirect(&lf);
	}


	return b;
}

BOOL CAutoFont::SetStrikeOut(BOOL s)
{
	BOOL b=(BOOL)lf.lfStrikeOut;

	DeleteObject();
	lf.lfStrikeOut=(BYTE)s;
	CreateFontIndirect(&lf);

	return b;
}

void CAutoFont::SetLogFont(LOGFONT& logfont)
{
	lf=logfont;
	DeleteObject();
	CreateFontIndirect(&lf);
}

LONG CAutoFont::GetHeight()
{
	return lf.lfHeight;
}

LONG CAutoFont::GetWidth()
{
	return lf.lfWidth;
}

LONG CAutoFont::GetEscapement()
{
	return lf.lfEscapement;
}

LONG CAutoFont::GetOrientation()
{
	return lf.lfEscapement;
}

LONG CAutoFont::GetWeight()
{
	return lf.lfWeight;
}

BYTE CAutoFont::GetCharset()
{
	return lf.lfCharSet;
}

BYTE CAutoFont::GetOutPrecision()
{
	return lf.lfOutPrecision;
}

BYTE CAutoFont::GetClipPrecision()
{
	return lf.lfClipPrecision;
}

BYTE CAutoFont::GetQuality()
{
	return lf.lfQuality;
}

BYTE CAutoFont::GetPitchAndFamily()
{
	return lf.lfPitchAndFamily;
}

LPCTSTR CAutoFont::GetFaceName()
{
	return lf.lfFaceName;
}

BOOL CAutoFont::GetBold()
{
	return lf.lfWeight >= FW_MEDIUM ? TRUE : FALSE;
}

BOOL CAutoFont::GetItalic()
{
	return (BOOL)lf.lfItalic;
}

BOOL CAutoFont::GetUnderline()
{
	return (BOOL)lf.lfUnderline;
}

BOOL CAutoFont::GetStrikeOut()
{
	return (BOOL)lf.lfStrikeOut;
}

CString CAutoFont::ContractFont()
{
	CString str;

	str.Format("%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%s",
		lf.lfHeight,
		lf.lfWidth,
		lf.lfEscapement,
		lf.lfOrientation,
		lf.lfWeight,
		lf.lfItalic,
		lf.lfUnderline,
		lf.lfStrikeOut,
		lf.lfCharSet,
		lf.lfOutPrecision,
		lf.lfClipPrecision,
		lf.lfQuality,
		lf.lfPitchAndFamily,
		GetRValue(m_crColour),
		GetGValue(m_crColour),
		GetBValue(m_crColour),
		lf.lfFaceName);

	return str;
}

void CAutoFont::ExtractFont(CString& str)
{
	lf.lfHeight=atol((LPCTSTR)GetToken(str, ","));
	lf.lfWidth=atol((LPCTSTR)GetToken(str, ","));
	lf.lfEscapement=atol((LPCTSTR)GetToken(str, ","));
	lf.lfOrientation=atol((LPCTSTR)GetToken(str, ","));
	lf.lfWeight=atol((LPCTSTR)GetToken(str, ","));
	lf.lfItalic=(BYTE)(atoi((LPCTSTR)GetToken(str, ",")));
	lf.lfUnderline=(BYTE)(atoi((LPCTSTR)GetToken(str, ",")));
	lf.lfStrikeOut=(BYTE)(atoi((LPCTSTR)GetToken(str, ",")));
	lf.lfCharSet=(BYTE)(atoi((LPCTSTR)GetToken(str, ",")));
	lf.lfOutPrecision=(BYTE)(atoi((LPCTSTR)GetToken(str, ",")));
	lf.lfClipPrecision=(BYTE)(atoi((LPCTSTR)GetToken(str, ",")));
	lf.lfQuality=(BYTE)(atoi((LPCTSTR)GetToken(str, ",")));
	lf.lfPitchAndFamily=(BYTE)(atoi((LPCTSTR)GetToken(str, ",")));
	
	int r = atoi((LPCTSTR)GetToken(str, ","));
	int g = atoi((LPCTSTR)GetToken(str, ","));
	int b = atoi((LPCTSTR)GetToken(str, ","));
	SetColour(r,g,b);

	strcpy(lf.lfFaceName, (LPCTSTR)str);

	DeleteObject();
	CreateFontIndirect(&lf);
}

CString CAutoFont::GetToken(CString& str, LPCTSTR c)
{
	int pos;
	CString token;

	pos=str.Find(c);
	token=str.Left(pos);
	str=str.Mid(pos+1);

	return token;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Function Header
void CAutoFont::SetAntialiasQuality(int nQuality)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Undocumented feature - max quality is 4
{	lf.lfQuality = (BYTE)nQuality;	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Function Header
void CAutoFont::SetNonAntialiasQuality(int nQuality)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Undocumented feature - max quality is 3
{	lf.lfQuality = (BYTE)nQuality;	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Function Header
void CAutoFont::SetColour(COLORREF crColour)	{	m_crColour = crColour;	}
void CAutoFont::SetColour(int r, int g, int b)	{	m_crColour = RGB(r,g,b);	}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Function Header
COLORREF CAutoFont::GetColour()	{	return m_crColour;	}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Function Header
int CAutoFont::GetRedValue(){ return GetRValue(m_crColour); }
int CAutoFont::GetGreenValue(){ return GetGValue(m_crColour); }
int CAutoFont::GetBlueValue(){ return GetBValue(m_crColour); }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
