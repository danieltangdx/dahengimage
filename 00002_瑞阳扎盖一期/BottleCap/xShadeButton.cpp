// CxShadeButton.cpp : implementation file
/** 12/05/2001 v1.00
 * - ing.davide.pizzolato@libero.it
 ** 23/05/2001 v1.10
 * - added text shift on button down
 * - fixed many CxDib bugs
 * - fixed SHS_HARDBUMP bug
 * - added icon support
 * - added text alignment
 * - added flat style
 ** 16/06/2001 v1.20
 * - fixed keyboard shortcut bug
 * - check box & radio button add on by Rainer@Mangold.com
 * - 2nd icon & 2nd tooltip add on by Rainer@Mangold.com
 ** 03/08/2001 v1.30
 * - fixed SetIcon bug
 * - added Font support
 ** 23/09/2001 v1.40
 * - fixed memory leakage bug in DrawItem() (by David Scambler <dave@bmm.com.au>) 
 * - fixed memory leakage bug in SetIcon() (by Orioli Alessandro <aorioli@temaweb.it>) 
 * - fixed second tooltip initialization bug (by Jesper Kinnås)
 * - added multiline tooltip support (thanks to Derek Lakin)
 * - fixed OnLButtonUp() problem with drag & drop. (by Hyeonsoo Jung <debussy2k@hanmail.net>)
 ** 28/10/2001 v1.41
 * - fixed memory leakage bug in SetIcon() and in the destructor.
 * - added 3rd icon for highlighted state.
 */

#include "stdafx.h"
#include "xShadeButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CxDib (under development) 
/////////////////////////////////////////////////////////////////////
CxDib::CxDib()
{
	hDib=NULL;
	memset(&m_bi,0,sizeof(BITMAPINFOHEADER));
    m_bi.biSize = sizeof(BITMAPINFOHEADER);

	m_LineWidth=0;
	m_nColors=0;
}
/////////////////////////////////////////////////////////////////////
CxDib::~CxDib()
{
	if (hDib) free(hDib);
}
/////////////////////////////////////////////////////////////////////
bool CxDib::IsWin30Dib(){
	return ((*(LPDWORD)(hDib))==sizeof(BITMAPINFOHEADER));
}
/////////////////////////////////////////////////////////////////////
WORD CxDib::GetPaletteSize()
{
//    if (IsWin30Dib())
        return (m_nColors * sizeof(RGBQUAD));
//    else return (m_nColors * sizeof(RGBTRIPLE));
}
/////////////////////////////////////////////////////////////////////
BYTE* CxDib::GetBits()
{ 
	if (hDib)	return ((BYTE*)hDib + *(LPDWORD)hDib + GetPaletteSize()); 
	return NULL;
}
/////////////////////////////////////////////////////////////////////
HDIB  CxDib::Create(DWORD dwWidth, DWORD dwHeight, WORD wBitCount)
{
    LPBITMAPINFOHEADER  lpbi;	// pointer to BITMAPINFOHEADER
    DWORD               dwLen;	// size of memory block

	if (hDib) free(hDib);
	hDib=NULL;

    // Make sure bits per pixel is valid
    if (wBitCount <= 1)			wBitCount = 1;
    else if (wBitCount <= 4)	wBitCount = 4;
    else if (wBitCount <= 8)	wBitCount = 8;
    else				        wBitCount = 24;

    switch (wBitCount){
        case 1:
            m_nColors = 2;
			break;
        case 4:
            m_nColors = 16;
			break;
        case 8:
            m_nColors = 256;
			break;
        default:
            m_nColors = 0;
    }

    m_LineWidth = WIDTHBYTES(wBitCount * dwWidth);

    // initialize BITMAPINFOHEADER
    m_bi.biSize = sizeof(BITMAPINFOHEADER);
    m_bi.biWidth = dwWidth;         // fill in width from parameter
    m_bi.biHeight = dwHeight;       // fill in height from parameter
    m_bi.biPlanes = 1;              // must be 1
    m_bi.biBitCount = wBitCount;    // from parameter
    m_bi.biCompression = BI_RGB;    
    m_bi.biSizeImage = m_LineWidth * dwHeight;
    m_bi.biXPelsPerMeter = 0;
    m_bi.biYPelsPerMeter = 0;
    m_bi.biClrUsed = 0;
    m_bi.biClrImportant = 0;

    // calculate size of memory block required to store the DIB.  This
    // block should be big enough to hold the BITMAPINFOHEADER, the color
    // table, and the bits
    dwLen = GetSize();

	//hDib = malloc(dwLen); // alloc memory block to store our bitmap
	hDib = new (HDIB[dwLen]); //fixes allocation problem under Win2k
    if (!hDib) return NULL;

    // use our bitmap info structure to fill in first part of
    // our DIB with the BITMAPINFOHEADER
	lpbi = (LPBITMAPINFOHEADER)(hDib);
    *lpbi = m_bi;

    return hDib; //return handle to the DIB
}
/////////////////////////////////////////////////////////////////////
long CxDib::Draw(HDC pDC, long xoffset, long yoffset)
{
	if((hDib)&&(pDC))  {
		//palette must be correctly filled
		LPSTR lpDIB = (char*)hDib;	//set image to hdc...
		SetStretchBltMode(pDC,COLORONCOLOR);	
		SetDIBitsToDevice(pDC, xoffset, yoffset,
			m_bi.biWidth, m_bi.biHeight, 0, 0, 0,
			m_bi.biHeight, GetBits(),
			(BITMAPINFO*)lpDIB, DIB_RGB_COLORS);
		return 1;
	} 
	return 0;
}
/////////////////////////////////////////////////////////////////////
long CxDib::Stretch(HDC pDC, long xoffset, long yoffset, long xsize, long ysize)
{
	if((hDib)&&(pDC)) {
		//palette must be correctly filled
		LPSTR lpDIB = (char*)hDib;	//set image to hdc...
		SetStretchBltMode(pDC,COLORONCOLOR);	
		StretchDIBits(pDC, xoffset, yoffset,
					xsize, ysize, 0, 0, m_bi.biWidth, m_bi.biHeight,
					GetBits(),(BITMAPINFO*)lpDIB,DIB_RGB_COLORS,SRCCOPY);
		return 1;
	}
	return 0;
}
/////////////////////////////////////////////////////////////////////
void CxDib::SetPaletteIndex(BYTE idx, BYTE r, BYTE g, BYTE b)
{
	if ((hDib)&&(m_nColors)){
		BYTE* iDst = (BYTE*)(hDib) + sizeof(BITMAPINFOHEADER);
		if ((idx>=0)&&(idx<m_nColors)){	
			long ldx=idx*sizeof(RGBQUAD);
			iDst[ldx++] = (BYTE) b;
			iDst[ldx++] = (BYTE) g;
			iDst[ldx++] = (BYTE) r;
			iDst[ldx] = (BYTE) 0;
		}
	}
}
/////////////////////////////////////////////////////////////////////
void CxDib::SetPaletteIndex(BYTE idx, RGBQUAD c)
{
	if ((hDib)&&(m_nColors)){
		BYTE* iDst = (BYTE*)(hDib) + sizeof(BITMAPINFOHEADER);
		if ((idx>=0)&&(idx<m_nColors)){	
			long ldx=idx*sizeof(RGBQUAD);
			iDst[ldx++] = (BYTE) c.rgbBlue;
			iDst[ldx++] = (BYTE) c.rgbGreen;
			iDst[ldx++] = (BYTE) c.rgbRed;
			iDst[ldx] = (BYTE) 0;
		}
	}
}
/////////////////////////////////////////////////////////////////////
void CxDib::SetPaletteIndex(BYTE idx, COLORREF cr)
{
	if ((hDib)&&(m_nColors)){
		BYTE* iDst = (BYTE*)(hDib) + sizeof(BITMAPINFOHEADER);
		if ((idx>=0)&&(idx<m_nColors)){	
			long ldx=idx*sizeof(RGBQUAD);
			iDst[ldx++] = (BYTE) GetBValue(cr);
			iDst[ldx++] = (BYTE) GetGValue(cr);
			iDst[ldx++] = (BYTE) GetRValue(cr);
			iDst[ldx] = (BYTE) 0;
		}
	}
}
/////////////////////////////////////////////////////////////////////
RGBQUAD CxDib::GetPaletteIndex(BYTE idx)
{
	RGBQUAD rgb = {0,0,0,0};
	if ((hDib)&&(m_nColors)){
		BYTE* iDst = (BYTE*)(hDib) + sizeof(BITMAPINFOHEADER);
		if ((idx>=0)&&(idx<m_nColors)){	
			long ldx=idx*sizeof(RGBQUAD);
			rgb.rgbBlue = iDst[ldx++];
			rgb.rgbGreen=iDst[ldx++];
			rgb.rgbRed =iDst[ldx];
			rgb.rgbReserved = 0;
		}
	}
	return rgb;
}
/////////////////////////////////////////////////////////////////////
BYTE CxDib::GetPixelIndex(long x,long y)
{
	if ((hDib==NULL)||(m_nColors==0)||
		(x<0)||(y<0)||(x>=m_bi.biWidth)||(y>=m_bi.biHeight)) return 0;
	BYTE* iDst = GetBits();
	return iDst[(m_bi.biHeight - y - 1)*m_LineWidth + x];
}
/////////////////////////////////////////////////////////////////////
RGBQUAD CxDib::GetPixelColor(long x,long y)
{
	RGBQUAD rgb={0,0,0,0};
	if ((hDib==NULL)||(x<0)||(y<0)||
		(x>=m_bi.biWidth)||(y>=m_bi.biHeight)) return rgb;
	if (m_nColors) return GetPaletteIndex(GetPixelIndex(x,y));
	else {
		BYTE* iDst = GetBits()+(m_bi.biHeight - y - 1)*m_LineWidth + x*sizeof(RGBQUAD);
		rgb.rgbBlue = *iDst++;
		rgb.rgbGreen= *iDst++;
		rgb.rgbRed =*iDst;
		return rgb;
	}
}
/////////////////////////////////////////////////////////////////////
void CxDib::SetPixelIndex(long x,long y,BYTE i)
{
	if ((hDib==NULL)||(m_nColors==0)||
		(x<0)||(y<0)||(x>=m_bi.biWidth)||(y>=m_bi.biHeight)) return ;
	BYTE* iDst = GetBits();
	iDst[(m_bi.biHeight - y - 1)*m_LineWidth + x]=i;
}
/////////////////////////////////////////////////////////////////////
void CxDib::SetPixelColor(long x,long y,COLORREF cr)
{
	SetPixelColor(x,y,RGB2RGBQUAD(cr));
}
/////////////////////////////////////////////////////////////////////
void CxDib::SetPixelColor(long x,long y,RGBQUAD c)
{
	if ((hDib==NULL)||(x<0)||(y<0)||
		(x>=m_bi.biWidth)||(y>=m_bi.biHeight)) return;
	if (m_nColors)
		SetPixelIndex(x,y,GetNearestIndex(c));
	else {
		BYTE* iDst = GetBits()+(m_bi.biHeight - y - 1)*m_LineWidth + x*sizeof(RGBQUAD);
		*(RGBQUAD*)iDst = c;
	}
}
/////////////////////////////////////////////////////////////////////
BYTE CxDib::GetNearestIndex(RGBQUAD c)
{
	if ((hDib==NULL)||(m_nColors==0)) return 0;
	BYTE* iDst = (BYTE*)(hDib) + sizeof(BITMAPINFOHEADER);
	long distance=200000;
	BYTE i,j;
	long k,l;
	for(i=0,l=0;i<m_nColors;i++,l+=sizeof(RGBQUAD)){
		k = (iDst[l]-c.rgbBlue)*(iDst[l]-c.rgbBlue)+
			(iDst[l+1]-c.rgbGreen)*(iDst[l+1]-c.rgbGreen)+
			(iDst[l+2]-c.rgbRed)*(iDst[l+2]-c.rgbRed);
		if (k==0){
			j=i;
			break;
		}
		if (k<distance){
			distance=k;
			j=i;
		}
	}
	return j;
}
/////////////////////////////////////////////////////////////////////
#define  HSLMAX   240	/* H,L, and S vary over 0-HSLMAX */
#define  RGBMAX   255   /* R,G, and B vary over 0-RGBMAX */
                        /* HSLMAX BEST IF DIVISIBLE BY 6 */
                        /* RGBMAX, HSLMAX must each fit in a byte. */
/* Hue is undefined if Saturation is 0 (grey-scale) */
/* This value determines where the Hue scrollbar is */
/* initially set for achromatic colors */
#define UNDEFINED (HSLMAX*2/3)
/////////////////////////////////////////////////////////////////////
RGBQUAD CxDib::RGBtoHSL(RGBQUAD lRGBColor)
{
	BYTE R,G,B;					/* input RGB values */
	BYTE H,L,S;					/* output HSL values */
	BYTE cMax,cMin;				/* max and min RGB values */
	WORD Rdelta,Gdelta,Bdelta;	/* intermediate value: % of spread from max*/

	R = lRGBColor.rgbRed;	/* get R, G, and B out of DWORD */
	G = lRGBColor.rgbGreen;
	B = lRGBColor.rgbBlue;

	cMax = max( max(R,G), B);	/* calculate lightness */
	cMin = min( min(R,G), B);
	L = (((cMax+cMin)*HSLMAX)+RGBMAX)/(2*RGBMAX);

	if (cMax==cMin){			/* r=g=b --> achromatic case */
		S = 0;					/* saturation */
		H = UNDEFINED;			/* hue */
	} else {					/* chromatic case */
		if (L <= (HSLMAX/2))	/* saturation */
			S = (((cMax-cMin)*HSLMAX)+((cMax+cMin)/2))/(cMax+cMin);
		else
			S = (((cMax-cMin)*HSLMAX)+((2*RGBMAX-cMax-cMin)/2))/(2*RGBMAX-cMax-cMin);
		/* hue */
		Rdelta = ( ((cMax-R)*(HSLMAX/6)) + ((cMax-cMin)/2) ) / (cMax-cMin);
		Gdelta = ( ((cMax-G)*(HSLMAX/6)) + ((cMax-cMin)/2) ) / (cMax-cMin);
		Bdelta = ( ((cMax-B)*(HSLMAX/6)) + ((cMax-cMin)/2) ) / (cMax-cMin);

		if (R == cMax)
			H = Bdelta - Gdelta;
		else if (G == cMax)
			H = (HSLMAX/3) + Rdelta - Bdelta;
		else /* B == cMax */
			H = ((2*HSLMAX)/3) + Gdelta - Rdelta;

		if (H < 0) H += HSLMAX;
		if (H > HSLMAX) H -= HSLMAX;
	}
	RGBQUAD hsl={L,S,H,0};
	return hsl;
}
/////////////////////////////////////////////////////////////////////
WORD CxDib::HueToRGB(WORD n1,WORD n2,WORD hue)
{ 
	/* range check: note values passed add/subtract thirds of range */
	if (hue < 0) hue += HSLMAX;
	if (hue > HSLMAX) hue -= HSLMAX;

	/* return r,g, or b value from this tridrant */
	if (hue < (HSLMAX/6))
		return ( n1 + (((n2-n1)*hue+(HSLMAX/12))/(HSLMAX/6)) );
	if (hue < (HSLMAX/2))
		return ( n2 );
	if (hue < ((HSLMAX*2)/3))
		return ( n1 + (((n2-n1)*(((HSLMAX*2)/3)-hue)+(HSLMAX/12))/(HSLMAX/6))); 
	else
		return ( n1 );
} 
/////////////////////////////////////////////////////////////////////
RGBQUAD CxDib::HSLtoRGB(COLORREF cHSLColor)
{
	return HSLtoRGB(RGB2RGBQUAD(cHSLColor));
}
/////////////////////////////////////////////////////////////////////
RGBQUAD CxDib::HSLtoRGB(RGBQUAD lHSLColor)
{ 
	WORD hue,lum,sat;
	BYTE R,G,B;					/* RGB component values */
	WORD Magic1,Magic2;			/* calculated magic numbers (really!) */

	hue = lHSLColor.rgbRed;	/* get H, S, and L out of DWORD */
	sat = lHSLColor.rgbGreen;
	lum = lHSLColor.rgbBlue;

	if (sat == 0) {            /* achromatic case */
		R=G=B=(lum*RGBMAX)/HSLMAX;
	} else {                   /* chromatic case */
		/* set up magic numbers */
		if (lum <= (HSLMAX/2))
			Magic2 = (lum*(HSLMAX + sat) + (HSLMAX/2))/HSLMAX;
		else
			Magic2 = lum + sat - ((lum*sat) + (HSLMAX/2))/HSLMAX;
		Magic1 = 2*lum-Magic2;

		/* get RGB, change units from HSLMAX to RGBMAX */
		R = (HueToRGB(Magic1,Magic2,(WORD)(hue+(HSLMAX/3)))*RGBMAX+(HSLMAX/2))/HSLMAX; 
		G = (HueToRGB(Magic1,Magic2,hue)*RGBMAX + (HSLMAX/2)) / HSLMAX;
		B = (HueToRGB(Magic1,Magic2,(WORD)(hue-(HSLMAX/3)))*RGBMAX+(HSLMAX/2))/HSLMAX; 
	}
	RGBQUAD rgb={B,G,R,0};
	return rgb;
}
/////////////////////////////////////////////////////////////////////
RGBQUAD CxDib::RGB2RGBQUAD(COLORREF cr)
{
	RGBQUAD c;
	c.rgbRed = GetRValue(cr);	/* get R, G, and B out of DWORD */
	c.rgbGreen = GetGValue(cr);
	c.rgbBlue = GetBValue(cr);
	c.rgbReserved=0;
	return c;
}
/////////////////////////////////////////////////////////////////////
COLORREF CxDib::RGBQUAD2RGB (RGBQUAD c)
{
	return RGB(c.rgbRed,c.rgbGreen,c.rgbBlue);
}
/////////////////////////////////////////////////////////////////////
void CxDib::SetGrayPalette()
{
	if ((hDib==NULL)||(m_nColors==0)) return;
	RGBQUAD pal[256];
	RGBQUAD* ppal;
	BYTE* iDst;
	int ni;
	ppal=(RGBQUAD*)&pal[0];
	iDst = (BYTE*)(hDib) + sizeof(BITMAPINFOHEADER);
	for (ni=0;ni<m_nColors;ni++){ pal[ni]=RGB2RGBQUAD(RGB(ni,ni,ni));}
	pal[0]=RGB2RGBQUAD(RGB(0,0,0));
	pal[m_nColors-1]=RGB2RGBQUAD(RGB(255,255,255));
	memcpy(iDst,ppal,GetPaletteSize());
}
/////////////////////////////////////////////////////////////////////
void CxDib::BlendPalette(COLORREF cr,long perc)
{
	if ((hDib==NULL)||(m_nColors==0)) return;
	BYTE* iDst = (BYTE*)(hDib) + sizeof(BITMAPINFOHEADER);
	long i,r,g,b;
	RGBQUAD* pPal=(RGBQUAD*)iDst;
	r = GetRValue(cr);
	g = GetGValue(cr);
	b = GetBValue(cr);
	if (perc>100) perc=100;
	for(i=0;i<m_nColors;i++){
		pPal[i].rgbBlue=(BYTE)((pPal[i].rgbBlue*(100-perc)+b*perc)/100);
		pPal[i].rgbGreen =(BYTE)((pPal[i].rgbGreen*(100-perc)+g*perc)/100);
		pPal[i].rgbRed =(BYTE)((pPal[i].rgbRed*(100-perc)+r*perc)/100);
	}
}
/////////////////////////////////////////////////////////////////////
long CxDib::WriteBMP(LPSTR bmpFileName)
{                         
	if ((*bmpFileName=='\0')||(hDib==0)) return 0;
	BITMAPFILEHEADER	hdr;
	HANDLE	hFile;
	DWORD	nByteWrite;

	hFile=CreateFile(			// open if exist ini file
		bmpFileName,				// pointer to name of the file 
		GENERIC_WRITE,			// access mode 
		0,						// share mode 
		NULL,					// pointer to security descriptor 
		TRUNCATE_EXISTING,		// how to create 
		FILE_ATTRIBUTE_NORMAL,	// file attributes 
		NULL				 	// handle to file with attributes to copy  
		);
	if (hFile==INVALID_HANDLE_VALUE) return 0;	//check if file exist

    /* Fill in the fields of the file header */
	hdr.bfType = BFT_BITMAP;
	hdr.bfSize = GetSize() + sizeof(BITMAPFILEHEADER);
	hdr.bfReserved1 = hdr.bfReserved2 = 0;
	hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER)+
					m_bi.biSize + GetPaletteSize();

    // Write the file header
	WriteFile(			// write ini (sync mode <-> no overlapped)
		hFile,			// handle of file to write 
		(LPSTR) &hdr,	// address of buffer that contains data  
		sizeof(BITMAPFILEHEADER),	// number of bytes to write 
		&nByteWrite,	// address of number of bytes written 
		NULL	 		// address of structure for data 
		);

    // Write the DIB header and the bits
	WriteFile(			// write ini (sync mode <-> no overlapped)
		hFile,			// handle of file to write 
		(LPSTR) hDib,	// address of buffer that contains data  
		GetSize(),		// number of bytes to write 
		&nByteWrite,	// address of number of bytes written 
		NULL	 		// address of structure for data 
		);

	CloseHandle(hFile);		//free file handle
	return 1;
}
/////////////////////////////////////////////////////////////////////
long CxDib::GetSize()
{
	return m_bi.biSize + m_bi.biSizeImage + GetPaletteSize();
}
/////////////////////////////////////////////////////////////////////
BOOL CxDib::IsValid()
{
	return (hDib!=NULL);
}
/////////////////////////////////////////////////////////////////////
void CxDib::Clone(CxDib *src)
{
 	Create(src->GetWidth(),src->GetHeight(),src->GetBitCount());
 	if (hDib) memcpy(hDib,src->hDib,GetSize());
}
/////////////////////////////////////////////////////////////////////
void CxDib::Clear(BYTE bval)
{
	if (hDib) memset(GetBits(),bval,m_bi.biSizeImage);
}
/////////////////////////////////////////////////////////////////////













/////////////////////////////////////////////////////////////////////////////
// CxShadeButton
CxShadeButton::CxShadeButton()
{
	m_Border=1;				//draw 3D border
	m_FocusRectMargin=4;	//focus dotted rect margin
	m_TextColor=GetSysColor(COLOR_BTNTEXT); // default button text color
	m_flat = m_Checked = m_button_down = m_tracking = false;

	m_Icon=m_IconDown=m_IconHighLight=NULL;	// icon handle
	m_IconBox.SetRectEmpty(); //icon location
	m_IconAlign=BS_CENTER;	//icon alignment
	m_TextAlign=DT_CENTER;	//text alignment
	m_pLF=NULL;				//font structure pointer
}
/////////////////////////////////////////////////////////////////////////////
CxShadeButton::~CxShadeButton()
{
	m_Font.DeleteObject();
	if (m_pLF)	free(m_pLF);
	if (m_IconDown!=m_Icon && m_IconDown) DestroyIcon(m_IconDown);
	if (m_IconHighLight!=m_Icon && m_IconHighLight) DestroyIcon(m_IconHighLight);
	if (m_Icon) DestroyIcon(m_Icon);
}
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CxShadeButton, CButton)
	//{{AFX_MSG_MAP(CxShadeButton)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KILLFOCUS()
	//ON_CONTROL_REFLECT_EX(BN_CLICKED, OnClicked)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
    ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_CXSHADE_RADIO , OnRadioInfo)
	ON_MESSAGE(BM_SETCHECK , OnBMSetCheck)
	ON_MESSAGE(BM_GETCHECK , OnBMGetCheck)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CxShadeButton message handlers
/////////////////////////////////////////////////////////////////////////////
void CxShadeButton::PreSubclassWindow() 
{
	m_Style=GetStyle(); //get general button styles
	m_IsPushLike=((m_Style & BS_PUSHLIKE)!=0);
	m_flat=((m_Style & BS_FLAT)!=0);
	SetTextAlign(m_Style & 0x0300);

	m_Style=GetButtonStyle();	///get specific BS_ styles
	if ((m_Style & BS_AUTOCHECKBOX)==BS_AUTOCHECKBOX)
//		||((m_Style & BS_CHECKBOX)==BS_CHECKBOX))
		m_Style=BS_CHECKBOX;
	else if ((m_Style & BS_AUTORADIOBUTTON)==BS_AUTORADIOBUTTON)
//			||((m_Style & BS_RADIOBUTTON)==BS_RADIOBUTTON))
		m_Style=BS_RADIOBUTTON;
	else { m_Style=BS_PUSHBUTTON; m_IsPushLike=true;}

	//default radio & check-box has no border
	if (!m_IsPushLike) m_Border = false;

	CButton::PreSubclassWindow();
	ModifyStyle(0, BS_OWNERDRAW);
}
/////////////////////////////////////////////////////////////////////////////
BOOL CxShadeButton::OnEraseBkgnd(CDC* pDC) 
{ return 1; }	// doesn't erase the button background
/////////////////////////////////////////////////////////////////////////////
void CxShadeButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
    ASSERT (lpDrawItemStruct);
    //TRACE("* Captured: %08X\n", ::GetCapture());

    //Check if the button state in not in inconsistent mode...
    POINT mouse_position;
    if ((m_button_down) && (::GetCapture() == m_hWnd) && (::GetCursorPos(&mouse_position))){
		if (::WindowFromPoint(mouse_position) == m_hWnd){
			if ((GetState() & BST_PUSHED) != BST_PUSHED) {
				//TRACE("* Inconsistency up detected! Fixing.\n");
				SetState(TRUE);
				return;
			}
		} else {
			if ((GetState() & BST_PUSHED) == BST_PUSHED) {
				//TRACE("* Inconsistency up detected! Fixing.\n");
				SetState(FALSE);
				return;
	}	}	}
	
    //TRACE("* Drawing: %08x\n", lpDrawItemStruct->itemState);
	CString sCaption;
	CDC *pRealDC = CDC::FromHandle(lpDrawItemStruct->hDC);	// get device context
	RECT r=lpDrawItemStruct->rcItem;					// context rectangle
	int cx = r.right  - r.left ;						// get width
	int cy = r.bottom - r.top  ;						// get height
	// get text box position
	RECT tr={r.left+m_FocusRectMargin+2,r.top,r.right-m_FocusRectMargin-2,r.bottom};

	CDC hdcMem;	//create a memory DC to avoid flicker
	hdcMem.CreateCompatibleDC(pRealDC);
	CDC* pDC = &hdcMem; //(just use pRealDC to paint directly the screen)
    CBitmap hBitmap; //create a destination for raster operations
	hBitmap.CreateCompatibleBitmap(pRealDC,cx,cy);
	CBitmap* pOldBitmap = hdcMem.SelectObject(&hBitmap); //select the destination for MemDC

	GetWindowText(sCaption);							// get button text
	pDC->SetBkMode(TRANSPARENT);
	// with MemDC we need to select the font...

	//get text font
	CFont* pOldFont=NULL;
	if (m_Font.m_hObject) pOldFont = pDC->SelectObject(&m_Font);
	else pDC->SelectObject(GetStockObject(DEFAULT_GUI_FONT));
//	TEXTMETRIC tm;
//	pDC->GetTextMetrics(&tm);							// get font metrics

	// accommodate text location
	if (m_Icon){
		switch (m_IconAlign){
		case BS_LEFT:
			tr.left+=m_IconBox.Width();		// shift left
			break;
		case BS_RIGHT:
			tr.right-=m_IconBox.Width();	// shift right
			break;
		case BS_CENTER:
			tr.top+=m_IconBox.bottom;		// shift text under the icon
			tr.bottom-=m_FocusRectMargin + 1;
		}
	}
	// Select the correct skin 
	if (lpDrawItemStruct->itemState & ODS_DISABLED){	// DISABLED BUTTON
		if(m_dDisabled.IsValid())	// paint the skin
			m_dDisabled.Draw(pDC->GetSafeHdc(),0,0);
		else	// no skin selected for disabled state -> standard button
			pDC->FillSolidRect(&r,GetSysColor(COLOR_BTNFACE));

		if (m_Icon)	//draw the icon
			pDC->DrawState(m_IconBox.TopLeft(),m_IconBox.Size(),
				m_Icon,DSS_DISABLED,(CBrush*)NULL);

		// if needed, draw the standard 3D rectangular border
		if ((m_Border)&&(m_flat==FALSE)) pDC->DrawEdge(&r,EDGE_RAISED,BF_RECT);
		// paint the etched button text
		pDC->SetTextColor(GetSysColor(COLOR_3DHILIGHT));
		pDC->DrawText(sCaption,&tr,m_TextAlign);
		pDC->SetTextColor(GetSysColor(COLOR_GRAYTEXT));
		OffsetRect(&tr,-1,-1);
		pDC->DrawText(sCaption,&tr,m_TextAlign);
	} else {
//---------------------------------------------------------------------------
		if ((lpDrawItemStruct->itemState & ODS_SELECTED)||m_Checked){ //SELECTED (DOWN) BUTTON
			if(m_dDown.IsValid())	// paint the skin
				m_dDown.Draw(pDC->GetSafeHdc(),m_Border,m_Border);
			else	// no skin selected for selected state -> standard button
				pDC->FillSolidRect(&r,GetSysColor(COLOR_BTNFACE));

			if (m_IconDown){ //draw the pushed icon
				if (m_IsPushLike) m_IconBox.OffsetRect(1,1);
				pDC->DrawState(m_IconBox.TopLeft(),m_IconBox.Size(),
								m_IconDown,DSS_NORMAL,(CBrush*)NULL);
				if (m_IsPushLike) m_IconBox.OffsetRect(-1,-1);
			}
			//shift text
			if (m_IsPushLike) OffsetRect(&tr,1,1);
			// if needed, draw the standard 3D rectangular border
			if (m_Border){
				if (m_flat) pDC->Draw3dRect(&r,GetSysColor(COLOR_BTNSHADOW),GetSysColor(COLOR_BTNHILIGHT));
				else		pDC->DrawEdge(&r,EDGE_SUNKEN,BF_RECT);
			}
		} else {
//---------------------------------------------------------------------------
			if(m_dNormal.IsValid()){ // paint the skin			// DEFAULT BUTTON
                if ((m_tracking)&&(m_dOver.IsValid())){
					m_dOver.Draw(pDC->GetSafeHdc(),0,0);
				} else {
					m_dNormal.Draw(pDC->GetSafeHdc(),0,0);
				}
			} else	// no skin selected for normal state -> standard button
				pDC->FillSolidRect(&r,GetSysColor(COLOR_BTNFACE));

			if (m_IconHighLight && m_tracking) { // draw the highlighted icon
				pDC->DrawState(m_IconBox.TopLeft(),m_IconBox.Size(),
								m_IconHighLight,DSS_NORMAL,(CBrush*)NULL);
			} else if (m_Icon) //draw the icon
				pDC->DrawState(m_IconBox.TopLeft(),m_IconBox.Size(),
								m_Icon,DSS_NORMAL,(CBrush*)NULL);

			// if needed, draw the standard 3D rectangular border
			if ((m_Border)&&((m_tracking)||(m_flat==FALSE))){
				if (lpDrawItemStruct->itemState & ODS_DEFAULT){
					pDC->DrawEdge(&r,EDGE_SUNKEN,BF_RECT);
					InflateRect(&r,-1,-1);
					pDC->DrawEdge(&r,EDGE_RAISED,BF_RECT);
				} else {
					if (m_flat) pDC->Draw3dRect(&r,GetSysColor(COLOR_BTNHILIGHT),GetSysColor(COLOR_BTNSHADOW));
					else		pDC->DrawEdge(&r,EDGE_RAISED,BF_RECT);
				}
			}
//---------------------------------------------------------------------------
		}
		// paint the focus rect
		if ((lpDrawItemStruct->itemState & ODS_FOCUS)&&(m_FocusRectMargin>0)){
			InflateRect(&r,-m_FocusRectMargin,-m_FocusRectMargin);
			m_dh.Draw(pDC->GetSafeHdc(),1+r.left,r.top);
			m_dh.Draw(pDC->GetSafeHdc(),1+r.left,r.bottom);
			m_dv.Draw(pDC->GetSafeHdc(),r.left,1+r.top);
			m_dv.Draw(pDC->GetSafeHdc(),r.right,1+r.top);
		}
		// paint the enabled button text
		pDC->SetTextColor(m_TextColor);
		pDC->DrawText(sCaption,&tr,m_TextAlign);
	}

	if (pOldFont) pDC->SelectObject(pOldFont);	//restore the old font object

	pRealDC->BitBlt(0,0,cx,cy,&hdcMem,0,0,SRCCOPY); //copy in the real world
	//restore the old objects to avoid memory leakage <David Scambler>
	if (pOldBitmap)	hdcMem.SelectObject(pOldBitmap);
//	hdcMem.DeleteDC(); the destructor will do this for us
//	hBitmap.DeleteObject(); the destructor will do this for us

}
/////////////////////////////////////////////////////////////////////////////
void CxShadeButton::SetShade(UINT shadeID,BYTE granularity,BYTE highlight,BYTE coloring,COLORREF color)
{
	long	sXSize,sYSize,bytes,j,i,k,h;
	BYTE	*iDst ,*posDst;
	
	RECT rect;
	GetWindowRect(&rect);
	sYSize=rect.bottom-rect.top;
	sXSize=rect.right-rect.left ;

	m_dh.Create(max(1,sXSize-2*m_FocusRectMargin-1),1,8);	//create the horizontal focus bitmap
	m_dv.Create(1,max(1,sYSize-2*m_FocusRectMargin),8);	//create the vertical focus bitmap

	m_dNormal.Create(sXSize,sYSize,8);					//create the default bitmap

	COLORREF hicr=GetSysColor(COLOR_BTNHIGHLIGHT);		//get the button base colors
	COLORREF midcr=GetSysColor(COLOR_BTNFACE);
	COLORREF locr=GetSysColor(COLOR_BTNSHADOW);
	long r,g,b;											//build the shaded palette
	for(i=0;i<129;i++){
		r=((128-i)*GetRValue(locr)+i*GetRValue(midcr))/128;
		g=((128-i)*GetGValue(locr)+i*GetGValue(midcr))/128;
		b=((128-i)*GetBValue(locr)+i*GetBValue(midcr))/128;
		m_dNormal.SetPaletteIndex((BYTE)i,(BYTE)r,(BYTE)g,(BYTE)b);
		m_dh.SetPaletteIndex((BYTE)i,(BYTE)r,(BYTE)g,(BYTE)b);
		m_dv.SetPaletteIndex((BYTE)i,(BYTE)r,(BYTE)g,(BYTE)b);
	}
	for(i=1;i<129;i++){
		r=((128-i)*GetRValue(midcr)+i*GetRValue(hicr))/128;
		g=((128-i)*GetGValue(midcr)+i*GetGValue(hicr))/128;
		b=((128-i)*GetBValue(midcr)+i*GetBValue(hicr))/128;
		m_dNormal.SetPaletteIndex((BYTE)(i+127),(BYTE)r,(BYTE)g,(BYTE)b);
		m_dh.SetPaletteIndex((BYTE)(i+127),(BYTE)r,(BYTE)g,(BYTE)b);
		m_dv.SetPaletteIndex((BYTE)(i+127),(BYTE)r,(BYTE)g,(BYTE)b);
	}

	m_dNormal.BlendPalette(color,coloring);	//color the palette

	iDst=m_dh.GetBits();		//build the horiz. dotted focus bitmap
	j=(long)m_dh.GetWidth();
	for(i=0;i<j;i++){
//		iDst[i]=64+127*(i%2);	//soft
		iDst[i]=255*(i%2);		//hard
	}

	iDst=m_dv.GetBits();		//build the vert. dotted focus bitmap
	j=(long)m_dv.GetHeight();
	for(i=0;i<j;i++){
//		*iDst=64+127*(i%2);		//soft
		*iDst=255*(i%2);		//hard
		iDst+=4;
	}

	bytes = m_dNormal.GetLineWidth();
	iDst = m_dNormal.GetBits();
	posDst =iDst;
	long a,x,y,d,xs,idxmax,idxmin;

	int grainx2=RAND_MAX/(max(1,2*granularity));
	idxmax=255-granularity;
	idxmin=granularity;

	switch(shadeID){
//----------------------------------------------------
	case 8:	//SHS_METAL
		m_dNormal.Clear();
		// create the strokes
		k=40;	//stroke granularity
		for(a=0;a<200;a++){
			x=rand()/(RAND_MAX/sXSize); //stroke postion
			y=rand()/(RAND_MAX/sYSize);	//stroke position
			xs=rand()/(RAND_MAX/min(sXSize,sYSize))/2; //stroke lenght
			d=rand()/(RAND_MAX/k);	//stroke color
			for(i=0;i<xs;i++){
				if (((x-i)>0)&&((y+i)<sYSize))
					m_dNormal.SetPixelIndex(x-i,y+i,(BYTE)d);
				if (((x+i)<sXSize)&&((y-i)>0))
					m_dNormal.SetPixelIndex(sXSize-x+i,y-i,(BYTE)d);
			}
		}
		//blend strokes with SHS_DIAGONAL
		posDst =iDst;
		a=(idxmax-idxmin-k)/2;
		for(i = 0; i < sYSize; i++) {
			for(j = 0; j < sXSize; j++) {
				d=posDst[j]+((a*i)/sYSize+(a*(sXSize-j))/sXSize);
				posDst[j]=(BYTE)d;
				posDst[j]+=rand()/grainx2;
			}
			posDst+=bytes;
		}

		break;
//----------------------------------------------------
	case 7:	// SHS_HARDBUMP
		//set horizontal bump
		for(i = 0; i < sYSize; i++) {
			k=(255*i/sYSize)-127;
			k=(k*(k*k)/128)/128;
			k=(k*(128-granularity*2))/128+128;
			for(j = 0; j < sXSize; j++) {
				posDst[j]=(BYTE)k;
				posDst[j]+=rand()/grainx2-granularity;
			}
			posDst+=bytes;
		}
		//set vertical bump
		d=min(16,sXSize/6);	//max edge=16
		a=sYSize*sYSize/4;
		posDst =iDst;
		for(i = 0; i < sYSize; i++) {
			y=i-sYSize/2;
			for(j = 0; j < sXSize; j++) {
				x=j-sXSize/2;
				xs=sXSize/2-d+(y*y*d)/a;
				if (x>xs) posDst[j]=idxmin+(BYTE)(((sXSize-j)*128)/d);
				if ((x+xs)<0) posDst[j]=idxmax-(BYTE)((j*128)/d);
				posDst[j]+=rand()/grainx2-granularity;
			}
			posDst+=bytes;
		}
		break;
//----------------------------------------------------
	case 6: //SHS_SOFTBUMP
		for(i = 0; i < sYSize; i++) {
			h=(255*i/sYSize)-127;
			for(j = 0; j < sXSize; j++) {
				k=(255*(sXSize-j)/sXSize)-127;
				k=(h*(h*h)/128)/128+(k*(k*k)/128)/128;
				k=k*(128-granularity)/128+128;
				if (k<idxmin) k=idxmin;
				if (k>idxmax) k=idxmax;
				posDst[j]=(BYTE)k;
				posDst[j]+=rand()/grainx2-granularity;
			}
			posDst+=bytes;
		}
		break;
//----------------------------------------------------
	case 5: // SHS_VBUMP
		for(j = 0; j < sXSize; j++) {
			k=(255*(sXSize-j)/sXSize)-127;
			k=(k*(k*k)/128)/128;
			k=(k*(128-granularity))/128+128;
			for(i = 0; i < sYSize; i++) {
				posDst[j+i*bytes]=(BYTE)k;
				posDst[j+i*bytes]+=rand()/grainx2-granularity;
			}
		}
		break;
//----------------------------------------------------
	case 4: //SHS_HBUMP
		for(i = 0; i < sYSize; i++) {
			k=(255*i/sYSize)-127;
			k=(k*(k*k)/128)/128;
			k=(k*(128-granularity))/128+128;
			for(j = 0; j < sXSize; j++) {
				posDst[j]=(BYTE)k;
				posDst[j]+=rand()/grainx2-granularity;
			}
			posDst+=bytes;
		}
		break;
//----------------------------------------------------
	case 1:	//SHS_DIAGSHADE
		a=(idxmax-idxmin)/2;
		for(i = 0; i < sYSize; i++) {
			for(j = 0; j < sXSize; j++) {
				posDst[j]=(BYTE)(idxmin+a*i/sYSize+a*(sXSize-j)/sXSize);
				posDst[j]+=rand()/grainx2-granularity;
			}
			posDst+=bytes;
		}
		break;
//----------------------------------------------------
	case 2:	//SHS_HSHADE
		a=idxmax-idxmin;
		for(i = 0; i < sYSize; i++) {
			k=a*i/sYSize+idxmin;
			for(j = 0; j < sXSize; j++) {
				posDst[j]=(BYTE)k;
				posDst[j]+=rand()/grainx2-granularity;
			}
			posDst+=bytes;
		}
		break;
//----------------------------------------------------
	case 3:	//SHS_VSHADE:
		a=idxmax-idxmin;
		for(j = 0; j < sXSize; j++) {
			k=a*(sXSize-j)/sXSize+idxmin;
			for(i = 0; i < sYSize; i++) {
				posDst[j+i*bytes]=(BYTE)k;
				posDst[j+i*bytes]+=rand()/grainx2-granularity;
			}
		}
		break;
//----------------------------------------------------
	default:	//SHS_NOISE
		for(i = 0; i < sYSize; i++) {
			for(j = 0; j < sXSize; j++) {
				posDst[j]=128+rand()/grainx2-granularity;
			}
			posDst+=bytes;
		}
	}
//----------------------------------------------------
	m_dDisabled.Clone(&m_dNormal);	//build the other bitmaps
	m_dOver.Clone(&m_dNormal);
	m_dOver.BlendPalette(hicr,highlight);
	m_dDown.Clone(&m_dOver);
}
/////////////////////////////////////////////////////////////////////////////
void CxShadeButton::SetIcon(UINT nIcon, UINT nIconAlign, UINT nIconDown, UINT nIconHighLight)
{
	if (m_IconDown!=m_Icon && m_IconDown) DestroyIcon(m_IconDown);
	if (m_IconHighLight!=m_Icon && m_IconHighLight) DestroyIcon(m_IconHighLight);
	if (m_Icon) DestroyIcon(m_Icon);
		
	m_Icon = (HICON)::LoadImage(AfxGetInstanceHandle(),
								MAKEINTRESOURCE(nIcon),
								IMAGE_ICON, 0, 0, 0);
	if (m_Icon){							// if success...
		ICONINFO iinfo;						// get icon info
		GetIconInfo(m_Icon, &iinfo);
		m_IconBox.left=m_IconBox.top=0;
		m_IconBox.right = iinfo.xHotspot * 2;
		m_IconBox.bottom = iinfo.yHotspot * 2;

		long x,y;
		RECT rect;
		GetWindowRect(&rect);
		y=rect.bottom-rect.top;
		x=rect.right-rect.left;
		switch (nIconAlign){				//set the icon location
		case BS_RIGHT:
			m_IconAlign=BS_RIGHT;
			m_IconBox.OffsetRect(x - iinfo.xHotspot*2 - m_FocusRectMargin,
								 max(0, (long)(y/2 - iinfo.yHotspot)));
			break;
		case BS_LEFT:
			m_IconAlign=BS_LEFT;
			m_IconBox.OffsetRect(m_FocusRectMargin,
								 max(0, (long)(y/2 - iinfo.yHotspot)));
			break;
		default:
			m_IconAlign=BS_CENTER;
			m_IconBox.OffsetRect(max(0, (long)(x/2 - iinfo.xHotspot)),
								 max(0, (long)(y/2 - 2*iinfo.yHotspot)));
		}

		//release icon mask bitmaps (Orioli Alessandro <aorioli@temaweb.it>)
		if( iinfo.hbmColor ) DeleteObject( iinfo.hbmColor );
		if( iinfo.hbmMask ) DeleteObject( iinfo.hbmMask );

		if (nIconDown > 0){	//load down icon
			m_IconDown = (HICON)::LoadImage(AfxGetInstanceHandle(),
											MAKEINTRESOURCE(nIconDown),
											IMAGE_ICON, 0, 0, 0);
			if (m_IconDown==NULL) m_IconDown= m_Icon;
		} else { m_IconDown= m_Icon; }	//reuse resource handle

		if (nIconHighLight > 0){	//load highlighted icon
			m_IconHighLight = (HICON)::LoadImage(AfxGetInstanceHandle(),
											MAKEINTRESOURCE(nIconHighLight),
											IMAGE_ICON, 0, 0, 0);
			if (m_IconHighLight==NULL) m_IconHighLight= m_Icon;
		} else { m_IconHighLight= m_Icon; }	//reuse resource handle
	}
}
/////////////////////////////////////////////////////////////////////////////
void CxShadeButton::SetTextAlign(UINT nTextAlign)
{
//	see DrawText() styles...
	switch (nTextAlign){
	case BS_RIGHT:
		m_TextAlign=DT_RIGHT;
		break;
	case BS_LEFT:
		m_TextAlign=DT_LEFT;
		break;
	default:
		m_TextAlign=DT_CENTER;
	}
	m_TextAlign|=(DT_SINGLELINE|DT_VCENTER);
}
/////////////////////////////////////////////////////////////////////////////
void CxShadeButton::SetFlat(bool bFlag)
{ m_flat=bFlag; }	//set flat border mode
/////////////////////////////////////////////////////////////////////////////
COLORREF CxShadeButton::SetTextColor(COLORREF new_color)
{
	COLORREF tmp_color=m_TextColor;
	m_TextColor=new_color;
	return tmp_color;			//returns the previous color
}
/////////////////////////////////////////////////////////////////////////////
void CxShadeButton::SetToolTipText(CString s, CString sDown)
{
	m_ToolTipUp = s;		//normal tooltip
	m_ToolTipDw = sDown;	//tooltip for checked state

	//select the down tooltip if the button is already checked <Jesper Kinnås>
	if (m_Checked && !m_ToolTipDw.IsEmpty()) s = m_ToolTipDw;
	
	if(m_tooltip.m_hWnd==NULL){
		if(m_tooltip.Create(this))	//first assignment
			if(m_tooltip.AddTool(this, (LPCTSTR)s)){
				m_tooltip.Activate(1);
				// enable multiline tooltips <Derek Lakin>
				m_tooltip.SendMessage(TTM_SETMAXTIPWIDTH, 0, 300);
			}
	} else {
		m_tooltip.UpdateTipText((LPCTSTR)s,this);
	}
}
/////////////////////////////////////////////////////////////////////////////
void CxShadeButton::RelayEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
// This function will create a MSG structure, fill it in a pass it to
// the ToolTip control, m_ttip.  Note that we ensure the point is in window
// coordinates (relative to the control's window).
	if(NULL != m_tooltip.m_hWnd){
		MSG msg;
		msg.hwnd = m_hWnd;
		msg.message = message;
		msg.wParam = wParam;
		msg.lParam = lParam;
		msg.time = 0;
		msg.pt.x = LOWORD(lParam);
		msg.pt.y = HIWORD(lParam);

		m_tooltip.RelayEvent(&msg);
	}
}
/////////////////////////////////////////////////////////////////////////////
//Method......: OnLButtonDblClk
//Class.......: CxShadeButton
//
//Author......: Milan Gardian
//Created.....: MAR-2001
//
//Return value: NONE
//Parameters..: Used only to be forwarded as WM_LBUTTONDOWN message parameters
//Exceptions..: NONE
//------------
//Description :
//  > We do not care about doublelicks - handle this event
//    like an ordinary left-button-down event
//---------------------------------------------------------
void CxShadeButton::OnLButtonDblClk(UINT flags, CPoint point) 
{
    SendMessage(WM_LBUTTONDOWN, flags, MAKELPARAM(point.x, point.y));
}
/////////////////////////////////////////////////////////////////////////////
//Method......: OnLButtonDown
//Class.......: CxShadeButton
//
//Author......: Milan Gardian
//Created.....: MAR-2001
//
//Return value: NONE
//Parameters..: As follows
//    > [in] nFlags: not used
//    > [in] point: coordinates of the mouse pointer when this event was spawned
//Exceptions..: NONE
//------------
//Description :
//  > Handle event when left button is pressed down
//---------------------------------------------------------
void CxShadeButton::OnLButtonDown(UINT nFlags, CPoint point)
{
    //TRACE("* %08X: down\n", ::GetTickCount());

	//Pass this message to the ToolTip control
	RelayEvent(WM_LBUTTONDOWN,(WPARAM)nFlags,MAKELPARAM(LOWORD(point.x),LOWORD(point.y)));

    //If we are tracking this button, cancel it
    if (m_tracking) {
        TRACKMOUSEEVENT t = {
            sizeof(TRACKMOUSEEVENT),
            TME_CANCEL | TME_LEAVE,
            m_hWnd, 0
		};
        if (::_TrackMouseEvent(&t)) {
            m_tracking = false;
        }
    }

    //Default-process the message
	CButton::OnLButtonDown(nFlags, point);
    m_button_down = true;
}
/////////////////////////////////////////////////////////////////////////////
//Method......: OnLButtonUp
//Class.......: CxShadeButton
//
//Author......: Milan Gardian
//Created.....: MAR-2001
//
//Return value: NONE
//Parameters..: As follows
//    > [in] nFlags: not used
//    > [in] point: coordinates of the mouse pointer when this event was spawned
//Exceptions..: NONE
//------------
//Description :
//
//  > Handle event when left button is released (goes up)
//---------------------------------------------------------
void CxShadeButton::OnLButtonUp(UINT nFlags, CPoint point)
{
    //TRACE("* %08X: up\n", ::GetTickCount());

	if (m_Style){ //track mouse for radio & check buttons
		POINT p2 = point;
		::ClientToScreen(m_hWnd, &p2);
		HWND mouse_wnd = ::WindowFromPoint(p2);
		if (mouse_wnd == m_hWnd && m_button_down == true){ // mouse is in button
			if (m_Style==BS_CHECKBOX) SetCheck(m_Checked ? 0 : 1);
			if (m_Style==BS_RADIOBUTTON) SetCheck(1);
		}
	}

	//Pass this message to the ToolTip control
	RelayEvent(WM_LBUTTONUP,(WPARAM)nFlags,MAKELPARAM(LOWORD(point.x),LOWORD(point.y)));

    //Default-process the message
    m_button_down = false;
	CButton::OnLButtonUp(nFlags, point);
}
/////////////////////////////////////////////////////////////////////////////
//Method......: OnMouseMove
//Class.......: CxShadeButton
//
//Author......: Milan Gardian
//Created.....: MAR-2001
//
//Return value: NONE
//Parameters..: As follows
//    > [in] nFlags: not used
//    > [in] point: coordinates of the mouse pointer when this event was spawned
//Exceptions..: NONE
//------------
//Description :
//  > Handle change of mouse position: see the comments in the
//    method for further info.
//---------------------------------------------------------
void CxShadeButton::OnMouseMove(UINT nFlags, CPoint point)
{
    //TRACE("* %08X: Mouse\n", ::GetTickCount());

	//Pass this message to the ToolTip control
	RelayEvent(WM_MOUSEMOVE,(WPARAM)nFlags,MAKELPARAM(LOWORD(point.x),LOWORD(point.y)));

    //If we are in capture mode, button has been pressed down
    //recently and not yet released - therefore check is we are
    //actually over the button or somewhere else. If the mouse
    //position changed considerably (e.g. we moved mouse pointer
    //from the button to some other place outside button area)
    //force the control to redraw
    //
    if ((m_button_down) && (::GetCapture() == m_hWnd)) {
	    POINT p2 = point;
        ::ClientToScreen(m_hWnd, &p2);
        HWND mouse_wnd = ::WindowFromPoint(p2);

        bool pressed = ((GetState() & BST_PUSHED) == BST_PUSHED);
        bool need_pressed = (mouse_wnd == m_hWnd);
        if (pressed != need_pressed) {
            //TRACE("* %08X Redraw\n", GetTickCount());
            SetState(need_pressed ? TRUE : FALSE);
            Invalidate();
        }
    } else {

	//Otherwise the button is released. That means we should
    //know when we leave its area - and so if we are not tracking
    //this mouse leave event yet, start now!
    //
        if (!m_tracking) {
            TRACKMOUSEEVENT t = {
                sizeof(TRACKMOUSEEVENT),
                TME_LEAVE,
                m_hWnd,
                0
            };
            if (::_TrackMouseEvent(&t)) {
                //TRACE("* Mouse enter\n");
                m_tracking = true;
                Invalidate();
            }
        }
    }

    //Forward this event to superclass
    CButton::OnMouseMove(nFlags, point);
}
/////////////////////////////////////////////////////////////////////////////
//Method......: OnMouseLeave
//Class.......: CxShadeButton
//
//Author......: Milan Gardian
//Created.....: MAR-2001
//
//Return value: NULL
//Parameters..: NOT USED
//Exceptions..: NONE
//------------
//Description :
//  > Handle situation when mouse cursor leaves area of this
//    window (button). This event might be generated ONLY
//    if we explicitely call 'TrackMouseEvent'. This is
//    signalled by setting the m_tracking flag (see the assert
//    precondition) - in 'OnMouseMove' method
//
//  > When a mouse pointer leaves area of this button (i.e.
//    when this method is invoked), presumably the look of
//    the button changes (e.g. when hover/non-hover images are set)
//    and therefore we force the control to redraw.
//---------------------------------------------------------
LRESULT CxShadeButton::OnMouseLeave(WPARAM, LPARAM)
{
    ASSERT (m_tracking);
    //TRACE("* Mouse leave\n");
    m_tracking = false;
    Invalidate();
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
//Method......: OnKillFocus
//Class.......: CxShadeButton
//
//Author......: Milan Gardian
//Created.....: MAR-2001
//
//Return value: NONE
//Parameters..: See superclass documentation
//Exceptions..: NONE
//------------
//Description :
//  > If focus is killed during capture, we may no longer
//    have the exclusive access to user input and therefore
//    release it.
//
//  > Such a situation might happens when the user left-clicks
//    this button, keeps the button down and simultaneously
//    presses TAB key.
//---------------------------------------------------------
void CxShadeButton::OnKillFocus(CWnd *new_wnd)
{
    if (::GetCapture() == m_hWnd) {
        ::ReleaseCapture();
        ASSERT (!m_tracking);
        m_button_down = false;
    }
    CButton::OnKillFocus(new_wnd);
}
/////////////////////////////////////////////////////////////////////////////
//Method......: OnClicked
//Class.......: CxShadeButton
//
//Author......: Milan Gardian
//Created.....: MAR-2001
//
//Return value: FALSE (do not stop in this handler - forward to parent)
//Parameters..: NONE
//Exceptions..: NONE
//------------
//Description :
//  > Keep consistency of attributes of this instance before
//    submitting click event to the parent.
//
//  > Currently NOT used. To use, umcomment line
//    "ON_CONTROL_REFLECT_EX(BN_CLICKED, OnClicked)" in message map
//    at the beginning of this file.
//---------------------------------------------------------
BOOL CxShadeButton::OnClicked() 
{
    if (::GetCapture() == m_hWnd) {
        ::ReleaseCapture();
        ASSERT (!m_tracking);
    }
    m_button_down = false;
    //Invalidate();
    return FALSE;
}
/////////////////////////////////////////////////////////////////////////////
//Method......: OnRadioInfo
//Class.......: CxShadeButton
//
//Author......: Rainer Mangold
//Created.....: JUN-2001
//
//Return value: NULL
//Parameters..: WPARAM and LPARAM (LPARAM not used)
//Exceptions..: NONE
//------------
//Description :
//  > Handle notification, that a Button in the same group was pushed
//---------------------------------------------------------
LRESULT CxShadeButton::OnRadioInfo(WPARAM wparam, LPARAM)
{
	if (m_Checked){	//only checked buttons need to be unchecked
		m_Checked = false;
		if(m_tooltip.m_hWnd!=NULL)
			m_tooltip.UpdateTipText((LPCTSTR)m_ToolTipUp,this);
		Invalidate();
	}
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
void CxShadeButton::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if ((m_Style)&&(nChar==' ')){ //needed stuff for check & radio buttons
		if (m_Style==BS_CHECKBOX) SetCheck(m_Checked ? 0 : 1);
		if (m_Style==BS_RADIOBUTTON) SetCheck(1);
	}
	CButton::OnKeyDown(nChar, nRepCnt, nFlags);
}
/////////////////////////////////////////////////////////////////////////////
//Method......: SetCheck
//Class.......: CxShadeButton
//
//Author......: Rainer Mangold
//Created.....: JUN-2001
//
//Return value: NONE
//Parameters..: bool
//Exceptions..: NONE
//------------
//Description :
//  > Set the state of this button (pushed or not). 
//    Works for both, Radio and CheckBox - Buttons
//---------------------------------------------------------
LRESULT CxShadeButton::OnBMSetCheck(WPARAM wparam, LPARAM)
{
	m_Checked=wparam!=0;
	switch (m_Style)
	{
	case BS_RADIOBUTTON:
		if (m_Checked) { //uncheck the other radio buttons (in the same group)
			HWND hthis,hwnd2,hpwnd;
			hpwnd=GetParent()->GetSafeHwnd();	//get button parent handle
			hwnd2=hthis=GetSafeHwnd();			//get this button handle
			if (hthis && hpwnd){				//consistency check
				for( ; ; ){	//scan the buttons within the group
					hwnd2=::GetNextDlgGroupItem(hpwnd,hwnd2,0);
					//until we reach again this button
					if ((hwnd2==hthis)||(hwnd2==NULL)) break;
					//post the uncheck message
					::PostMessage(hwnd2, WM_CXSHADE_RADIO, 0, 0);
				}
			}
		}
		break;
	case BS_PUSHBUTTON:
		m_Checked=false;
		ASSERT(false); // Must be a Check or Radio button to use this function
	}

	//set the correct tooltip
	if (m_tooltip.m_hWnd!=NULL){
		m_tooltip.Pop();	//to avoid flicker
		if (m_Checked && !m_ToolTipDw.IsEmpty())
			m_tooltip.UpdateTipText((LPCTSTR)m_ToolTipDw,this);
		else
			m_tooltip.UpdateTipText((LPCTSTR)m_ToolTipUp,this);
	}
	Invalidate();
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
LRESULT CxShadeButton::OnBMGetCheck(WPARAM wparam, LPARAM)
{ return m_Checked; }	//returns the state for check & radio buttons
/////////////////////////////////////////////////////////////////////////////
void CxShadeButton::SetButtonStyle(UINT nStyle, BOOL bRedraw)
{
	// see PreSubclassWindow
	m_IsPushLike=((nStyle & BS_PUSHLIKE)!=0);
	m_flat=((nStyle & BS_FLAT)!=0);

	if ((nStyle & BS_AUTOCHECKBOX)==BS_AUTOCHECKBOX)
//		||((m_Style & BS_CHECKBOX)==BS_CHECKBOX))
		m_Style=BS_CHECKBOX;
	else if ((nStyle & BS_AUTORADIOBUTTON)==BS_AUTORADIOBUTTON)
//			||((m_Style & BS_RADIOBUTTON)==BS_RADIOBUTTON))
		m_Style=BS_RADIOBUTTON;
	else { m_Style=BS_PUSHBUTTON; m_IsPushLike=true; m_Checked=false; }

	//default radio & check-box has no border
	if (!m_IsPushLike) m_Border = false;

	if (bRedraw) Invalidate();
}
/////////////////////////////////////////////////////////////////////////////
bool CxShadeButton::SetFont(CString sFontName,long lSize, long lWeight, BYTE bItalic, BYTE bUnderline)
{
	if (m_pLF==NULL) m_pLF=(LOGFONT*)calloc(1,sizeof(LOGFONT));
	if (m_pLF){
		strncpy(m_pLF->lfFaceName,sFontName,31);
		m_pLF->lfHeight=lSize;
		m_pLF->lfWeight=lWeight;
		m_pLF->lfItalic=bItalic;
		m_pLF->lfUnderline=bUnderline;
		m_Font.DeleteObject();
		if (m_Font.CreateFontIndirect(m_pLF)) return true;
	}
	return false;
}
/////////////////////////////////////////////////////////////////////////////
bool CxShadeButton::SetFont(LOGFONT* pNewStyle)
{
	if (pNewStyle){
		if (m_pLF==NULL) m_pLF=(LOGFONT*)calloc(1,sizeof(LOGFONT));
		if (m_pLF){
			memcpy(m_pLF,pNewStyle,sizeof(LOGFONT));
			m_Font.DeleteObject();
			if (m_Font.CreateFontIndirect(m_pLF)) return true;
		}
	}
	return false;
}
/////////////////////////////////////////////////////////////////////////////
LOGFONT* CxShadeButton::GetFont()
{ return m_pLF; }

BOOL CxShadeButton::GetButtonState()
{
	return m_button_down;
}
/////////////////////////////////////////////////////////////////////////////
//EOF

